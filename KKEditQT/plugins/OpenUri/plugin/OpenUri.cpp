/*
 *
 * ©K. D. Hedger. Sat  5 Mar 17:05:39 GMT 2022 keithdhedger@gmail.com

 * This file (OpenUri.cpp) is part of OpenUri.

 * OpenUri is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * OpenUri is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with OpenUri.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "OpenUri.h"

void OpenUri::initPlug(KKEditClass *kk,QString pathtoplug)
{
	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;
	this->openUriMenuitem=new QAction("Open Selection");
	this->openUriMenuitem->setIcon(QIcon(QString("%1/OpenUri.png").arg(QFileInfo(pathtoplug).absolutePath())));
	QObject::connect(this->openUriMenuitem,&QAction::triggered,[this]()
		{
			this->openUri();
		});
}

void OpenUri::unloadPlug(void)
{
	delete this->openUriMenuitem;
}

void OpenUri::plugRun(plugData *data)
{
	if(data==NULL)
		return;

	this->data=data;

	if(data->what==DOCONTEXTMENU)
		{
			QTextCursor	tc=data->doc->textCursor();
			if(tc.hasSelection()==true)
				{
					this->selection=tc.selectedText().trimmed();
					data->menu->addAction(this->openUriMenuitem);
				}
		}
}

void OpenUri::openUri(void)
{
	QString command=QString("xdg-open %1 &").arg(this->selection);
	system(command.toStdString().c_str());
}

void OpenUri::plugAbout(void)
{
	QMessageBox msgBox;

	QString txt="Open URI Plugin\n\n©K.D.Hedger 2022\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";
	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("Plugin About");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.exec();
}

unsigned int OpenUri::plugWants(void)
{
	return(DOCONTEXTMENU|DOABOUT|DOPOSTLOAD);
}
