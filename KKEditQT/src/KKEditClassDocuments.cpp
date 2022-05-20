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
		}
}

bool KKEditClass::goToDefinition(const QString txt)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);
	DocumentClass	*dochold=this->getDocumentForTab(-1);
	QString			searchfor;
	bool				whileflag=false;
	int				linenumber;
	QString			label="";
	QStringList		sl;
	bool				retval=true;

	if((txt.isEmpty()==true) && (doc==NULL))
		return(retval);

	if((txt.isEmpty()==true) && (doc->textCursor().hasSelection()==false))
		return(retval);
	else
		{
			if(txt.isEmpty()==true)
				searchfor=doc->textCursor().selectedText();
			else
				searchfor=txt;
		}

	Qt::CaseSensitivity casesens=Qt::CaseSensitive;
	for(int sens=0;sens<2;sens++)
		{
//optimized for speed
//exact match case sensitive this file
			doc=this->getDocumentForTab(-1);
			if(doc!=NULL)
				{
					sl=this->getNewRecursiveTagList(doc->getFilePath());
					if(sl.isEmpty()==false)
						{
							for(int loop=0;loop<sl.count();loop++)
								{
									label=sl.at(loop).section(" ",0,0);
									if( ((sens<2) && (label.compare(searchfor,casesens)==0)) || ((sens==2) && (label.contains(searchfor,casesens)==true)))
										{
											linenumber=sl.at(loop).section(" ",2,2).toInt();
											this->history->pushToBackList(dochold->getCurrentLineNumber(),dochold->getFilePath());
											this->gotoLine(linenumber);
											return(retval);
										}
								}
						}
				}
	
//check open files
//exact match case sensitive
			sl.clear();
			for(int tabs=0;tabs<this->mainNotebook->count();tabs++)
				{
					doc=this->getDocumentForTab(tabs);
					if(doc->filePath!=NULL)
						sl=this->getNewRecursiveTagList(doc->getFilePath());
//exact match case sensitive
					if(sl.isEmpty()==false)
						{
							for(int loop=0;loop<sl.count();loop++)
								{
									label=sl.at(loop).section(" ",0,0);
									if( ((sens<2) && (label.compare(searchfor,casesens)==0)) || ((sens==2) && (label.contains(searchfor,casesens)==true)))
										{
											linenumber=sl.at(loop).section(" ",2,2).toInt();
											label=sl.at(loop).section(" ",3,3);
											this->history->pushToBackList(dochold->getCurrentLineNumber(),dochold->getFilePath());
											this->setTabVisibilty(tabs,true);
											this->gotoLine(linenumber);
											return(retval);
										}

								}
						}
				}
			casesens=Qt::CaseInsensitive;
		}
//check in folder of current tab
	QString		command;
	QStringList	list;
	QString		results;
	int			start=this->mainNotebook->currentIndex();
	int			end=start+1;

	for(int k=0;k<2;k++)
		{
			for(int loop=start;loop<end;loop++)
				{
					doc=this->getDocumentForTab(loop);
					if(doc->filePath!=NULL)
						{
							command=QString("ctags -R --maxdepth=%1 -x '%2'|sed 's@ \\+@ @g'").arg(this->prefsDepth).arg(doc->getDirPath());
							results=this->runPipeAndCapture(command);
							list=results.split("\n",Qt::SkipEmptyParts);

							for(int j=0;j<list.count();j++)
								{
									label=list.at(j).section(" ",0,0);
									if(label.compare(searchfor,Qt::CaseInsensitive)==0)
										{
											this->history->pushToBackList(dochold->getCurrentLineNumber(),dochold->getFilePath());
											linenumber=list.at(j).section(" ",2,2).toInt();
											this->openFile(list.at(j).section(" ",3,3),linenumber);
											return(retval);
										}
								}
						}
				}
//last try check in folders of all tabs ... slow//TODO//
			start=0;
			end=this->mainNotebook->count();
		}
	this->statusBar->showMessage(QString("Couldn't find definition for %1").arg(searchfor),STATUSBARTIMEOUT);
	return(false);
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












