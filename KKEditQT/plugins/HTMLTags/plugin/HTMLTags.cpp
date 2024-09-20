/*
 *
 * ©K. D. Hedger. Wed 18 Sep 12:56:13 BST 2024 keithdhedger@gmail.com

 * This file (HTMLTags.cpp) is part of HTMLTags.

 * HTMLTags is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * HTMLTags is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with HTMLTags.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "HTMLTags.h"

QString HTMLTags::runPipeAndCapture(QString command)
{
	QString	dump="";
	FILE		*fp=NULL;
	char		line[1024];

	fp=popen(command.toStdString().c_str(), "r");
	if(fp!=NULL)
		{
			while(fgets(line,1024,fp))
				{
					dump+=line;
				}
			pclose(fp);
		}
	return(dump);
}

void HTMLTags::initPlug(KKEditClass *kk,QString pathtoplug)
{
	QAction		*tagaction;
	QString		results;
	QStringList	taglist;
	QString		command;

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

	command=QString("find '%1/actions' -mindepth 1 -iname '*' -exec basename '{}' \\; |sort -V").arg(QFileInfo(this->plugPath).absolutePath());
	results=this->runPipeAndCapture(command);

	taglist=results.split("\n");
	QString str=taglist.at(0);
	str.remove(0,5);
	this->htmlContextMenu=new QMenu(str);
	this->htmlContextMenu->addSeparator();

	for(int j=1;j<taglist.size()-1;j++)
		{
			QString str=taglist.at(j);
			str.remove(0,5);
			if(str.compare("line")==0)
				{
					this->htmlContextMenu->addSeparator();
					continue;
				}
			tagaction=new  QAction(str);
			this->htmlContextMenu->addAction(tagaction);
			QObject::connect(tagaction,&QAction::triggered,[this,taglist,j]()
				{
					QProcess		script;
					QString		res;
					script.start(QString("%1/actions/%2").arg(QFileInfo(this->plugPath).absolutePath()).arg(taglist.at(j)),QStringList() << this->document->filePath<<this->selection);
					if(!script.waitForStarted())
        					return ;
        				if(!script.waitForFinished())
				         return;
					res=script.readAll();
					if(res.isEmpty()==false)
						{
							this->tc.removeSelectedText();
							this->tc.insertText(res);
						}
				});
			//qDebug()<<j<<taglist.at(j);
		}
}

void HTMLTags::plugRun(plugData *data)
{
	bool breakit=false;

	this->document=data->doc;
	this->tc=data->doc->textCursor();
	this->selection=this->tc.selectedText();

	foreach(QAction *action,data->menu->actions())
		{
			if(breakit==true)
				{
					data->menu->insertSeparator(action);
					data->menu->insertMenu(action,this->htmlContextMenu);
					break;
				}
			if(((this->tc.hasSelection()==true) && (action->text().compare("Go To Definition")==0)) )
				{
					breakit=true;
					continue;
				}
			if(((this->tc.hasSelection()==false) && (action->text().compare("Select All")==0)) )
				{
					breakit=true;
					continue;
				}
		}
}

void HTMLTags::plugAbout(void)
{
	QMessageBox msgBox;

	QString txt="HTML Tag Plugin\n\n©K.D.Hedger 2022\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";
	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("Plugin About");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.exec();
}

unsigned int HTMLTags::plugWants(void)
{
	return(DOCONTEXTMENU|DOABOUT);
}
