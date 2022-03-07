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
	QObject::connect(this->openUriMenuitem,&QAction::triggered,[this]()
		{
			this->openUri();
		});
}

void OpenUri::openUri(void)
{
	QString command=QString("xdg-open %1 &").arg(this->selection);
	system(command.toStdString().c_str());
}

void OpenUri::plugAddToContextMenu(QMenu *menu,DocumentClass *document)
{
	QTextCursor	tc=document->textCursor();
	if(tc.hasSelection()==true)
		{
			this->selection=tc.selectedText().trimmed();
			menu->addAction(this->openUriMenuitem);
		}
}

void OpenUri::unloadPlug(void)
{
	DEBUGSTR("void OpenUri::unloadPlug(void)")
}

void OpenUri::plugAbout(void)
{
	DEBUGSTR("void OpenUri::plugAbout(void)")
}

void OpenUri::plugSettings(void)
{
	DEBUGSTR("void OpenUri::plugSettings(void)")
}

void OpenUri::plugRun(unsigned int)
{
	DEBUGSTR("void OpenUri::plugRun(whatIWant)")
}

unsigned int OpenUri::plugWants(void)
{
	DEBUGSTR("whatIWant OpenUri::plugWants(void)")
	return(DOCONTEXTMENU|DOABOUT);
}
