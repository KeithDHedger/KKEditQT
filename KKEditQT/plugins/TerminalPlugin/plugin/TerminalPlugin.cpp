/*
 *
 * ©K. D. Hedger. Sat 26 Mar 15:12:06 GMT 2022 keithdhedger@gmail.com

 * This file (TerminalPlugin.cpp) is part of TerminalPlugin.

 * TerminalPlugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * TerminalPlugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with TerminalPlugin.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TerminalPlugin.h"

enum {FOCUSTERM,VISCHANGED,CDTOFOLDER,NEWTERM};

void TerminalPluginPlug::addTerminal(void)
{
	QSettings		plugprefs("KDHedger","TerminalPlugin");
	QString			dwss="QDockWidget::title {background: grey;padding-left: 0px;padding-top: 0px;padding-bottom: 0px;}\nQDockWidget {font-size: 10pt;}";
	QDockWidget		*newdw;
	QString			s;
	TerminalWidget	*newconsole;
	termDataStruct	ts;
	int				whome;

	if(this->currentHeight==-1)
		{
			this->currentHeight=plugprefs.value("geom").toRect().height();

			if(this->currentHeight==0)
				this->currentHeight=200;
		}
	else
		{
			for(int j=0;j<this->terminals.size();j++)
				{
					if(this->terminals.at(j).dockWidget->isVisible()==true)
						{
							this->currentHeight=this->terminals.at(j).dockWidget->height();
							break;
						}
				}
		}

	newdw=new QDockWidget(this->mainKKEditClass->mainWindow);
	newdw->resize(this->mainKKEditClass->mainWindow->geometry().width(),this->currentHeight);
	newdw->setStyleSheet(dwss);
	newdw->setFloating(false);
	newdw->setContextMenuPolicy(Qt::CustomContextMenu);
	newdw->setVisible(false);

    newconsole=new TerminalWidget(QString("%1%2").arg(this->baseName).arg(this->namenum++),newdw);
	newconsole->backCol=plugprefs.value("backcolour","white").toString();
	newconsole->foreCol=plugprefs.value("forecolour","black").toString();
	newconsole->plugPath=QFileInfo(this->plugPath).absolutePath();
	newconsole->startXTerm();
	newdw->setWidget(newconsole);
	this->mainKKEditClass->mainWindow->addDockWidget(Qt::BottomDockWidgetArea,newdw);

	whome=this->terminals.size();
	QObject::connect(newdw,&QDockWidget::visibilityChanged,[this,whome,newdw](bool vis)
		{
			if(vis==true)
				{
					this->terminals.at(whome).console->setFocus();
					this->currentTerminal=whome;
				}
			else
				this->currentHeight=newdw->size().height();
		});

	QObject::connect(newdw,&QDockWidget::customContextMenuRequested,[this,newdw,whome](const QPoint pos)
		{
			QPoint	globalpos=newdw->mapToGlobal(pos);
			QMenu	cmenu;
			QAction	copyitem(QIcon::fromTheme("edit-copy"),"Copy");
			QAction	pasteitem(QIcon::fromTheme("edit-paste"),"Paste");
			QAction	pasteinitem(QIcon::fromTheme("edit-paste"),"Paste In Quotes");
			QAction	cdtofolderitem("CD To DocFolder");
			QAction	openfilehere("Open File From Here");

			copyitem.setData(QVariant(101));
			cmenu.addAction(&copyitem);
			pasteitem.setData(QVariant(102));
			cmenu.addAction(&pasteitem);
			pasteinitem.setData(QVariant(103));
			cmenu.addAction(&pasteinitem);
			cdtofolderitem.setData(QVariant(104));
			cmenu.addAction(&cdtofolderitem);
			openfilehere.setData(QVariant(105));
			cmenu.addAction(&openfilehere);

			QAction	*selecteditem=cmenu.exec(globalpos);
			if(selecteditem!=NULL)
				{
					switch(selecteditem->data().toInt())
						{
							case 101://copy
								{
									QString s=QString("xvkbd -window %1 -text \"\\C\\SC\"").arg(this->terminals.at(whome).console->xtermWinID);
									system(s.toStdString().c_str());
								}
							break;
							case 102://paste
								{
									QString s=QString("xvkbd -window %1 -text \"\\C\\SV\"").arg(this->terminals.at(whome).console->xtermWinID);
									system(s.toStdString().c_str());
								}
								break;
							case 103://paste in "
								{
									QString s=QString("xvkbd -window %1 -text \"\\\"\\C\\SV\\\"\"").arg(this->terminals.at(whome).console->xtermWinID);
									system(s.toStdString().c_str());
								}
								break;
							case 104://cd to folder
								{
									QString	ht=qApp->clipboard()->text();
									qApp->clipboard()->setText(QString("cd '%1'").arg(this->folderPath));

									QString s=QString("xvkbd -window %1 -no-jump-pointer -xsendevent -text \"\\C\\SV\"").arg(this->terminals.at(whome).console->xtermWinID);
									system(s.toStdString().c_str());
									qApp->processEvents();
									s=QString("xdotool key -window %1 Return").arg(this->terminals.at(whome).console->xtermWinID);
									system(s.toStdString().c_str());
									qApp->processEvents();
									qApp->clipboard()->setText(ht);
								}
								break;
							case 105://open here
								{
									QString	filepath="";
									QString	cwd="";
									QString	ht=qApp->clipboard()->text();
									int		cnt=0;

									QString d=QString("echo 'echo $(pwd)|xclip -selection clipboard -i'|xclip -selection clipboard -i;xvkbd -window %1 -no-jump-pointer -xsendevent -text '\\C\\SV' >/dev/null 2>/dev/null;echo ''|xclip -selection clipboard -i;xvkbd -window %1 -no-jump-pointer -xsendevent -text '\\n' >/dev/null 2>/dev/null").arg(this->terminals.at(whome).console->xtermWinID);
									system(d.toStdString().c_str());
									cwd=qApp->clipboard()->text(QClipboard::Clipboard).trimmed();
									while(cwd.isEmpty()==true && cnt<50)
										{
											cwd=qApp->clipboard()->text(QClipboard::Clipboard).trimmed();
											cnt++;
										}
									if(cnt>=50)
										{
											qApp->clipboard()->setText(ht);
											return;
										}
									filepath=QFileDialog::getOpenFileName(nullptr,"Open File",cwd,"",nullptr,QFileDialog::HideNameFilterDetails);
									if(filepath.isEmpty()==false)
										{
											QString comm=QString("kkeditqtmsg -k %1 -c openfile -d '%2'").arg(this->mainKKEditClass->sessionID).arg(filepath);
											system(comm.toStdString().c_str());
										}
									qApp->clipboard()->setText(ht);
								}
								break;
						}
				}
		});

	whome=this->terminals.size();
	ts.console=newconsole;
	ts.dockWidget=newdw;

	ts.toggleTerm=ts.dockWidget->toggleViewAction();
	ts.toggleTerm->setText("Toggle Terminal");
	QObject::connect(ts.toggleTerm,&QAction::triggered,[this,whome]() { this->doMenuItem(VISCHANGED,whome); });

	this->dw=newdw;
	this->terminals.push_back(ts);
	this->terminals.at(whome).dockWidget->setWindowTitle(QString("Terminal %1").arg(whome+1));

	ts.toggleTerm->setText(QString("Terminal %1").arg(whome+1));
	this->terminalsMenu->addAction(ts.toggleTerm);

	if((this->terminals.size()>1) && (plugprefs.value("usetabs").toBool()==true))
		this->mainKKEditClass->mainWindow->tabifyDockWidget(this->terminals.at(this->terminals.size()-2).dockWidget,this->terminals.at(this->terminals.size()-1).dockWidget);

	this->mainKKEditClass->mainWindow->resizeDocks({newdw},{this->mainKKEditClass->mainWindow->geometry().width()}, Qt::Horizontal);
	newdw->setVisible(true);
	while(newconsole->getXTermData()==false);
	this->mainKKEditClass->mainWindow->resizeDocks({newdw},{this->currentHeight+1}, Qt::Vertical);
	qApp->processEvents();
	this->mainKKEditClass->mainWindow->resizeDocks({newdw},{this->currentHeight}, Qt::Vertical);
	qApp->processEvents();
}

void TerminalPluginPlug::showDocs(void)
{
	if(this->data!=NULL)
		{
			QString		comm;
			QDir::setCurrent(this->folderPath);
			comm=QString("kkeditqtmsg -k %1 -c openindocview -d %2").arg(this->mainKKEditClass->sessionID).arg(QString("'%1/%2'").arg(this->folderPath).arg("html/index.html"));
			system(comm.toStdString().c_str());		
		}
}

void TerminalPluginPlug::initPlug(KKEditClass *kk,QString pathtoplug)
{
	QSettings	plugprefs("KDHedger","TerminalPlugin");

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

	srand(static_cast<unsigned int>(time(0)));
	const char alphanum[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for(int i=0;i<15;++i)
		this->baseName+=alphanum[rand()%(sizeof(alphanum)-1)];

	this->TerminalPluginMenu=new QMenu("Terminals",this->mainKKEditClass->pluginMenu);
	this->TerminalPluginMenu->setIcon(QIcon(QString("%1/TerminalPlugin.png").arg(QFileInfo(pathtoplug).absolutePath())));
	this->mainKKEditClass->pluginMenu->addMenu(TerminalPluginMenu);

	this->openOnStart=plugprefs.value("openonstart").toBool();
	this->terminalsMenu=new QMenu("Terminals",this->TerminalPluginMenu);

	this->newAct=new QAction("New Terminal ...",this->TerminalPluginMenu);
	this->TerminalPluginMenu->addAction(this->newAct);
	QObject::connect(this->newAct,&QAction::triggered,[this]() { this->doMenuItem(NEWTERM,-1); });

	this->toggleTabsAct=new QAction("Open in Tabs ...",this->TerminalPluginMenu);
	if(plugprefs.value("usetabs").toBool()==true)
		this->toggleTabsAct->setText("Opening In Tabs ...");
	else
		this->toggleTabsAct->setText("Opening In Window ...");

	this->TerminalPluginMenu->addAction(this->toggleTabsAct);
	QObject::connect(this->toggleTabsAct,&QAction::triggered,[this]()
		{
			QSettings	plugprefs("KDHedger","TerminalPlugin");
			bool			what=plugprefs.value("usetabs").toBool();
			plugprefs.setValue("usetabs",!what);
			if(!what==true)
				this->toggleTabsAct->setText("Opening In Tabs ...");
			else
				this->toggleTabsAct->setText("Opening In Window ...");
		});

	this->TerminalPluginMenu->addMenu(this->terminalsMenu);
	if(this->openOnStart==true)
		this->addTerminal();
}

void TerminalPluginPlug::unloadPlug(void)
{
	for(int j=0;j<this->terminals.size();j++)
		{
			if(this->terminals.at(j).console->process->state()==QProcess::Running)
				{
					this->terminals.at(j).console->process->kill();
					this->terminals.at(j).console->process->waitForFinished();
				}
		}
	delete this->TerminalPluginMenu;
	for(int j=0;j<this->terminals.size();j++)
		delete this->terminals.at(j).dockWidget;
}

void TerminalPluginPlug::plugAbout(void)
{
	QMessageBox msgBox;
	QFileInfo	fileinfo(this->plugPath);
	QString txt="KKEditQT TerminalPlugin Plugin\n\n©K.D.Hedger 2024\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";
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
					args<<"-d"<<fileinfo.canonicalPath()+"/docs/help.html";
					QProcess::startDetached("kkeditqtmsg",args);
					this->mainKKEditClass->pluginPrefsWindow->hide();
				}
				break;
		}}

void TerminalPluginPlug::plugSettings(void)
{
	QString	prefsname="TerminalPlugin";
	prefsWidgetsClass	pw(prefsname);

	pw.startWindow("TerminalPlugin Settings");

//back col
	pw.addWidgetToWindow(pw.getPrefsColourWidget("Back Colour","white"));
	pw.addWidgetToWindow(pw.getPrefsColourWidget("Fore Colour","black"));
	pw.addWidgetToWindow(pw.getPrefsCheckWidget("Open On Start",false));
	pw.addWidgetToWindow(pw.getPrefsCheckWidget("Open In Tabs",false));

	pw.finishWindow(true);
}

unsigned int TerminalPluginPlug::plugWants(void)
{
	return(DOABOUT|DOSETTINGS|DOSWITCHPAGE|DOSHUTDOWN|DOPOSTLOAD);
}

void TerminalPluginPlug::plugRun(plugData *data)
{
	QSettings plugprefs("KDHedger","TerminalPlugin");

	if(data==NULL)
		return;

	this->data=data;

	if(data->what==DOSHUTDOWN)
		{	
			if(this->dw!=NULL)
				plugprefs.setValue("geom",this->dw->geometry());
		}
		
	if(data->what==DOSWITCHPAGE)
		{
			this->data=data;
			this->filePath=data->userStrData1;
			this->folderPath=data->userStrData3;
		}
}

void TerminalPluginPlug::doMenuItem(int what,int whome)
{
	QSettings plugprefs("KDHedger","TerminalPlugin");
	switch(what)
		{
			case VISCHANGED:
				this->terminals.at(whome).toggleTerm->setText(QString("Terminal %1").arg(whome+1));
				break;
			case NEWTERM:
				this->addTerminal();
				break;
		}
}

