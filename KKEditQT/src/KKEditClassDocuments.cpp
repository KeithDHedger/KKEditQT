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

#include "KKEditClass.h"

DocumentClass* KKEditClass::getDocumentForTab(int tabnum)
{
	if(tabnum==-1)
		return(qobject_cast<DocumentClass*>(this->mainNotebook->currentWidget()));
	else
		return(qobject_cast<DocumentClass*>(this->mainNotebook->widget(tabnum)));
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

bool KKEditClass::goToDefinition(const QString txt)
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

	return(this->findDefInFolders(searchfor));
}

bool KKEditClass::findDefInFolders(QString searchtxt)
{
	struct docResultStruct
		{
			QString tagName;
			QString tagType;
			int lineNumber;
			QString tagPath;
		};

	QString						command;
	QStringList					list;
	QString						comresults;
	QString						label="";
	QMap<int,docResultStruct>	resultmap;
	int							cnt=0;
	DocumentClass				*doc=this->getDocumentForTab(-1);
	int							linenumber;
	bool							retval=true;
	QStringList					folders;
	QString						f;
	QString						s;

	for(int j=0;j<this->mainNotebook->count();j++)
		folders<<"'"+this->getDocumentForTab(j)->getDirPath()+"'";

	folders.removeDuplicates();
	f=folders.join(" ");
	command=QString("find %1 -maxdepth 1 -mindepth 1 -type f -iname '[^.][^moc]*[^.o]' |ctags -L - -x").arg(f);
	comresults=this->runPipeAndCapture(command);
	list=comresults.split("\n",Qt::SkipEmptyParts);

	for(int j=0;j<list.count();j++)
		{
			s=list.at(j).simplified();
			label=s.section(" ",0,0);
			if(label.contains(searchtxt,Qt::CaseInsensitive)==true)
				{
					resultmap[cnt++]={s.section(" ",0,0),s.section(" ",1,1),s.section(" ",2,2).toInt(),s.section(" ",3,3)};
				}
		}

	if(resultmap.size()>1)
		{
			QVBoxLayout	*vlayout=new QVBoxLayout;
			QWidget		*hbox;
			QHBoxLayout	*hlayout;
			QPushButton	*button;
			QDialog		*searchdialog;
			QComboBox	*searchcombobox;

			searchdialog=new QDialog(this->mainWindow);
			searchdialog->setWindowTitle("Select Definition");
			searchdialog->setAttribute(Qt::WA_DeleteOnClose,true);
			searchcombobox=new QComboBox;
			connect(searchcombobox,QOverload<int>::of(&QComboBox::activated),[=](int index)
				{
					this->history->pushToBackList(this->getDocumentForTab(-1)->getCurrentLineNumber(),this->getDocumentForTab(-1)->getFilePath());
					this->openFile(resultmap[index].tagPath,resultmap[index].lineNumber,false,false);
				});

			for(int h=0;h<resultmap.size();h++)
				searchcombobox->addItem(resultmap[h].tagType+": "+resultmap[h].tagName+" > "+QFileInfo(resultmap[h].tagPath).fileName());
			vlayout->addWidget(searchcombobox);

			hlayout=new QHBoxLayout;
			button=new QPushButton("OK");
			
			QObject::connect(button,&QPushButton::clicked,[=]()
				{
					searchdialog->done(0);
				});
			hlayout->addStretch(1);
			hlayout->addWidget(button);
			hlayout->addStretch(1);
			vlayout->addLayout(hlayout);

			searchdialog	->setLayout(vlayout);
			button->setFocus();
			searchdialog->show();
		}
	else
		{
			if(resultmap[0].tagPath.isEmpty()==false)
				{
					this->history->pushToBackList(this->getDocumentForTab(-1)->getCurrentLineNumber(),this->getDocumentForTab(-1)->getFilePath());
					this->openFile(resultmap[0].tagPath,resultmap[0].lineNumber,false,false);
				}
			else
				retval=false;
		}

	if(retval==false)
		this->statusBar->showMessage(QString("Couldn't find definition for %1").arg(searchtxt),STATUSBARTIMEOUT);

	return(retval);
}

void KKEditClass::gotoLine(int linenumber)
{
	DocumentClass	*doc=NULL;
	QTextBlock		block;
	QTextCursor		cursor;
	int				theline=linenumber;

	if(theline<1)
		theline=1;

	doc=this->getDocumentForTab(-1);
	if(doc==NULL)
		return;
	block=doc->document()->findBlockByNumber(theline-1);

	if(block.isValid()==false)
		block=doc->document()->lastBlock();
	cursor=doc->textCursor();
	cursor.setPosition(block.position());
	doc->setTextCursor(cursor);
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
	AspellStringEnumeration*	elements;
	const char*					suggestedword;
	int							wordcnt=0;
	char*						wordlist[100];
	const char					*word=selection.toStdString().c_str();

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



