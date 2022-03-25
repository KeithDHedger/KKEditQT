/*
 *
 * ©K. D. Hedger. Sat  5 Mar 16:00:46 GMT 2022 keithdhedger@gmail.com

 * This file (kkeditqtPluginExample.cpp) is part of kkeditqtPluginExample.

 * kkeditqtPluginExample is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * kkeditqtPluginExample is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with kkeditqtPluginExample.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kkeditqtPluginExample.h"

void kkeditQTexamplePlug::initPlug(KKEditClass *kk,QString pathtoplug)
{
	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

	this->exampleMenuitem=new QAction("Example plugin menu");
	this->exampleMenuitem->setObjectName("exampleplug");
	this->exampleMenuitem->setIcon(QIcon("/usr/share/KKEditQT/pixmaps/KKEditQTPlugMenu.png"));
	this->mainKKEditClass->pluginMenu->addAction(this->exampleMenuitem);

	QObject::connect(this->exampleMenuitem,&QAction::triggered,[this]()
		{
			this->mainKKEditClass->toolsOPText->setPlainText("Welcome to the example plugin!\n");
			this->mainKKEditClass->toolOutputWindow->show();
		});
}

void kkeditQTexamplePlug::unloadPlug(void)
{
	this->mainKKEditClass->pluginMenu->removeAction(this->exampleMenuitem);
	delete this->exampleMenuitem;
}

void kkeditQTexamplePlug::plugAbout(void)
{
	QMessageBox msgBox;

	QString txt="KKEditQT Example Plugin\n\n©K.D.Hedger 2022\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";
	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("Plugin About");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.exec();
}

void kkeditQTexamplePlug::plugSettings(void)
{
	DEBUGSTR("void kkeditQTexamplePlug::plugSettings(void)")
}

void kkeditQTexamplePlug::plugRun(plugData *data)
{
//	DEBUGSTR("void kkeditQTexamplePlug::plugRun(whatIWant)")
//settings and about handled seperatly
	switch(data->what)
		{
			case DOSAVE:
				qDebug() << "void kkeditQTexamplePlug::plugRun(plugData *data) > DOSAVE" << "\nNot using this on " << this->mainKKEditClass->mainNotebook->tabToolTip(data->tabNumber);
				break;
			case DOLOAD:
				qDebug() << "void kkeditQTexamplePlug::plugRun(plugData *data) > DOLOAD" << "\nNot using this on " << this->mainKKEditClass->mainNotebook->tabToolTip(data->tabNumber);
				break;
			case DOCLOSE:
				qDebug() << "void kkeditQTexamplePlug::plugRun(plugData *data) > DOCLOSE" << "\nNot using this on " << this->mainKKEditClass->mainNotebook->tabToolTip(data->tabNumber) << "\nDOCLOSE is called BEFORE closing tab";
				break;
			case DORESSESSION:
				qDebug() << "void kkeditQTexamplePlug::plugRun(plugData *data) > DORESSESSION" << "\nNot using this on session " << data->userIntData1 << " named " << data->userStrData1;
				break;
			case DOSAVESESSION:
				qDebug() << "void kkeditQTexamplePlug::plugRun(plugData *data) > DOSAVESESSION" << "\nNot using this on session " << data->userIntData1 << " named " << data->userStrData1;
				break;
			case DOCONTEXTMENU:
				qDebug() << "void kkeditQTexamplePlug::plugRun(plugData *data) > DOCONTEXTMENU" << "\nNot using this on " << this->mainKKEditClass->mainNotebook->tabToolTip(data->tabNumber);
				break;
			case DOTABPOPUP:
				qDebug() << "void kkeditQTexamplePlug::plugRun(plugData *data) > DOTABPOPUP" << "\nNot using this on tab named" << this->mainKKEditClass->mainNotebook->tabText(data->tabNumber);
				break;
			case DOSETSENSITVE:
				qDebug() << "void kkeditQTexamplePlug::plugRun(plugData *data) > DOSETSENSITVE" << "\nNot using this";
				break;
			case DOSWITCHPAGE:
				qDebug() << "void kkeditQTexamplePlug::plugRun(plugData *data) > DOSWITCHPAGE" << "\nNot using this index " << data->userIntData1 << " on " << this->mainKKEditClass->mainNotebook->tabToolTip(data->tabNumber);
				break;
			case DONEWDOCUMENT:
				qDebug() << "void kkeditQTexamplePlug::plugRun(plugData *data) > DONEWDOCUMENT" << "\nNot using this on " << this->mainKKEditClass->mainNotebook->tabToolTip(data->tabNumber);
				break;
		}
}

unsigned int kkeditQTexamplePlug::plugWants(void)
{
	return(DOSAVE|DOLOAD|DOCLOSE|DORESSESSION|DOSAVESESSION|DOCONTEXTMENU|DOTABPOPUP|DOSETSENSITVE|DOSWITCHPAGE|DONEWDOCUMENT|DOABOUT);
}
