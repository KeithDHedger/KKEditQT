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

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;
	this->saveHistoryMenuitem=new QAction("Clear Document History");
	this->saveHistoryMenuitem->setIcon(itemicon);
	this->openHistoryMenuitem=new QAction("Open Document History");
	this->openHistoryMenuitem->setIcon(itemicon2);
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
	if(data->what==DOTABPOPUP)
		{
			data->menu->addAction(this->saveHistoryMenuitem);
			data->menu->addAction(this->openHistoryMenuitem);
			this->homeDataFolder=data->userStrData1;
			this->docDir=data->userStrData2;
			this->docName=data->userStrData3;
		}

	if(data->what==DOSAVE)
		{
			QDir			todir;
			QDateTime	dt(QDateTime::currentDateTime());
			QString		command;

			todir.mkpath(data->userStrData1+"/SaveFilesHistory"+data->userStrData2+"/"+data->userStrData3);
			command="cp '"+data->userStrData2+"/"+data->userStrData3+"' '"+data->userStrData1+"/SaveFilesHistory"+data->userStrData2+"/"+data->userStrData3+"/"+data->userStrData3+"-"+dt.toString("yyyy.MM.dd-hh.mm.ss.z")+"' 2>/dev/null";
			system(command.toStdString().c_str());
		}
}

void SaveHistory::clearHistory(void)
{
	QString	command;
	QDir		todir;

	command="rm -rf '"+this->homeDataFolder+"/SaveFilesHistory"+this->docDir+"/"+this->docName+"' 2>/dev/null";
	system(command.toStdString().c_str());
	todir.rmpath(this->homeDataFolder+"/SaveFilesHistory"+this->docDir);
}

void SaveHistory::openHistoryFolder(void)
{
	QString	command;

	command="xdg-open '"+this->homeDataFolder+"/SaveFilesHistory"+this->docDir+"/"+this->docName+"' 2>/dev/null";
	system(command.toStdString().c_str());
}

void SaveHistory::plugAbout(void)
{
	QMessageBox msgBox;

	QString txt="Save History Plugin\n\n©K.D.Hedger 2022\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";
	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("About Save History");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.exec();
}

unsigned int SaveHistory::plugWants(void)
{
	return(DOTABPOPUP|DOSAVE|DOABOUT);
}

void SaveHistory::unloadPlug(void)
{
	QObject::disconnect(this->clearMenuCon);
	QObject::disconnect(this->openMenuCon);
	delete this->saveHistoryMenuitem;
	delete this->openHistoryMenuitem;
}




