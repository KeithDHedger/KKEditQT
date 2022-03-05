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
	this->mainKKEditClass->pluginMenu->addAction(this->exampleMenuitem);

	QObject::connect(this->exampleMenuitem,&QAction::triggered,[this]()
		{
			this->mainKKEditClass->toolsOPText->setPlainText("Welcome to the example plugin!\n");
			this->mainKKEditClass->toolOutputWindow->show();
		});
}

void kkeditQTexamplePlug::unloadPlug(void)
{
	DEBUGSTR("void kkeditQTexamplePlug::unloadPlug(void)")
}

void kkeditQTexamplePlug::plugAbout(void)
{
	DEBUGSTR("void kkeditQTexamplePlug::plugAbout(void)")
}

void kkeditQTexamplePlug::plugSettings(void)
{
	DEBUGSTR("void kkeditQTexamplePlug::plugSettings(void)")
}

void kkeditQTexamplePlug::plugRun(whatIWant)
{
	DEBUGSTR("void kkeditQTexamplePlug::plugRun(whatIWant)")
}

whatIWant kkeditQTexamplePlug::plugWants(void)
{
	DEBUGSTR("whatIWant kkeditQTexamplePlug::plugWants(void)")
	return(DONONE);
}
