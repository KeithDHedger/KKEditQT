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

void KKEditClass::runPipeAndCaptureToToolOP(QString command)
{
	FILE		*fp=NULL;
	char		line[1024];
	QTextCursor tc=this->toolsOPText->textCursor();

	this->toolOutputWindow->show();
	this->toggleToolWindowMenuItem->setText("Show Tool Output");
	this->toolWindowVisible=true;
	this->application->processEvents();
	fp=popen(command.toStdString().c_str(),"r");
	if(fp!=NULL)
		{
			while(fgets(line,1024,fp))
				{
					tc.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
					this->toolsOPText->setTextCursor(tc);
					this->toolsOPText->insertPlainText(line);
					this->application->processEvents();
				}
			pclose(fp);
		}
	this->application->processEvents();
}

QString KKEditClass::runPipeAndCapture(QString command)
{
	QString	dump("");
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
	QString				dump;
	chooserDialogClass	chooser(chooserDialogType::loadDialog);

	chooser.setMultipleSelect(true);
	chooser.gFind.LFSTK_sortByTypeAndName();
	chooser.setShowImagesInList(false);
	chooser.gFind.LFSTK_setIgnoreBroken(true);
	chooser.addFileTypes("All Files");
	chooser.dialogWindow.exec();
	if(chooser.valid==false)
		return;

	if(chooser.multiFileList.count()>0)
		{
			for (int j=0;j<chooser.multiFileList.count();j++)
				{
					dump=this->runPipeAndCapture(QString("hexdump -C '%1'").arg(chooser.multiFileList.at(j)));
					QFile		file(chooser.multiFileList.at(j));
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
	int				ci=this->mainNotebook->currentIndex()+1;

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

	this->moveTabToRight(ci);

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

bool KKEditClass::saveFileAs(int tabnum,QString filepath)
{
	DocumentClass	*doc=this->getDocumentForTab(tabnum);
	QFile			file;
	QFileInfo		fileinfo;
	bool				retval=false;
	QString			dialogpath;
	int				calctabnum=this->mainNotebook->indexOf(doc);
	plugData			pd;
	QString			fileName="";

	if(filepath.isEmpty()==true)
		{
			if(doc->getFilePath().isEmpty()==true)
				dialogpath=getenv("HOME") + QString("/") + doc->getFileName();
			else
				dialogpath=doc->getFilePath();

			chooserDialogClass	chooser(chooserDialogType::saveDialog,doc->getFileName());
			chooser.gFind.LFSTK_sortByTypeAndName();
			chooser.gFind.LFSTK_setIgnoreBroken(true);
			chooser.setShowImagesInList(false);
			chooser.addFileTypes(".cpp;.c;.h;.hpp;.m;.mm;.py;.go;.java;.js;.rb;.sh;.rs;.tcl;.pl");
			chooser.addFileTypes(".html;.xml;.css;.php;.pro;.in;.am;.m4;.md;.ac;.json;.class;.sql");
			chooser.addFileTypes("All Files");
			chooser.dialogWindow.exec();
			if(chooser.valid==false)
				return(false);
			fileName=chooser.selectedFilePath;
		}
	else
		{
			fileName=filepath;
		}

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
	this->rebuildTabsMenu();
	this->rebuildFunctionMenu(tabnum);

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
			if((doc->filePath.isEmpty()==false) && (doc->filePath.compare(filepath)==0))
				{
					this->setTabVisibilty(tabs,true);
					return(true);
				}
		}
	return(false);
}

bool KKEditClass::openFile(QString filepath,int linenumber,bool warn,bool addtorecents)//TODO//warn
{
	DocumentClass	*doc=new DocumentClass(this);
	bool				retval=false;

	int				correctedln=linenumber;
	int				tabnum;
	QMimeDatabase	db;
	QMimeType		type;
	QString			content;
	plugData			pd;
	QIcon			tabicon(QString("%1/pixmaps/empty.png").arg(DATADIR));

	QString			corrected=LFSTK_UtilityClass::LFSTK_strStr(filepath.toStdString(),"@").c_str();
	if(corrected.isEmpty()==false)
		{
			QString line=corrected;
			corrected=LFSTK_UtilityClass::LFSTK_strReplaceAllStr(filepath.toStdString(),corrected.toStdString(),"",true).c_str();
			correctedln=QString(LFSTK_UtilityClass::LFSTK_strReplaceAllChar(line.toStdString(),"@","",true).c_str()).toInt();
		}
	else
		{
			corrected=filepath;
		}
		
	QFile			file(corrected);
	QFileInfo		fileinfo(file);

	if((this->prefsNoOpenduplicate==true) && (this->checkForOpenFile(fileinfo.canonicalFilePath())==true))
		{
			if(correctedln>0)
				this->gotoLine(correctedln);
			return(true);
		}

			int ci=this->mainNotebook->currentIndex()+1;
	retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
	if(retval==true)
		{
			content=QString::fromUtf8(file.readAll());
			type=db.mimeTypeForFile(fileinfo.canonicalFilePath());
			doc->mimeType=type.name();
			doc->setPlainText(content);
			doc->pageIndex=this->newPageIndex;
			this->pages[this->newPageIndex++]=doc;
			tabnum=this->mainNotebook->addTab(doc,tabicon,doc->getTabName());
			doc->setDirPath(fileinfo.canonicalPath());
			doc->setFilePath(fileinfo.canonicalFilePath());
			doc->setFileName(fileinfo.fileName());
			doc->setTabName(this->truncateWithElipses(doc->getFileName(),this->prefsMaxTabChars));
			this->mainNotebook->setTabToolTip(tabnum,doc->getFilePath());
			this->mainNotebook->setCurrentIndex(tabnum);
			if(correctedln>0)
				this->gotoLine(correctedln);
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
			doc->oldBlockCount=doc->blockCount();
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
