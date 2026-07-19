/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:38:56 GMT 2021 keithdhedger@gmail.com

 * This file (KKEditClassDocuments.cpp) is part of KKEditQT.

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

#include "MainWindow.h"
#include "QT_notebook.h"
#include "QT_historyClass.h"
#include "tagClass.h"
#include "QT_document.h"
#include "KKEditClass.h"

#ifdef _ASPELL_
#include "QT_SpellCheck.h"
#endif

DocumentClass* KKEditClass::getDocumentForTab(int tabnum)
{
	if(tabnum==-1)
		return((DocumentClass*)this->mainNotebook->currentWidget());
	else
		return((DocumentClass*)this->mainNotebook->widget(tabnum));
}

void KKEditClass::resetAllFilePrefs(void)
{
	DocumentClass	*doc;

	for(int loop=0;loop<this->mainNotebook->count();loop++)
		{
			doc=this->getDocumentForTab(loop);
			doc->setFilePrefs();
			if(this->fileWatch->files().contains(doc->getFilePath())==false)
				this->fileWatch->addPath(doc->getFilePath());	
			doc->fromMe=false;
		}
}

bool KKEditClass::goToDefinition(const QString txt,bool singlepage)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);
	QString			searchfor;
	bool				retval=false;

	if((txt.isEmpty()==true) && (doc==NULL))
		return(retval);

	if((txt.isEmpty()==true) && (doc->textCursor().hasSelection()==false))
		return(retval);
	else
		{
			if(txt.isEmpty()==true)
				searchfor=doc->textCursor().selectedText().trimmed();
			else
				searchfor=txt.trimmed();
		}

	return(this->findDefInFolders(searchfor,singlepage));
}

bool KKEditClass::findDefInFolders(QString searchtxt,bool singlepage)
{
	DocumentClass		*doc=this->getDocumentForTab(-1);
	bool					retval=true;
	QStringList			files;
	QRegularExpression	r(searchtxt);
	tagClass				tc(this);
	QVector<tagsStruct>	gottaglist;

	r.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

	if(singlepage==false)
		{
			for(int j=0;j<this->mainNotebook->count();j++)
				{
					QDirIterator it(this->getDocumentForTab(j)->getDirPath(),{"*[^.o]"},QDir::Files|QDir::NoSymLinks|QDir::NoDotAndDotDot);
					while(it.hasNext())
						{
							it.next();
							files<<QString("%1").arg(it.filePath());
						}	
				}
		}
	else
		{
			files<<QString("%1").arg(doc->getFilePath());
		}

	files.removeDuplicates();
	tc.getTagList(files);
	for(int h=0;h<tc.tagList.count();h++)
		{
			if(tc.tagList.at(h).tagName.contains(r)==true)
				gottaglist.push_back(tc.tagList.at(h));
		}

	if(gottaglist.count()==0)
		retval=false;
	else
		{
			if(gottaglist.count()>1)
				{
					QVBoxLayout	*vlayout=new QVBoxLayout;
					QHBoxLayout	*hlayout;
					QPushButton	*button;
					QPushButton	*button1;
					QDialog		*searchdialog;
					QComboBox	*searchcombobox;

					searchdialog=new QDialog(this->mainWindow);
					searchdialog->setWindowTitle("Select Definition");
					searchdialog->setAttribute(Qt::WA_DeleteOnClose,true);
					searchcombobox=new QComboBox;

					QObject::connect(searchcombobox,&QComboBox::activated,[this,gottaglist](int index)
						{
							this->history->pushToBackList(this->getDocumentForTab(-1)->getCurrentLineNumber(),this->getDocumentForTab(-1)->getFilePath());
							this->openFile(gottaglist.at(index).tagFilepath,gottaglist.at(index).lineNumber,false,false);
						});

					for(int h=0;h<gottaglist.count();h++)
						searchcombobox->addItem(gottaglist.at(h).tagType+": "+gottaglist.at(h).tagName+" > "+QFileInfo(gottaglist.at(h).tagFilepath).fileName()+QString(":%1").arg(gottaglist.at(h).lineNumber));

					vlayout->addWidget(searchcombobox);

					hlayout=new QHBoxLayout;
					button1=new QPushButton("Goto");
					QObject::connect(button1,&QPushButton::clicked,[=]()
						{
							int get=searchcombobox->currentIndex();
							this->history->pushToBackList(this->getDocumentForTab(-1)->getCurrentLineNumber(),this->getDocumentForTab(-1)->getFilePath());
							this->openFile(gottaglist.at(get).tagFilepath,gottaglist.at(get).lineNumber,false,false);
						});
					hlayout->addStretch(1);
					hlayout->addWidget(button1);
					hlayout->addStretch(1);
					button=new QPushButton("Done");
					hlayout->addWidget(button);
					hlayout->addStretch(1);
					QObject::connect(button,&QPushButton::clicked,[=]()
						{
							searchdialog->done(0);
							delete searchdialog;
						});
				
					vlayout->addLayout(hlayout);
					searchdialog	->setLayout(vlayout);
					button1->setFocus();
					searchdialog->exec();
				}
			else
				{
					if(gottaglist.at(0).tagFilepath.isEmpty()==false)
						{
							this->history->pushToBackList(this->getDocumentForTab(-1)->getCurrentLineNumber(),this->getDocumentForTab(-1)->getFilePath());
							this->openFile(gottaglist.at(0).tagFilepath,gottaglist.at(0).lineNumber,false,false);
						}
					else
						retval=false;
				}
		}

	if(retval==false)
		this->statusBar->showMessage(QString("Couldn't find definition for %1").arg(searchtxt),STATUSBARTIMEOUT);

	this->activateMainWindow();
	return(retval);
}

bool KKEditClass::gotoLine(int linenumber)
{
	DocumentClass	*doc=NULL;
	QTextBlock		block;
	QTextCursor		cursor;
	int				theline=linenumber;

	if(theline<1)
		theline=1;

	doc=this->getDocumentForTab(-1);
	if(doc==NULL)
		return(false);
	block=doc->document()->findBlockByNumber(theline-1);

	if(block.isValid()==false)
		block=doc->document()->lastBlock();
	cursor=doc->textCursor();
	cursor.setPosition(block.position());
	doc->setTextCursor(cursor);
	return(true);
}

void KKEditClass::reloadDocument(void)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);

	if(doc==NULL)
		return;
	doc->refreshFromDisk();
}

void KKEditClass::setUpSpellGUI(QString word,DocumentClass *doc)
{
#ifdef _ASPELL_
	this->spellChecker->te=doc;
	this->spellChecker->doSpellCheckWord(word);
	if(this->spellChecker->changed==true)
		doc->makeDirty();
#endif
}

void KKEditClass::checkDoc(DocumentClass *doc)
{
#ifdef _ASPELL_
	this->spellChecker->te=doc;
	this->spellChecker->doSpellCheckDoc();
	if(this->spellChecker->changed==true)
		doc->makeDirty();
#endif
}

void KKEditClass::moveTabToRight(int newpos)
{
	if(this->openTabToRight==true)
		{
			int tabnum=this->mainNotebook->currentIndex();
			DocumentClass *doc=this->getDocumentForTab(tabnum);
			if(doc!=NULL)
				{
					this->mainNotebook->removeTab(tabnum);
					this->mainNotebook->insertTab(newpos,doc,doc->getFileName());
					this->mainNotebook->setCurrentIndex(0);
					this->mainNotebook->setCurrentIndex(newpos);
				}
	}
}


