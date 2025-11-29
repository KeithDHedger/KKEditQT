/*
 *
 * ©K. D. Hedger. Sun 13 Feb 13:33:15 GMT 2022 keithdhedger@gmail.com

 * This file (QT_recentMenu.cpp) is part of KKEditQT.

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
#include "QT_recentMenu.h"
#include "runExternalProc.h"

RecentMenuClass::~RecentMenuClass()
{
	runExternalProcClass	rp;
	std::string			com="";

	com=QString("tail -n%1 %2 | awk '!a[$0]++'").arg(this->maxFiles).arg(this->recentFileList).toStdString();
	rp.runExternalCommands(com,false,QString(this->recentFileList+".bak").toStdString());
	com=QString("mv %1.bak %2").arg(this->recentFileList).arg(this->recentFileList).toStdString();
	rp.runExternalCommands(com,false);
	delete this->recentMenu;
}

RecentMenuClass::RecentMenuClass(KKEditClass *kk)
{
	QIcon	itemicon=QIcon::fromTheme("document-open");
	this->mainKKEditClass=kk;
	this->recentMenu=new QMenu("Open Recent");
	
	QObject::connect(this->recentMenu,&QMenu::triggered,[this](QAction *act)
		{
			this->menuClicked(act);
		});

	this->recentMenu->setIcon(itemicon);

	this->mainKKEditClass=kk;
	this->recentFileList=QString("%1/recentfiles").arg(kk->homeDataFolder);
}

void RecentMenuClass::addFilePath(QString path)
{
	QFile			file;
	bool				retval;
	QList<QAction*>	acts=this->recentMenu->actions(); 

	for(int j=0;j<acts.count();j++)
		{
			if(acts.at(j)->objectName().compare(path)==0)
				return;
		}

	file.setFileName(this->recentFileList);
	retval=file.open(QIODevice::Text | QIODevice::Append);
	if(retval==true)
		{
			QTextStream(&file) << path << Qt::endl;
			file.close();
			this->updateRecents();
		}
}

void RecentMenuClass::updateRecents(void)
{
	QFile	file;
	bool		retval;
	int		diff;
	int		cnt=0;
	QAction	*newact=NULL;

	this->recentMenu->clear();

	file.setFileName(this->recentFileList);
	retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
	if(retval==true)
		{
			QTextStream	in(&file);
			QString		line;
			while(in.atEnd()==false)
				{
					line=in.readLine().trimmed();
					this->recentMenu->addAction(line);
				}
			file.close();
		}

	QList<QAction*>	acts=this->recentMenu->actions(); 
	if(acts.count()>this->maxFiles)
		{
			diff=acts.count()-this->maxFiles;
			for(int j=0;j<diff;j++)
				this->recentMenu->removeAction(acts.at(cnt++));
		}
}

void RecentMenuClass::menuClicked(QAction *action)
{
//	this->mainKKEditClass->openFile(action->objectName());
	this->mainKKEditClass->openFile(action->text());
}


