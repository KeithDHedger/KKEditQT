/*
 *
 * ©K. D. Hedger. Sat 26 Mar 15:12:06 GMT 2022 keithdhedger@gmail.com

 * This file (PLUGNAME.cpp) is part of PLUGNAME.

 * PLUGNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * PLUGNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with PLUGNAME.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PLUGNAME.h"

void PLUGNAMEPlug::initPlug(KKEditClass *kk,QString pathtoplug)
{
	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

//simple menu action example
	this->PLUGNAMEaction=new QAction("PLUGNAME");
	this->PLUGNAMEaction->setIcon(QIcon(QString("%1/PLUGNAME.png").arg(QFileInfo(pathtoplug).absolutePath())));
	this->mainKKEditClass->pluginMenu->addAction(this->PLUGNAMEaction);

	this->PLUGNAMEcon=QObject::connect(this->PLUGNAMEaction,&QAction::triggered,[this]()
		{
			qDebug()<<"Example action";
		});

//simple menu example
	//this->PLUGNAMEMenu=new QMenu("PLUGNAME");
	//this->PLUGNAMEMenu->setIcon(QIcon(QString("%1/PLUGNAME.png").arg(QFileInfo(pathtoplug).absolutePath())));
	//this->mainKKEditClass->pluginMenu->addMenu(PLUGNAMEMenu);	
}

void PLUGNAMEPlug::unloadPlug(void)
{
	QObject::disconnect(this->PLUGNAMEcon);
	delete this->PLUGNAMEaction;
	//delete this->PLUGNAMEMenu;
}

void PLUGNAMEPlug::plugAbout(void)
{
	QMessageBox msgBox;

	QString txt="KKEditQT PLUGNAME Plugin\n\n©K.D.Hedger 2022\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";
	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("Plugin About");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.exec();
}

void PLUGNAMEPlug::plugSettings(void)
{
}

unsigned int PLUGNAMEPlug::plugWants(void)
{
	return(DOABOUT|DONEWDOCUMENT|DOCLOSE);
}

void PLUGNAMEPlug::plugRun(plugData *data)
{
	qDebug()<<"plugRun called";
	qDebug()<<"Name: "<<data->plugName;
	qDebug()<<"Version: "<<data->plugVersion;
	qDebug()<<"What to do: "<<data->what;
}
