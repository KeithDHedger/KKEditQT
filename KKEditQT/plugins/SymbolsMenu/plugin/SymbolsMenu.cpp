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

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

	this->symbolMenu=new QMenu("Symbols");
	this->mainKKEditClass->pluginMenu->addMenu(symbolMenu);

	QFileInfo	datafile(this->plugPath);
	QFile		thefile(QString("%1/indicator-chars.txt").arg(datafile.absolutePath()));
	bool			retval=false;
	retval=thefile.open(QIODevice::Text | QIODevice::ReadOnly);
	if(retval==true)
		{
			QString menustring;
			QTextStream	in(&thefile);
			while(in.atEnd()==false)
				{
					menustring=in.readLine();
					submenu=new QMenu(menustring.left(menustring.indexOf("<<--")));
					this->symbolMenu->addMenu(submenu);

					QTextBoundaryFinder bf2(QTextBoundaryFinder::Grapheme,menustring);
					bf2.setPosition(menustring.indexOf("<<--")+4);
					while(bf2.position()!=-1)
						{
							int	st=bf2.position();
							int	dne=bf2.toNextBoundary();
							if(dne!=-1)
								{
									symact=new  QAction(QString(menustring.mid(st,dne-st)));
									symact->setObjectName(QString(menustring.mid(st,dne-st)));
									submenu->addAction(symact);
									QObject::connect(symact,&QAction::triggered,[this,symact]()
										{
											this->clipboard->setText(symact->objectName());
										});
								}
						}
				}
			thefile.close();
		}
}

void SymbolsMenuPlug::unloadPlug(void)
{
	QObject::disconnect(this->symbolCon);
	delete this->symbolMenu;
}

void SymbolsMenuPlug::plugAbout(void)
{
	QMessageBox	msgBox;
	QFileInfo	fileinfo(this->plugPath);
	QString		txt="KKEditQT Symbols Menu Plugin\n\n©K.D.Hedger 2022\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";

	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("Plugin About");
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
					args<<"-d"<<"file:///"+fileinfo.canonicalPath()+"/docs/help.html";
					QProcess::startDetached("kkeditqtmsg",args);
				}
				break;
		}
}

void SymbolsMenuPlug::plugSettings(void)
{
	//TODO//
}

unsigned int SymbolsMenuPlug::plugWants(void)
{
	return(DOABOUT);
}
