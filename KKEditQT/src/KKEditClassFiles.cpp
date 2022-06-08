/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:39:15 GMT 2021 keithdhedger@gmail.com

 * This file (KKEditClassFiles.cpp) is part of KKEditQT.

 * KKEditQT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * KKEditQT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with KKEditQT.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "KKEditClass.h"

void KKEditClass::runPipe(QString command)
{
	FILE		*fp=NULL;
	char		line[1024];

	fp=popen(command.toStdString().c_str(), "r");
	if(fp!=NULL)
		pclose(fp);
}

QString KKEditClass::runPipeAndCapture(QString command)
{
	QString		dump("");
	FILE		*fp=NULL;
	char		line[1024];

	fp=popen(command.toStdString().c_str(), "r");
	if(fp!=NULL)
		{
			while(fgets(line,1024,fp))
				dump+=line;
			pclose(fp);
		}
	return(dump);
}

void KKEditClass::openAsHexDump(void)
{
	QStringList fileNames;
	QString		dump;
	FILE		*fp=NULL;
	char		line[1024];
	QString		command;

	fileNames=QFileDialog::getOpenFileNames(this->mainWindow,"Open File","","",0);
	if(fileNames.count())
		{
			for (int j=0;j<fileNames.size();j++)
				{
					command=QString("hexdump -C %1").arg(fileNames.at(j));
					dump=this->runPipeAndCapture(QString("hexdump -C %1").arg(fileNames.at(j)));
					QFile		file(fileNames.at(j));
					QFileInfo	fileinfo(file);
					this->newFile(dump,QString("%1.hexdump").arg(fileinfo.fileName()));
				}
		}
}

void KKEditClass::newFile(const QString data,const QString filename)
{
	DocumentClass	*doc;
	int				tabnum;
	bool				holdsb=this->sessionBusy;
	plugData			pd;

	this->sessionBusy=true;
	doc=new DocumentClass(this);
	doc->setPlainText(data);
	tabnum=this->mainNotebook->addTab(doc,"");
	if(filename.compare("")==0)
		doc->setFileName(QString("Untitled-%1").arg(this->untitledNumber));
	else
		doc->setFileName(filename);
	doc->setTabName(this->truncateWithElipses(doc->getFileName(),this->prefsMaxTabChars));
	this->mainNotebook->setTabToolTip(tabnum,doc->getFileName());
	doc->setFilePrefs();
	doc->mimeType="text/plain";
	doc->pageIndex=this->newPageIndex;
	this->pages[this->newPageIndex++]=doc;
	doc->setHiliteLanguage();
	doc->document()->clearUndoRedoStacks(QTextDocument::UndoAndRedoStacks);
	this->untitledNumber++;
	this->mainNotebook->setCurrentWidget(doc);
	this->rebuildTabsMenu();
	this->sessionBusy=holdsb;
	this->setToolbarSensitive();

	doc->state=NORMALTAB;
	doc->setTabColourType(doc->state);

//plugins
	pd.doc=doc;
	pd.tabNumber=this->mainNotebook->currentIndex();
	pd.what=DONEWDOCUMENT;
	this->runAllPlugs(pd);
}

int KKEditClass::askSaveDialog(const QString filename)
{
	QMessageBox msgBox;

	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setText(filename+" has been modified.");
	msgBox.setInformativeText("Do you want to save your changes?");
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	return(msgBox.exec());
}

bool KKEditClass::saveFileAs(int tabnum)
{
	DocumentClass	*doc=this->getDocumentForTab(tabnum);
	QFile			file;
	QFileInfo		fileinfo;
	bool				retval=false;
	QString			dialogpath;
	int				calctabnum=this->mainNotebook->indexOf(doc);
	plugData			pd;

	if(doc->getFilePath().isEmpty()==true)
		dialogpath=getenv("HOME") + QString("/") + doc->getFileName();
	else
		dialogpath=doc->getFilePath();

	QString fileName = QFileDialog::getSaveFileName(this->mainWindow,"Save File",dialogpath);

	if(fileName.isEmpty()==true)
		return(false);
	else
		{
			file.setFileName(fileName);
			fileinfo.setFile(file);

			retval=file.open(QIODevice::Text | QIODevice::WriteOnly);
			if(retval==true)
				{
					doc->setDirPath(fileinfo.canonicalPath());
					doc->setFilePath(fileinfo.canonicalFilePath());
					doc->setFileName(fileinfo.fileName());
					this->mainNotebook->setTabToolTip(calctabnum,doc->getFilePath());
					QTextStream(&file) << doc->toPlainText() << Qt::endl;
					doc->dirty=false;
					doc->setTabName(this->truncateWithElipses(doc->getFileName(),this->prefsMaxTabChars));
					this->recentFiles->addFilePath(doc->getFilePath());
					this->setCompWordList();
					if(this->fileWatch->files().contains(fileinfo.canonicalFilePath())==false)
						this->fileWatch->addPath(fileinfo.canonicalFilePath());
					doc->state=NORMALTAB;
					doc->setTabColourType(doc->state);
					file.close();
				}
			else
				{
					QMessageBox *msg=new QMessageBox(QMessageBox::Warning,QString("Save File"),QString("Cant save file \"%1\"").arg(doc->getFileName()),QMessageBox::Ok,this->mainWindow,Qt::Dialog);
					msg->exec();
					delete msg;
				}
		}
	this->setToolbarSensitive();
	return(retval);
}

bool KKEditClass::saveFile(int tabnum,bool ask)
{
	DocumentClass	*doc=this->getDocumentForTab(tabnum);
	QFile			file;
	QFileInfo		fileinfo;
	bool				retval=false;
	plugData			pd;

	if(doc==NULL)
		return(false);

	if((doc->dirty==true) && (ask==true))
		{
			int result=this->askSaveDialog(doc->fileName);
			switch(result)
				{
					case QMessageBox::Save:
						break;
					case QMessageBox::Discard:
       					return(true);
						break;
					case QMessageBox::Cancel:
						this->sessionBusy=false;
						return(false);
						break;
				}
		}

	if((doc->getFilePath().isEmpty()==true) && (doc->dirty==true))
		return(this->saveFileAs(tabnum));
	else
		{
			if(doc->dirty==false)
				return(true);

			doc->fromMe=true;
			doc->modifiedOnDisk=false;

			file.setFileName(doc->getFilePath());
			fileinfo.setFile(file);
			if(file.exists())
				{
//plugins
					pd.doc=doc;
					pd.tabNumber=this->mainNotebook->currentIndex();
					pd.userStrData1=this->homeDataFolder;
					pd.userStrData2=doc->getDirPath();
					pd.userStrData3=doc->getFileName();
					pd.what=DOSAVE;
					this->runAllPlugs(pd);
				}
			retval=file.open(QIODevice::Text | QIODevice::WriteOnly);
			if(retval==true)
				{
					doc->fromMe=true;
					QTextStream(&file) << doc->toPlainText();
					doc->dirty=false;
					doc->setTabName(this->truncateWithElipses(doc->getFileName(),this->prefsMaxTabChars));
					this->setCompWordList();
					if(this->fileWatch->files().contains(doc->getFilePath())==false)
						this->fileWatch->addPath(fileinfo.canonicalFilePath());
					file.close();
				}
			else
				{
					QMessageBox *msg=new QMessageBox(QMessageBox::Warning,QString("Save File"),QString("Cant save file \"%1\"").arg(doc->getFileName()),QMessageBox::Ok,this->mainWindow,Qt::Dialog);
					msg->exec();
					delete msg;
				}
		}

	doc->modifiedOnDisk=false;
	doc->state=NORMALTAB;
	doc->setTabColourType(doc->state);
	return true;
}

bool KKEditClass::saveAllFiles(bool ask)
{
	this->sessionBusy=true;

	int	numtabs=this->mainNotebook->count();
	for(int loop=0;loop<numtabs;loop++)
		{
			if(this->saveFile(loop,ask)==false)
				{
					this->sessionBusy=false;
					return(false);;
				}
		}
	this->sessionBusy=false;
	return(true);
}

void KKEditClass::newEditor(int what)
{
	QString	command;

	switch(what)
		{
			case NEWADMINEDMENUITEM:
				{
					QStringList lst;
					QStringList lst2;
					lst=this->prefsRootCommand.simplified().split(" ");
					for(int j=1;j<lst.count();j++)
						lst2<<lst.at(j);
					lst2<<"kkeditqt"<<"-m";
					QProcess::startDetached(lst.at(0),lst2);
				}
				break;
			case NEWEDMENUITEM:
				QProcess::startDetached("kkeditqt",QStringList()<<"-m");
				break;
			case MANPAGEEDMENUITEM:
				if(this->gotManEditor==0)
					QProcess::startDetached("manpageeditor",QStringList());
				break;
		}
}

bool KKEditClass::checkForOpenFile(QString filepath)
{
	DocumentClass	*doc;
	for(int tabs=0;tabs<this->mainNotebook->count();tabs++)
		{
			doc=this->getDocumentForTab(tabs);
			if((doc->filePath!=NULL) && (doc->filePath.compare(filepath)==0))
				{
					this->setTabVisibilty(tabs,true);
					return(true);
				}
		}
	return(false);
}

bool KKEditClass::openFile(QString filepath,int linenumber,bool warn,bool addtorecents)
{
	DocumentClass	*doc=new DocumentClass(this);
	bool				retval=false;
	QFile			file(filepath);
	QFileInfo		fileinfo(file);
	int				tabnum;
	QMimeDatabase	db;
	QMimeType		type;
	QString			content;
	plugData			pd;
	QIcon			tabicon(QString("%1/pixmaps/empty.png").arg(DATADIR));

	if((this->prefsNoOpenduplicate==true) && (this->checkForOpenFile(filepath)==true))
		{
			if(linenumber>0)
				this->gotoLine(linenumber);
			return(true);
		}

	retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
	if(retval==true)
		{
			content=QString::fromUtf8(file.readAll());
			type=db.mimeTypeForFile(fileinfo.canonicalFilePath());
			doc->mimeType=type.name();
			doc->setPlainText(content);
			doc->setFilePrefs();
			doc->pageIndex=this->newPageIndex;
			this->pages[this->newPageIndex++]=doc;
			tabnum=this->mainNotebook->addTab(doc,tabicon,doc->getTabName());
			doc->setDirPath(fileinfo.canonicalPath());
			doc->setFilePath(fileinfo.canonicalFilePath());
			doc->setFileName(fileinfo.fileName());
			doc->setTabName(this->truncateWithElipses(doc->getFileName(),this->prefsMaxTabChars));
			this->mainNotebook->setTabToolTip(tabnum,doc->getFilePath());
			this->mainNotebook->setCurrentIndex(tabnum);
			if(linenumber>0)
				this->gotoLine(linenumber);
			doc->setHiliteLanguage();
			doc->highlighter->rehighlight();
			doc->document()->clearUndoRedoStacks(QTextDocument::UndoAndRedoStacks);
			doc->dirty=false;
			retval=true;
			if(addtorecents==true)
				this->recentFiles->addFilePath(doc->getFilePath());
			doc->setFilePrefs();
			if(this->fileWatch->files().contains(fileinfo.canonicalFilePath())==false)
				this->fileWatch->addPath(fileinfo.canonicalFilePath());
			doc->state=NORMALTAB;
			doc->setTabColourType(doc->state);
			file.close();
		}
	else
		{
#ifdef _DEBUGCODE_
			qDebug()<<"File not found ...";
#endif
			return(false);
		}

	this->rebuildTabsMenu();
	doc->dirty=false;

	if(this->openFromDialog==false)
		this->switchPage(tabnum);

	this->setToolbarSensitive();
	if(this->sessionBusy==false)
		this->setCompWordList();

//plugins
	pd.doc=doc;
	pd.tabNumber=this->mainNotebook->currentIndex();
	pd.what=DOLOAD;
	this->runAllPlugs(pd);
	return(retval);
}

QStringList KKEditClass::getNewRecursiveTagList(QString filepath)
{
	QString		sort;
	QString		command;
	QStringList	retval;
	QString		results;

	switch (this->prefsFunctionMenuLayout)
		{
			case 0:
				sort="sort -k 2rb,2rb -k 1b,1b";
				break;
			case 1:
				sort="sort -k 2rb,2rb -k 3n,3n";
				break;
			case 2:
				sort="sort -k 3n";
				break;
			case 4:
				sort="sort -k 2rb,2rb -k 1b,1b";
				break;
			default:
				sort="sort";
				break;
		}

	command=QString("find \"%1\" -maxdepth %2|ctags -L - -x|%3|sed 's@ \\+@ @g'").arg(filepath).arg(this->prefsDepth).arg(sort);
	results=this->runPipeAndCapture(command);
	retval=results.split("\n",Qt::SkipEmptyParts);
	return(retval);
}




