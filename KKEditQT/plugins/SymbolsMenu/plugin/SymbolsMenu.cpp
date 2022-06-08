/*
 *
 * ©K. D. Hedger. Wed  8 Jun 12:08:50 BST 2022 keithdhedger@gmail.com

 * This file (SymbolsMenu.cpp) is part of SymbolsMenu.

 * SymbolsMenu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * SymbolsMenu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with SymbolsMenu.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SymbolsMenu.h"

void SymbolsMenuPlug::initPlug(KKEditClass *kk,QString pathtoplug)
{
	QAction	*symact;
	QMenu	*submenu;
	QString	menustring;
	int		cnt=0;

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

	this->symbolMenu=new QMenu("Symbols");
	this->mainKKEditClass->pluginMenu->addMenu(symbolMenu);

	while(this->AandP[cnt]!=NULL)
		{
			menustring=this->AandP[cnt];
			submenu=new QMenu(menustring.left(menustring.indexOf("<<--")));
			//submenu=new QMenu(QString("☻☺☹😀😁😂😃😄😅😆😇😈😉😊😋😌😍😎😏😐😑😒😓😔😕😖😗😘😙😚"));
			
			this->symbolMenu->addMenu(submenu);
			for(int j=menustring.indexOf("<<--")+4;j<menustring.length();j++)
				{
					symact=new  QAction(QString(menustring.at(j)));
					symact->setObjectName(QString(menustring.at(j)));
					submenu->addAction(symact);
					QObject::connect(symact,&QAction::triggered,[this,symact]()
						{
							this->clipboard->setText(symact->objectName());
						});
				}
			cnt++;
		}
}

void SymbolsMenuPlug::unloadPlug(void)
{
	QObject::disconnect(this->symbolCon);
	delete this->symbolMenu;
}

void SymbolsMenuPlug::plugAbout(void)
{
	QMessageBox msgBox;

	QString txt="KKEditQT Symbols Menu Plugin\n\n©K.D.Hedger 2022\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";
	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("Plugin About");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.exec();
}

void SymbolsMenuPlug::plugSettings(void)
{
	//TODO//
}

unsigned int SymbolsMenuPlug::plugWants(void)
{
	return(DOABOUT);
}
