/*
 *
 * ©K. D. Hedger. Fri  5 Jul 11:15:45 BST 2024 keithdhedger@gmail.com

 * This file (DocumentationPlugin.cpp) is part of DocumentationPlugin.

 * DocumentationPlugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * DocumentationPlugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with DocumentationPlugin.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DocumentationPlugin.h"
//QPainter
//gtk_window
//cairo_save
//gtk_main
//gtk_window_get_title
QString DocumentationPlugin::runPipeAndCapture(QString command)
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

void DocumentationPlugin::runSearch(QString command)
{
	QTextCursor	tc;

	if(this->doc==NULL)
		return;

	tc=this->doc->textCursor();

	if(tc.hasSelection()==true)
		{
			QString		comm=QString("pushd %1/actions &>/dev/null;%2 %3 %4;popd &>/dev/null").arg(QFileInfo(this->plugPath).absolutePath()).arg(command).arg(tc.selectedText().trimmed()).arg(this->mainKKEditClass->htmlFile);
			QFile		html(this->mainKKEditClass->htmlFile);
			QStringList	reslist;
			QString		results=this->runPipeAndCapture(comm);

			if(results.compare("Just Open\n")==0)
				{
					comm=QString("kkeditqtmsg -k %1 -c openindocview -d %2").arg(this->mainKKEditClass->sessionID).arg(this->mainKKEditClass->htmlFile);
					system(comm.toStdString().c_str());
					return;
				}

			reslist=results.split("\n",Qt::SkipEmptyParts);
			if(reslist.isEmpty()==false)
				{
					if(reslist.size()==3)
						{
							comm=QString("kkeditqtmsg -k %1 -c openindocview -d %2").arg(this->mainKKEditClass->sessionID).arg(reslist.at(1));
							system(comm.toStdString().c_str());
							return;
						}

					if(html.open(QFile::WriteOnly|QFile::Truncate))
						{
							QTextStream out(&html);

							out << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">" << Qt::endl;
							out << "<html>" << Qt::endl;
							out << "<head>" << Qt::endl;
							out << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">" << Qt::endl;
							out << "</head>" << Qt::endl;
							out << "<body>" << Qt::endl;
							if(reslist.size()>0)
								out << "<span style=\"font-weight: bold;\">"<<reslist.at(0)<<"</span></font><br>" << Qt::endl;
							for(int loop=1;loop<reslist.size();loop+=2)
								{
									if(reslist.size()>loop+1)
										out << "<a href=\"" << reslist.at(loop) << "\">" << reslist.at(loop+1) << "</a><br>" << Qt::endl;
								}					
							out << "</body>" << Qt::endl;
							out << "</html>" << Qt::endl;
							html.close();
							this->mainKKEditClass->htmlURI="file://"+this->mainKKEditClass->htmlFile;
							comm=QString("kkeditqtmsg -k %1 -c openindocview -d %2").arg(this->mainKKEditClass->sessionID).arg(this->mainKKEditClass->htmlFile);
							system(comm.toStdString().c_str());
						}
				}
		}
}

void DocumentationPlugin::initPlug(KKEditClass *kk,QString pathtoplug)
{
	QString		results;
	QStringList	reslist;
	QString		command;
	bool			breakit=false;

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;
	this->apiMenu=new QMenu("Find API");
	this->apiMenu->setIcon(QIcon::fromTheme("edit-find"));

	foreach(QAction *action,this->mainKKEditClass->navMenu->actions())
		{
			if(breakit==true)
				{
					this->mainKKEditClass->navMenu->insertMenu(action,this->apiMenu);
					break;
				}
			if(action->text().compare("Find In Documentation")==0)
				breakit=true;
		}
	command=QString("cat $(find '%1/actions' -iname '*.info'|sort -V)").arg(QFileInfo(this->plugPath).absolutePath());
	results=this->runPipeAndCapture(command);
	reslist=results.split("\n",Qt::SkipEmptyParts);

	for(int j=0;j<reslist.size();j+=2)
		{
			QAction	*menuitem=new QAction(reslist.at(j));
			this->apiMenu->addAction(menuitem);
			QObject::connect(menuitem,&QAction::triggered,[this,j,reslist]()
				{
					this->runSearch(reslist.at(j+1));
				});
		}
}

void DocumentationPlugin::unloadPlug(void)
{
	delete this->apiMenu;
}

void DocumentationPlugin::plugAbout(void)
{
	QMessageBox	msgBox;
	QFileInfo	fileinfo(this->plugPath);
	QString		txt="Search API Docs Plugin\n\n©K.D.Hedger 2024\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";

	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("Plugin About");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.setStandardButtons(QMessageBox::Help|QMessageBox::Close);
	int ret=msgBox.exec();
	switch(ret)
		{
			case QMessageBox::Close:
				break;
			case QMessageBox::Help:
				{
					QStringList args;
					args<<"-k";
					args<<QString("%1").arg(this->mainKKEditClass->sessionID);
					args<<"-c"<<"openindocview";
					args<<"-d"<<"file:///"+fileinfo.canonicalPath()+"/docs/help.html";
					QProcess::startDetached("kkeditqtmsg",args);
					this->mainKKEditClass->pluginPrefsWindow->hide();
				}
				break;
		}}

void DocumentationPlugin::plugSettings(void)
{
	qDebug()<<"void DocumentationPlugin::plugSettings(void)";
}

void DocumentationPlugin::plugRun(plugData *data)
{
//settings and about handled seperatly
	switch(data->what)
		{
			case DOSETSENSITVE:
				{
					bool hasselection=false;
					this->data=data;
					this->doc=data->doc;
					if(data->doc!=NULL)
						hasselection=data->doc->textCursor().hasSelection();
					this->apiMenu->setEnabled(hasselection);
				}
				break;
			case DOSWITCHPAGE:
				{
					this->data=data;
					this->doc=data->doc;
					break;
				}

			case DOCONTEXTMENU:
				{
					bool breakit=false;

					foreach(QAction *action,data->menu->actions())
						{
							if(breakit==true)
								{
									data->menu->insertMenu(action,this->apiMenu);
									break;
								}
							if(action->text().compare("Find In Documentation")==0)
								breakit=true;
						}
				}
				break;
		}
}

unsigned int DocumentationPlugin::plugWants(void)
{
	return(DOCONTEXTMENU|DOABOUT|DOSWITCHPAGE|DOSETSENSITVE);
}
