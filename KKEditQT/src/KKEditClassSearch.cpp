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
//xxx	this->startingTab=this->tabBar->currentIndex();
	this->startingTab=this->mainNotebook->currentIndex();
	this->currentTab=this->startingTab;
	this->doFindReplace(sender()->objectName().toInt());
}

void KKEditClass::setHighlightAll(void)
{
	DocumentClass				*document=this->getDocumentForTab(-1);
	QTextDocument				*doc;
	QTextDocument::FindFlags	hflags;
	QTextCursor					thiscursor;
	QRegularExpression			rx;

	if(this->hightlightAll==false)
		{
			document->clearHilites();
			return;
		}

	rx.setPattern(this->findDropBox->currentText());

	if(this->insensitiveSearch==true)
		rx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
	else
		hflags|=QTextDocument::FindCaseSensitively;

	doc=document->document();
	thiscursor=document->textCursor();
	thiscursor.movePosition(QTextCursor::Start);
	document->clearHilites();
	document->selection.format.setBackground(QColor(document->highlighter->documentForeground));//TODO// add to theme?
	document->selection.format.setForeground(QColor(document->highlighter->documentBackground));
	if(this->insensitiveSearch==false)
		hflags|=QTextDocument::FindCaseSensitively;

	while(!thiscursor.isNull() && !thiscursor.atEnd())
		{
			if(this->useRegex==false)
				thiscursor=doc->find(this->findDropBox->currentText(),thiscursor,hflags);
			else
				thiscursor=doc->find(rx,thiscursor,hflags);

			if(!thiscursor.isNull())
				{
					document->selection.cursor=thiscursor;
					document->hilightSelections.append(document->selection);
				}
		}
	document->setXtraSelections();
}

void KKEditClass::doFindReplace(int response_id)
{
	unsigned int				whattodo=0;
	bool						flag=false;
	DocumentClass			*document=this->getDocumentForTab(this->currentTab);
	QTextCursor				thiscursor;
	QTextDocument::FindFlags	flags;
	bool						foundmatch=false;
	int						position;
	QRegularExpression		rx;
	QComboBox				*box;
	QStringList				*list;

	this->setSearchPrefs(0);

	whattodo+=(this->wrapSearch);//+1
	whattodo+=(this->findInAllFiles<<1);//+2
	whattodo+=(this->replaceAll<<2);//+4

	rx.setPattern(this->findDropBox->currentText());

	if(this->insensitiveSearch==true)
		rx.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
	else
		flags|=QTextDocument::FindCaseSensitively;

	if(this->searchBack==true)
		flags|=QTextDocument::FindBackward;

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

	this->setHighlightAll();

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
				//break;
				
//			case FINDPREV:
//				if(this->searchBack==true)
//					flags|=QTextDocument::FindBackward;
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
	//								if(response_id==FINDNEXT)
									if(this->searchBack==false)
										thiscursor.movePosition(QTextCursor::Start);
									else
										thiscursor.movePosition(QTextCursor::End);
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
									if(this->searchBack==false)
								//	if(response_id==FINDNEXT)
										{
											this->currentTab++;
											if(this->currentTab==this->mainNotebook->count())
												this->currentTab=0;
										}
									else
										{
											this->currentTab--;
											if(this->currentTab<0)
												this->currentTab=this->mainNotebook->count()-1;
										}
									if(this->currentTab==this->startingTab)
										return;

									document=this->getDocumentForTab(this->currentTab);
									thiscursor=document->textCursor();
									if(this->searchBack==false)
								//	if(response_id==FINDNEXT)
										thiscursor.movePosition(QTextCursor::Start);
									else
										thiscursor.movePosition(QTextCursor::End);
									document->setTextCursor(thiscursor);
									if(this->useRegex==false)
										foundmatch=document->find(this->findDropBox->currentText(),flags);
									else
										foundmatch=document->find(rx,flags);
									if(foundmatch==true)
										{
											//this->mainNotebook->setCurrentIndex(this->currentTab);
											//this->tabBar->setTabVisible(this->mainNotebook->currentIndex(),true);
											//document->visible=true;
											this->setTabVisibilty(this->currentTab,true);
											this->setHighlightAll();
											//this->mainNotebook->repaint();
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
				searchfor=doc->textCursor().selectedText().trimmed();
			else
				searchfor=txt.trimmed();
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

	if(reslist.isEmpty()==true)
		{
			this->htmlURI=QString("https://duckduckgo.com/?q=%1&ia=web").arg(searchfor);
		}
	else
		{
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
					this->htmlURI="file://"+this->htmlFile;
				}
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
					case FRSEARCHBACK:
						this->searchBack=this->frSwitches[FRSEARCHBACK]->isChecked();
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



