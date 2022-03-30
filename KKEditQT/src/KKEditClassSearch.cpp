/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:39:32 GMT 2021 keithdhedger@gmail.com

 * This file (KKEditClassSearch.cpp) is part of KKEditQT.

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

void KKEditClass::doFindButton(void)
{
	this->startingTab=this->tabBar->currentIndex();
	this->currentTab=this->startingTab;
	this->doFindReplace(sender()->objectName().toInt());
}

#if 1
void KKEditClass::doFindReplace(int response_id)
{

/*
#define FINDNEXTOBJECTNAME "1"
#define FINDPREVOBJECTNAME "2"
#define FINDREPLACEOBJECTNAME "3"

*/
	unsigned int				whattodo=0;
	bool						flag=false;
	DocumentClass				*document=this->getDocumentForTab(this->currentTab);
	QTextCursor					thiscursor;
	QTextDocument::FindFlags	flags;
	bool						foundmatch=false;
	int							position;
	QRegularExpression			rx;
	QComboBox					*box;
	QStringList					*list;

	this->setSearchPrefs(0);

	whattodo+=(this->wrapSearch);//+1
	whattodo+=(this->findInAllFiles<<1);//+2
	whattodo+=(this->replaceAll<<2);//+4

	rx.setPattern(this->findDropBox->currentText());

	if(this->insensitiveSearch==true)
		rx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
	else
		flags|=QTextDocument::FindCaseSensitively;

//save combos
	box=this->findDropBox;
	list=&(this->findList);
	for(int k=0;k<2;k++)
		{
			if(box->currentText().isEmpty()==false)
				{
					for(int j=0;j<box->count();j++)
						{
							if(box->currentText().compare(box->itemText(j))==0)
								flag=true;
						}
					if(flag==false)
						box->addItem(box->currentText());
				}

			for(int j=0;j<box->count();j++)
				list->append(box->itemText(j));
			list->removeDuplicates();
			box=this->replaceDropBox;
			list=&(this->replaceList);
		}

//flags+=(((response_id==FINDPREV)<<((QTextDocument::FindBackward)-1)));
	switch(response_id)
		{
			case FINDREPLACE:
				if((whattodo & 4)==4)
					{
						QString	txt;
						QString	newstr;
						int		start=this->mainNotebook->currentIndex();
						int		end=this->mainNotebook->currentIndex()+1;

						if((whattodo & 2)==2)
							{
								start=0;
								end=this->mainNotebook->count();
							}

						for(int j=start;j<end;j++)
							{
								document=this->getDocumentForTab(j);
								txt=document->toPlainText();
								newstr=document->toPlainText();
								if(this->useRegex==false)
									txt.replace(this->findDropBox->currentText(),this->replaceDropBox->currentText(),(Qt::CaseSensitivity)!this->insensitiveSearch);
								else
									txt.replace(rx,this->replaceDropBox->currentText());

								if(txt.compare(newstr)!=0)
									{
										thiscursor=document->textCursor();
										thiscursor.beginEditBlock();
											thiscursor.select(QTextCursor::Document);
											thiscursor.removeSelectedText();
											thiscursor.insertText(txt);
										thiscursor.endEditBlock();
									}
							}
						break;
					}

				if(this->useRegex==false)
					{
						if(document->textCursor().hasSelection())
							{
								if(document->textCursor().selectedText().compare(this->findDropBox->currentText())==0)
									document->textCursor().insertText(this->replaceDropBox->currentText());
							}
					}
				else
					{
						if(document->textCursor().hasSelection())
							{
								QString	str=document->textCursor().selectedText();
								str.replace(rx,this->replaceDropBox->currentText());
								document->textCursor().insertText(str);
							}
					}
				
			case FINDPREV:
				flags|=QTextDocument::FindBackward;
			case FINDNEXT:
				if(this->useRegex==false)
					foundmatch=document->find(this->findDropBox->currentText(),flags);
				else
					foundmatch=document->find(rx,flags);

				if(foundmatch==false)//now what?
					{
						switch(whattodo & 3)
							{
								case 0:
									return;
									break;
								case 1://wrap only
									position=document->textCursor().position();
									thiscursor=document->textCursor();
									thiscursor.setPosition(0);
									document->setTextCursor(thiscursor);
									if(this->useRegex==false)
										foundmatch=document->find(this->findDropBox->currentText(),flags);
									else
										foundmatch=document->find(rx,flags);

									if(foundmatch==false)
										{
											thiscursor=document->textCursor();
											thiscursor.setPosition(position);
											document->setTextCursor(thiscursor);
										}
									break;
								case 2:	//all files implies wrap
								case 3:
									this->currentTab++;
									if(this->currentTab==this->mainNotebook->count())
										this->currentTab=0;
									if(this->currentTab==this->startingTab)
										return;

									document=this->getDocumentForTab(this->currentTab);
									thiscursor=document->textCursor();
									thiscursor.setPosition(0);
									document->setTextCursor(thiscursor);
									if(this->useRegex==false)
										foundmatch=document->find(this->findDropBox->currentText(),flags);
									else
										foundmatch=document->find(rx,flags);
									if(foundmatch==true)
										{
											this->mainNotebook->setCurrentIndex(this->currentTab);
											this->tabBar->setTabVisible(this->mainNotebook->currentIndex(),true);
											this->mainNotebook->repaint();
										}
									else
										{
											this->doFindReplace(response_id);
										}
									break;
							}
					}
				break;
		}
}
#else
void KKEditClass::doFindReplace(int response_id)
{
	int									flags=0;
	DocumentClass						*document=this->getDocumentForTab(-1);
	char								*currentfindtext;//TODO//
	char								*currentreplacetext;
	const char							*thetext;
	QStringList							*tlist;
	QStringList							*list;
	QComboBox							*combo;
	bool								flag=false;
	int									cnt;
	QString								str;
	bool								gotresult;
	QColor								lineColor=QColor(Qt::green);
	QTextDocument						*doc;
	QTextCursor							newCursor;
	QRegularExpression					rx;
	int									currentline;
	int									comboindex;

	if(document==NULL)
		return;

	doc=document->document();

	this->setSearchPrefs(0);

	currentline=document->getCurrentLineNumber();
	newCursor=document->textCursor();
	currentfindtext=strdup(this->findDropBox->currentText().toUtf8().constData());
	currentreplacetext=strdup(this->replaceDropBox->currentText().toUtf8().constData());

	rx.setPattern(currentfindtext);

	if(this->insensitiveSearch==true)
		rx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
	else
		{
			flags+=QTextDocument::FindCaseSensitively;
		}

	flags+=(((response_id==FINDPREV)<<((QTextDocument::FindBackward)-1)));//TODO// wrap backwards do all files

	if((response_id==FINDNEXT) && (this->hightlightAll==true))
		{
			document->clearHilites();
			document->selection.format.setBackground(lineColor);
			while(!newCursor.isNull() && !newCursor.atEnd())
				{
					if(this->useRegex==false)
						newCursor=doc->find(currentfindtext,newCursor,(QTextDocument::FindFlags)flags);
					else
						newCursor=doc->find(rx,newCursor,(QTextDocument::FindFlags)flags);
					
					if(!newCursor.isNull())
						{
							document->selection.cursor=newCursor;
							document->hilightSelections.append(document->selection);
						}
				}
				document->setXtraSelections();
		}

	if(this->hightlightAll==false)
		{
			document->clearHilites();
		}

	if(response_id!=FINDREPLACE)
		{
			combo=this->findDropBox;
			list=&this->findList;
			comboindex=combo->currentIndex();
			thetext=currentfindtext;
			if(this->useRegex==false)
				gotresult=document->find(thetext,(QTextDocument::FindFlags)flags);
			else
				gotresult=document->find(rx,(QTextDocument::FindFlags)flags);

			if((this->wrapSearch==true) && (gotresult==false))
				{qDebug() << " 1 " << gotresult;
					if(this->findInAllFiles==true)
					{
						int tabnum=this->tabBar->currentIndex()+1;
						if(tabnum==this->startingTab)
							return;
						if(tabnum>this->tabBar->count())
							{
								tabnum=0;
							}
						document=this->getDocumentForTab(tabnum);
					}
					
					int ln=document->getCurrentLineNumber();
					this->gotoLine(0);
					if(this->useRegex==false)
						gotresult=document->find(thetext,(QTextDocument::FindFlags)flags);
					else
						gotresult=document->find(rx,(QTextDocument::FindFlags)flags);
					if(gotresult==false)
					{
						qDebug() << " 2 " << gotresult;
						this->gotoLine(ln);
					}
				}
			
			if((this->wrapSearch==true) && (gotresult==false) && (response_id==FINDPREV))
				{
					int ln=document->blockCount();
					this->gotoLine(ln);
					if(this->useRegex==false)
						gotresult=document->find(thetext,(QTextDocument::FindFlags)flags);
					else
						gotresult=document->find(rx,(QTextDocument::FindFlags)flags);
					if(gotresult==false)
						this->gotoLine(0);
				}
		}
	else
		{
			combo=this->replaceDropBox;
			list=&this->replaceList;
			thetext=currentreplacetext;
			if(this->replaceAll==false)
				{
					if(this->useRegex==false)
						{
							if(document->textCursor().hasSelection())
								document->textCursor().insertText(thetext);
							gotresult=document->find(currentfindtext,(QTextDocument::FindFlags)flags);
						}
					else
						{
							if(document->textCursor().hasSelection())
								{
									str=document->textCursor().selectedText();
									str.replace(rx,thetext);
									document->textCursor().insertText(str);
								}
							gotresult=document->find(rx,(QTextDocument::FindFlags)flags);
							
						}
//				if((this->wrapSearch==true) && (gotresult==false) && (response_id==FINDREPLACE))
//				{
//								fprintf(stderr,"FINDNEXT\n");
//								//this->gotoLine(1);
//				//this->doFindReplace(FINDREPLACE);
//				}

				}
			else
				{
					cnt=0;
					this->gotoLine(0);
						newCursor=document->textCursor();
					if(this->useRegex==false)
						document->find(currentfindtext,(QTextDocument::FindFlags)flags);
					else
						document->find(rx,(QTextDocument::FindFlags)flags);

					while(!newCursor.isNull() && !newCursor.atEnd())
						{
							if(this->useRegex==false)
								newCursor=doc->find(currentfindtext,newCursor,(QTextDocument::FindFlags)flags);
							else
								newCursor=doc->find(rx,newCursor,(QTextDocument::FindFlags)flags);
					
							if(!newCursor.isNull())
								{
									if(this->useRegex==false)
										{
											newCursor.insertText(thetext);
											cnt++;
										}
									else
										{
											str=newCursor.selectedText();
											str.replace(rx,thetext);
											newCursor.insertText(str);
											cnt++;
										}
								}
						}
					this->statusBar->showMessage(QString("Replaced %1 occurrances of %2 with %3").arg(cnt).arg(currentfindtext).arg(thetext),STATUSBARTIMEOUT);
				}
		}
qDebug() << " 3 " << gotresult;
	if(QString(thetext).isEmpty()==false)
		{
			flag=false;
			for(int j=0;j<list->count();j++)
				{
					if(list->at(j).compare(thetext)==0)
						flag=true;
				}
			if(flag==false)
				{
					list->append(thetext);
					combo->addItem(thetext);
				}
		}

	if (currentfindtext!=NULL) free(currentfindtext);currentfindtext=NULL;//TODO//
	if (currentreplacetext!=NULL) free(currentreplacetext);currentreplacetext=NULL;
}
#endif

void KKEditClass::searchAPIDocs(const QString txt,int what)
{
	QString			searchfor;
	QString			funcname;
	QString			link;
	QString			basefile;
	QString			results;
	QStringList		reslist;
	QString			searchcommand;
	QFile			html(this->htmlFile);

	DocumentClass	*doc=this->getDocumentForTab(-1);

	if((txt.isEmpty()==true) && (doc==NULL))
		return;

	if((txt.isEmpty()==true) && (doc->textCursor().hasSelection()==false))
		return;
	else
		{
			if(txt.isEmpty()==true)
				searchfor=doc->textCursor().selectedText();
			else
				searchfor=txt;
		}

	switch(what)
		{
			case 0:
				searchcommand=QString("find /usr/share/gtk-doc/html -iname \"*.devhelp2\" -exec grep -iHe %1 '{}' \\;").arg(searchfor);
				break;
			case 1:
				searchcommand=QString("find %1 -iname \"%2*.html\"|sed 's/.html$//'|sort").arg(this->prefsQtDocDir).arg(searchfor);
				break;
		}

	results=this->runPipeAndCapture(searchcommand);
	reslist=results.split("\n",Qt::SkipEmptyParts);

	if(html.open(QFile::WriteOnly|QFile::Truncate))
		{
     		QTextStream out(&html);
			out << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">" << Qt::endl;
			out << "<html>" << Qt::endl;
			out << "<head>" << Qt::endl;
			out << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">" << Qt::endl;
			out << "</head>" << Qt::endl;
			out << "<body>" << Qt::endl;

			for(int loop=0;loop<reslist.count();loop++)
				{
					switch(what)
						{
							case 0:
								funcname=reslist.at(loop).section("\" link=",0,0).section("name=\"",1,1);
								link=reslist.at(loop).section("link=\"",1,1).section("\"",0,0);
								basefile=reslist.at(loop).section(":",0,0).section("/",0,-2);
								out << "<a href=\"file://" << basefile << "/" << link << "\">" << funcname << "</a><br>" << Qt::endl;
								break;
							case 1:
								link="file://" + reslist.at(loop) + ".html";
								funcname=reslist.at(loop).section("/",-1,-1);
								out << "<a href=\"" << link << "\">" << funcname << "</a><br>" << Qt::endl;
								break;
						}
				}
			out << "</body>" << Qt::endl;
			out << "</html>" << Qt::endl;
			html.close();
		}

	this->showWebPage("Results for: " + searchfor,this->htmlURI);
}

void KKEditClass::setSearchPrefs(int state)
{
	for(int j=FRCASE;j<FRMAXSWITCHES;j++)
		{
			switch(j)
				{
					case FRCASE:
						this->insensitiveSearch=this->frSwitches[FRCASE]->isChecked();
						break;
					case FRUSEREGEX:
						this->useRegex=this->frSwitches[FRUSEREGEX]->isChecked();
						break;
					case FRWRAP:
						this->wrapSearch=this->frSwitches[FRWRAP]->isChecked();
						break;
					case FRALLFILES:
						this->findInAllFiles=this->frSwitches[FRALLFILES]->isChecked();
						break;
					case FRHIGHLIGHTALL:
						this->hightlightAll=this->frSwitches[FRHIGHLIGHTALL]->isChecked();
						break;
					case FRREPLACEALL:
						this->replaceAll=this->frSwitches[FRREPLACEALL]->isChecked();
						if(this->replaceAll==false)
							this->frReplace->setText("Replace");
						else
							this->frReplace->setText("Replace All");
						break;
				}
		}
}

void KKEditClass::functionSearchDialog(void)
{
	bool			ok;
	DocumentClass	*doc=this->getDocumentForTab(-1);
	QString			defaulttxt="";

	if(doc!=NULL)
		{
			if(doc->textCursor().hasSelection()==true)
				defaulttxt=doc->textCursor().selectedText();
		}

	QString	text=QInputDialog::getText(this->mainWindow,"Find Definition","Enter Definition",QLineEdit::Normal,defaulttxt,&ok);

	if ((ok==true) && (!text.isEmpty()))
		this->goToDefinition(text);
}

void KKEditClass::doLiveSearch(const QString text)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);
	bool			retval;
	QTextCursor		savetc=doc->textCursor();

	if(this->application->queryKeyboardModifiers()==Qt::ShiftModifier)
		{
			QTextCursor tc=doc->textCursor();
			tc.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,text.length());
			doc->setTextCursor(tc);
			retval=doc->find(text,QTextDocument::FindBackward);
			if(retval==false)
				{
					QTextCursor tc=doc->textCursor();
					tc.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
					doc->setTextCursor(tc);
					retval=doc->find(text,QTextDocument::FindBackward);
				}
		}
	else
		{
			QTextCursor savetc=doc->textCursor();
			retval=doc->find(text);
			if(retval==false)
				{
					QTextCursor tc=doc->textCursor();
					tc.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
					doc->setTextCursor(tc);
					retval=doc->find(text);
				}
		}
	if(retval==false)
		doc->setTextCursor(savetc);
}
