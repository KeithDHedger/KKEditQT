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
	DocumentClass	*document;

	this->findDropBox->currentText().replace("\\n","\n");
	if(this->findDropBox->findText(this->findDropBox->currentText(),Qt::MatchFixedString|Qt::MatchFixedString)==-1)
		{
			this->findList.append(this->findDropBox->currentText());
			this->findDropBox->addItem(this->findDropBox->currentText());
		}

	this->replaceDropBox->currentText().replace("\\n","\n");
	if(this->replaceDropBox->findText(this->replaceDropBox->currentText(),Qt::MatchFixedString|Qt::MatchFixedString)==-1)
		{
			this->replaceList.append(this->replaceDropBox->currentText());
			this->replaceDropBox->addItem(this->replaceDropBox->currentText());
		}

	this->currentTab=this->mainNotebook->currentIndex();
	this->doFindReplace(sender()->objectName().toInt());
	if(this->hightlightAll==true)
		{
			document=this->getDocumentForTab(-1);
			if(document!=NULL)
				document->setHighlightAll();
		}
}

void KKEditClass::doFindReplace(int response_id)
{
	bool				retval;
	DocumentClass	*doc=this->getDocumentForTab(this->currentTab);

	if(doc==NULL)
		return;

	if((this->findInAllFiles==true) && (this->replaceAll==true) && (response_id==FINDREPLACE))
		{
			int	total=0;
			for(int j=0;j<this->mainNotebook->count();j++)
				{
					doc=this->getDocumentForTab(j);
					doc->findStr(FINDREPLACE);
					doc->finishedSearch=false;
					total+=doc->totalMatches;
				}
			this->statusText->setText(QString("Replaced %1 occurences in %2 files ...").arg(total).arg(this->mainNotebook->count()));
			return;
		}

	retval=doc->findStr(response_id);
	if((retval==false) && (doc->finishedSearch==true) && (this->findInAllFiles==true) && (this->searchBack==false))
		{	
			int holdtab=this->currentTab;
			bool flagjit=false;

			doc->finishedSearch=false;
			while(retval==false)
				{
					this->currentTab++;
					if(this->currentTab==this->mainNotebook->count())
						this->currentTab=0;
					if(holdtab==this->currentTab)
						{
							if(flagjit==false)
								flagjit=true;
							else
								return;
						}
					doc=this->getDocumentForTab(this->currentTab);
					doc->finishedSearch=false;
					doc->searchPos=0;
					retval=doc->findStr(FINDNEXT);
					if(retval==true)
						{
							this->setTabVisibilty(this->currentTab,true);
							break;
						}
				}
		}

	if((retval==false) && (doc->finishedSearch==true) && (this->findInAllFiles==true) && (this->searchBack==true))
		{
			int holdtab=this->currentTab;
			bool flagjit=false;

			doc->finishedSearch=false;
			while(retval==false)
				{
					this->currentTab--;
					if(this->currentTab==-1)
						this->currentTab=this->mainNotebook->count()-1;
					if(holdtab==this->currentTab)
						{
							if(flagjit==false)
								flagjit=true;
							else
								return;
						}
					doc=this->getDocumentForTab(this->currentTab);
					doc->searchPos=doc->toPlainText().length()-1;
					retval=doc->findStr(FINDNEXT);
					if(retval==true)
						{
							this->setTabVisibilty(this->currentTab,true);
							return;;
						}
				}
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

void KKEditClass::setSearchPrefs(void)
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
					case FRREPLACEFIND:
						this->findAfterReplace=this->frSwitches[FRREPLACEFIND]->isChecked();
						break;
				}
		}

	this->correctedFind=this->findDropBox->currentText().replace("\\n","\n");
	this->correctedFind=this->correctedFind.replace("\\t","\t");
	this->correctedReplace=this->replaceDropBox->currentText().replace("\\n","\n");
	this->correctedReplace=this->correctedReplace.replace("\\t","\t");
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


