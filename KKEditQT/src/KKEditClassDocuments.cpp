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

#include "QT_notebook.h"
#include "QT_historyClass.h"
#include "tagClass.h"
#include "QT_document.h"
#include "KKEditClass.h"

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

#ifndef _USEQT6_
					QObject::connect(searchcombobox,QOverload<int>::of(&QComboBox::activated),[this,gottaglist](int index)
						{
							this->history->pushToBackList(this->getDocumentForTab(-1)->getCurrentLineNumber(),this->getDocumentForTab(-1)->getFilePath());
							this->openFile(gottaglist.at(index).tagFilepath,gottaglist.at(index).lineNumber,false,false);
							
						});
#else
					QObject::connect(searchcombobox,&QComboBox::activated,[this,gottaglist](int index)
						{
							this->history->pushToBackList(this->getDocumentForTab(-1)->getCurrentLineNumber(),this->getDocumentForTab(-1)->getFilePath());
							this->openFile(gottaglist.at(index).tagFilepath,gottaglist.at(index).lineNumber,false,false);
						});
#endif

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

bool KKEditClass::checkSelection(QString selection)
{
#ifdef _ASPELL_
	int							correct;
	AspellWordList*				suggestions;
	AspellStringEnumeration		*elements;
	const char*					suggestedword;
	int							wordcnt=0;
	char*						wordlist[100];
	QByteArray					ba=selection.toLocal8Bit();
	const char					*word=ba.constData();

	correct=aspell_speller_check(spellChecker,word,-1);
	this->wordDropBox->clear();

	if(!correct)
		{
			suggestions=(AspellWordList*)aspell_speller_suggest(spellChecker,word,-1);
			elements=aspell_word_list_elements(suggestions);
			while((suggestedword=aspell_string_enumeration_next(elements))!=NULL)
				{
					wordlist[wordcnt]=strdup(suggestedword);
					this->wordDropBox->addItem(wordlist[wordcnt],QVariant(wordcnt));
					wordcnt++;
				}
			delete_aspell_string_enumeration(elements);
			return(true);
		}
#endif
	return(false);
}

void KKEditClass::setUpSpellGUI(QString word,DocumentClass *doc)
{
#ifdef _ASPELL_
	if(doc==NULL)
		doc=this->getDocumentForTab(-1);

	if(this->checkSelection(word)==true)
		{
			this->infoLabel->setText(QString("Change %1 to:").arg(word));
			this->spellCheckGUI->exec();
		}
#endif
}

void KKEditClass::checkDoc(DocumentClass *doc)
{
#ifdef _ASPELL_
	AspellCanHaveError*		ret;
	AspellDocumentChecker*	checker;
	AspellToken				token;
	int						diff=0;
	unsigned int			goodwordlen;
	char*					word_begin;
	char*					badword;
	char*					line;
	QString					qstr;
	QTextCursor				cursor;
	int						docstart=0;
	QByteArray				bytearray;

	if(doc==NULL)
		return;

	cursor=doc->textCursor();
	qstr=doc->toPlainText();
	bytearray=qstr.toUtf8();
	line=(char*)bytearray.constData();

	/* Set up the document checker */
	ret=new_aspell_document_checker(this->spellChecker);
	if (aspell_error(ret)!=0)
		{
			printf("Error: %s\n",aspell_error_message(ret));
			return;
		}

	checker=to_aspell_document_checker(ret);
	  /* First process the line */
	aspell_document_checker_process(checker,line,-1);
	  /* Now find the misspellings in the line */
	while(token=aspell_document_checker_next_misspelling(checker),token.len!=0)
		{
	    /* Pay particular attention to how token.offset and diff is used */
			asprintf(&badword,"%.*s",token.len,(char*)&line[token.offset+diff]);
			this->goodWord="";
			this->cancelCheck=false;
			this->returnWord=true;
			this->badWord=badword;
			cursor.setPosition(docstart+token.offset);
			cursor.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);
			doc->setTextCursor(cursor);
			this->setUpSpellGUI(badword,NULL);

			if(this->cancelCheck==true)
				{
					delete_aspell_document_checker(checker);
					this->cancelCheck=false;
					return;
				}
			word_begin=line+token.offset+diff;

			if(this->goodWord.isEmpty()==false)
				{
					goodwordlen=this->goodWord.length();
	    /* Replace the misspelled word with the replacement */
					diff+=goodwordlen-token.len;
					memmove(word_begin+goodwordlen,word_begin+token.len,strlen(word_begin+token.len)+1);
					memcpy(word_begin,this->goodWord.toStdString().c_str(),goodwordlen);
					doc->setPlainText(line);
					docstart=diff;
				}
			}

	delete_aspell_document_checker(checker);
//replace all text in check document

	doc->setPlainText(line);
#endif
}

void KKEditClass::moveTabToRight(int newpos)
{
	if(this->openTabToRight==true)
		{
			int tabnum=this->mainNotebook->currentIndex();
			DocumentClass *doc=this->getDocumentForTab(tabnum);
			this->mainNotebook->removeTab(tabnum);
			this->mainNotebook->insertTab(newpos,doc,doc->getFileName());
			this->mainNotebook->setCurrentIndex(0);
			this->mainNotebook->setCurrentIndex(newpos);
	}
}


