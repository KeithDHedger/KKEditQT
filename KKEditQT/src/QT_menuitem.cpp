/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:37:00 GMT 2021 keithdhedger@gmail.com

 * This file (QT_menuitem.cpp) is part of KKEditQT.

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

#include "QT_menuitem.h"

MenuItemClass::~MenuItemClass()
{
}

MenuItemClass::MenuItemClass(const QString &text, QWidget *parent): QAction(parent)
{
	this->menuID=0;
	this->menuString=nullptr;
	this->setText(text);
}

void MenuItemClass::setMenuID(unsigned int id)
{
	this->menuID=id;
}

unsigned int MenuItemClass::getMenuID(void)
{
	return(this->menuID);
}

void MenuItemClass::setMenuString(QString str)
{
	this->menuString=str;
}

QString MenuItemClass::getMenuString(void)
{
	return(this->menuString);
}

void MenuItemClass::menuClickedGotoLine()
{
	DocumentClass	*document=qobject_cast<DocumentClass*>(this->mainKKEditClass->mainNotebook->currentWidget());
	this->mainKKEditClass->history->pushToBackList(document->getCurrentLineNumber(),document->getFilePath());
	this->mainKKEditClass->gotoLine(this->getMenuID());
}
