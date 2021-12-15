/*
 *
 * ©K. D. Hedger. Sun  5 Dec 16:00:07 GMT 2021 keithdhedger@gmail.com

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
	this->doFindReplace(sender()->objectName().toInt());
}

void KKEditClass::doFindReplace(int response_id)
{
	int									flags=0;
	DocumentClass						*document=this->getDocumentForTab(-1);
	char								*currentfindtext;
	char								*currentreplacetext;
	const char							*thetext;
	GSList								*tlist;
	GSList								*list;
	QComboBox							*combo;
	bool								flag=false;
	int									cnt;
	QString								str;
	bool								gotresult;
	QColor								lineColor=QColor(Qt::green);
	QTextDocument						*doc;

	if(document==NULL)
		return;

	doc=document->document();
	QTextCursor							newCursor(doc);

	currentfindtext=strdup(this->findDropBox->currentText().toUtf8().constData());
	currentreplacetext=strdup(this->replaceDropBox->currentText().toUtf8().constData());

	QRegExp								rx(currentfindtext);

	if(this->insensitiveSearch==true)
		rx.setCaseSensitivity(Qt::CaseInsensitive);
	else
		{
			flags+=QTextDocument::FindCaseSensitively;
			rx.setCaseSensitivity(Qt::CaseSensitive);
		}

	flags+=(((response_id==FINDPREV)<<((QTextDocument::FindBackward)-1)));

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
			list=findList;
			thetext=currentfindtext;
			if(this->useRegex==false)
				gotresult=document->find(thetext,(QTextDocument::FindFlags)flags);
			else
				gotresult=document->find(rx,(QTextDocument::FindFlags)flags);

			if((this->wrapSearch==true) && (gotresult==false))
				{
					int ln=document->textCursor().blockNumber()+1;
					this->gotoLine(0);
					if(this->useRegex==false)
						gotresult=document->find(thetext,(QTextDocument::FindFlags)flags);
					else
						gotresult=document->find(rx,(QTextDocument::FindFlags)flags);
					if(gotresult==false)
						this->gotoLine(ln);			
				}
		}
	else
		{
			combo=this->replaceDropBox;
			list=replaceList;
			thetext=currentreplacetext;
			if(this->replaceAll==false)
				{
					if(this->useRegex==false)
						{
							if(document->textCursor().hasSelection())
								document->textCursor().insertText(thetext);
							document->find(currentfindtext,(QTextDocument::FindFlags)flags);
						}
					else
						{
							if(document->textCursor().hasSelection())
								{
									str=document->textCursor().selectedText();
									str.replace(rx,thetext);
									document->textCursor().insertText(str);
								}
							document->find(rx,(QTextDocument::FindFlags)flags);
						}
				}
			else
				{
					cnt=0;
					this->gotoLine(0);
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

	if(list==NULL)
		{
			list=g_slist_append(list,strdup(thetext));
			combo->addItem(thetext);
		}
	else
		{
			tlist=list;
			flag=false;
			do
				{
					if(strcmp((const gchar*)tlist->data,thetext)==0)
						flag=true;
					tlist=tlist->next;
				}
			while(tlist!=NULL);

			if(flag==false)
				{
					list=g_slist_append(list,strdup(thetext));
					combo->addItem(thetext);
				}
		}

	debugFree(&currentfindtext,"doFindReplace currentfindtext");
	debugFree(&currentreplacetext,"doFindReplace currentreplacetext");
}

void KKEditClass::searchGtkDocs(const QString txt,int what)
{
	QString			searchfor;
	QString			funcname;
	QString			link;
	QString			basefile;
	QString			results;
	QStringList		reslist;
	QString			searchcommand;
	QFile			html(htmlFile);

	DocumentClass	*doc=this->getDocumentForTab(-1);
//<a href="/usr/share/doc/qt5/doc/qtcore/qtextstream
//			asprintf(&command,"find %s -iname \"%s*.html\"|sed 's/.html$//'|sort",QT5DOCSDIR,str->str);
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
				searchcommand=QString("find %1 -iname \"%2*.html\"|sed 's/.html$//'|sort").arg(QT5DOCSDIR).arg(searchfor);
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

	debugFree(&thePage,"thePage");
	thePage=strdup(htmlURI);
	showDocView(USEURI,(char*)searchfor.toStdString().c_str(),"Gtk Docs");
}