/*
 *
 * ©K. D. Hedger. Sat  5 Mar 17:05:39 GMT 2022 keithdhedger@gmail.com

 * This file (SaveHistory.cpp) is part of SaveHistory.

 * SaveHistory is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * SaveHistory is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with SaveHistory.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SaveHistory.h"

void SaveHistory::initPlug(KKEditClass *kk,QString pathtoplug)//TODO//init more data
{
	QIcon	itemicon(QIcon::fromTheme("edit-clear"));
	QIcon	itemicon2(QIcon::fromTheme("document-open"));
	QIcon	itemicon3(QIcon::fromTheme("document-revert"));

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;
	this->saveHistoryMenuitem=new QAction("Clear Document History");
	this->saveHistoryMenuitem->setIcon(itemicon);
	this->openHistoryMenuitem=new QAction("Open Document History");
	this->openHistoryMenuitem->setIcon(itemicon2);
	this->openHistoryListMenuitem=new QMenu("History List");
	this->openHistoryListMenuitem->setIcon(itemicon3);
	this->clearMenuCon=QObject::connect(this->saveHistoryMenuitem,&QAction::triggered,[this]()
		{
			this->clearHistory();
		});
	this->openMenuCon=QObject::connect(this->openHistoryMenuitem,&QAction::triggered,[this]()
		{
			this->openHistoryFolder();
		});
}

void SaveHistory::plugRun(plugData *data)
{
	if(data==NULL)
		return;

	this->data=data;

	if(data->what==DOTABPOPUP)
		{
			data->menu->addAction(this->openHistoryMenuitem);
			data->menu->addAction(this->saveHistoryMenuitem);
			data->menu->addMenu(this->openHistoryListMenuitem);
			this->homeDataFolder=data->userStrData1;
			this->docDir=data->userStrData2;
			this->docName=data->userStrData3;
			this->showHistoryList();
		}

	if(data->what==DOSAVE)
		{
			QDir			todir;
			QDateTime	dt(QDateTime::currentDateTime());

			todir.mkpath(data->userStrData1+"/SaveFilesHistory"+data->userStrData2+"/"+data->userStrData3);
			QStringList args;
			args<<data->userStrData2+"/"+data->userStrData3;
			args<<data->userStrData1+"/SaveFilesHistory"+data->userStrData2+"/"+data->userStrData3+"/"+data->userStrData3+"-"+dt.toString("yyyy.MM.dd-hh.mm.ss.z");
			QProcess::execute("cp",args);
		}
}

void SaveHistory::showHistoryList(void)
{
	QAction		*menuitem;
	QDir			dir(this->homeDataFolder+"/SaveFilesHistory"+this->docDir+"/"+this->docName);
	QStringList	flist=dir.entryList(QDir::Files);

	this->openHistoryListMenuitem->clear();
	for(int k=0;k<flist.count();k++)
		{
			menuitem=new QAction(flist.at(k),this->openHistoryListMenuitem);
			this->openHistoryListMenuitem->addAction(menuitem);
			QObject::connect(menuitem,&QAction::triggered,[this,menuitem,k,dir]()
				{
					QStringList args;

					args<<"-k"<<QString("%1").arg(this->mainKKEditClass->sessionID);
					args<<"-c"<<"openfile";
					args<<"-d"<<QString("%2/%3").arg(dir.canonicalPath()).arg(menuitem->text());
					QProcess::execute("kkeditqtmsg",args);
				});
		}
}

void SaveHistory::clearHistory(void)
{
	QDir			todir;
	QStringList	args;

	args<<"-rf";
	args<<this->homeDataFolder+"/SaveFilesHistory"+this->docDir+"/"+this->docName;
	QProcess::execute("rm",args);
	todir.rmpath(this->homeDataFolder+"/SaveFilesHistory"+this->docDir);
}

void SaveHistory::openHistoryFolder(void)
{
	QStringList args;

	args<<this->homeDataFolder+"/SaveFilesHistory"+this->docDir+"/"+this->docName;
	QProcess::startDetached("xdg-open",args);
}

void SaveHistory::plugAbout(void)
{
	QMessageBox msgBox;
	QFileInfo	fileinfo(this->plugPath);
	QString		txt="Save History Plugin\n\n©K.D.Hedger 2022\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";

	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("About Save History");
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
					args<<"-d"<<fileinfo.canonicalPath()+"/docs/help.html";
					QProcess::startDetached("kkeditqtmsg",args);
					this->mainKKEditClass->pluginPrefsWindow->hide();
				}
				break;
		}
}

unsigned int SaveHistory::plugWants(void)
{
	return(DOTABPOPUP|DOSAVE|DOABOUT|DOPOSTLOAD);
}

void SaveHistory::unloadPlug(void)
{
	QObject::disconnect(this->clearMenuCon);
	QObject::disconnect(this->openMenuCon);
	delete this->saveHistoryMenuitem;
	delete this->openHistoryMenuitem;
	delete this->openHistoryListMenuitem;
}
