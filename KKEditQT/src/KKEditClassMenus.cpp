/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:38:44 GMT 2021 keithdhedger@gmail.com

 * This file (KKEditClassMenus.cpp) is part of KKEditQT.

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
#include "KKEditClass.h"

MenuItemClass* KKEditClass::makeMenuItemClass(int mainmenu,const QString name,const QKeySequence key,const QString iconname,const QString objectname,int userdata)
{
	MenuItemClass	*menuitem=new MenuItemClass(name);
	QIcon			itemicon=QIcon::fromTheme(iconname);

	menuitem->setMenuID(userdata);
	menuitem->setIcon(itemicon);
	if(key!=0)
		menuitem->setShortcut(key);
	menuitem->setObjectName(objectname);

	switch(mainmenu)
		{
			case FILEMENU:
				menuitem->setParent(this->fileMenu);
				this->fileMenu->addAction(menuitem);
				QObject::connect(menuitem,&QAction::triggered,[this,menuitem]()
					{
						this->doFileMenuItems(menuitem);
					});
				break;
			case EDITMENU:
				menuitem->setParent(this->editMenu);
				this->editMenu->addAction(menuitem);
				QObject::connect(menuitem,&QAction::triggered,[this,menuitem]()
					{
						this->doEditMenuItems(menuitem);
					});
				break;
			case VIEWMENU:
				menuitem->setParent(this->viewMenu);
				this->viewMenu->addAction(menuitem);
				QObject::connect(menuitem,&QAction::triggered,[this,menuitem]()
					{
						this->doViewMenuItems(menuitem);
					});
				break;
			case NAVMENU:
				menuitem->setParent(this->navMenu);
				this->navMenu->addAction(menuitem);
				QObject::connect(menuitem,&QAction::triggered,[this,menuitem]()
					{
						this->doNavMenuItems(menuitem);
					});
				break;
			case BOOKMARKSMENU:
				menuitem->setParent(this->bookMarkMenu);
				this->bookMarkMenu->addAction(menuitem);
				QObject::connect(menuitem,&QAction::triggered,[this,menuitem]()
					{
						this->doBookmarkMenuItems(menuitem);
					});
				break;
			case HELPMENU:
				menuitem->setParent(this->helpMenu);
				this->helpMenu->addAction(menuitem);
				QObject::connect(menuitem,&QAction::triggered,[this,menuitem]()
					{
						this->doHelpMenuItems(menuitem);
					});
				break;
			case TOOLSMENU:
				menuitem->setParent(this->toolsMenu);
				this->toolsMenu->addAction(menuitem);
				QObject::connect(menuitem,&QAction::triggered,[this,menuitem]()
					{
						this->doToolsMenuItems(menuitem);
					});
				break;
			case SAVESESSIONSMENU:
				menuitem->setParent(this->saveSessionsMenu);
				this->saveSessionsMenu->addAction(menuitem);
				QObject::connect(menuitem,&QAction::triggered,[this,menuitem]()
					{
						this->doSessionsMenuItems(menuitem);
					});

				break;
			case RESTORESESSIONSMENU:
				menuitem->setParent(this->restoreSessionsMenu);
				this->restoreSessionsMenu->addAction(menuitem);
				QObject::connect(menuitem,&QAction::triggered,[this,menuitem]()
					{
						this->doSessionsMenuItems(menuitem);
					});
				break;

			case NOMENU:
				menuitem->setParent(this);
				QObject::connect(menuitem,&QAction::triggered,[this,menuitem]()
					{
						this->doOddMenuItems(menuitem);
					});
				break;
		}

	return(menuitem);
}


