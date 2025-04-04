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
	QStringList		themenames=QTermWidget::availableColorSchemes();
	QString			dwss="QDockWidget::title {background: grey;padding-left: 0px;padding-top: 0px;padding-bottom: 0px;}\nQDockWidget {font-size: 10pt;}";
	QDockWidget		*newdw;
	QTermWidget		*newconsole;
	termDataStruct	ts;
	int				whome;

	themenames.sort();
	newconsole=new QTermWidget(0,mainKKEditClass->mainWindow);
	newconsole->setScrollBarPosition(QTermWidget::ScrollBarRight);
	newconsole->setColorScheme(themenames.at(this->cbnum));

	newdw=new QDockWidget(this->mainKKEditClass->mainWindow);
	newdw->setStyleSheet(dwss);
	newdw->setFloating(false);
	newdw->setContextMenuPolicy(Qt::CustomContextMenu);
	
	if((this->openOnStart==true) && (this->saveCurrentVis==true))
		{
			newdw->setVisible(plugprefs.value("currentstate").toBool());
			newdw->setFloating(plugprefs.value("floating").toBool());
		}
	else
		newdw->setVisible(this->openOnStart);

	newdw->setWidget(newconsole);

	this->mainKKEditClass->mainWindow->addDockWidget(Qt::BottomDockWidgetArea,newdw);
	whome=this->terminals.size();
	QObject::connect(newdw,&QDockWidget::visibilityChanged,[this,whome](bool vis)
		{
			if(vis==true)
				{
					this->terminals.at(whome).console->setFocus();
					this->currentTerminal=whome;
				}
		});

	QObject::connect(newdw,&QDockWidget::customContextMenuRequested,[this,newdw,whome](const QPoint pos)
		{
			QPoint	globalpos=newdw->mapToGlobal(pos);
			QMenu	cmenu;
			QAction	copyitem(QIcon::fromTheme("edit-copy"),"Copy");
			QAction	pasteitem(QIcon::fromTheme("edit-paste"),"Paste");
			QAction	pasteinitem(QIcon::fromTheme("edit-paste"),"Paste In Quotes");
			QAction	cdtofolderitem("CD To DocFolder");

			copyitem.setData(QVariant(101));
			cmenu.addAction(&copyitem);
			pasteitem.setData(QVariant(102));
			cmenu.addAction(&pasteitem);
			pasteinitem.setData(QVariant(103));
			cmenu.addAction(&pasteinitem);
			cdtofolderitem.setData(QVariant(104));
			cmenu.addAction(&cdtofolderitem);

			QAction	*selecteditem=cmenu.exec(globalpos);
			if(selecteditem!=NULL)
				{
					switch(selecteditem->data().toInt())
						{
							case 101:
							this->terminals.at(whome).console->copyClipboard();
							break;
							case 102:
								this->terminals.at(whome).console->pasteClipboard();
								break;
							case 103:
								this->terminals.at(whome).console->sendText(QString("'%1'").arg(QGuiApplication::clipboard()->text(QClipboard::Clipboard)));
								break;
							case 104:
								this->terminals.at(whome).dockWidget->activateWindow();
								this->terminals.at(whome).dockWidget->raise();
								this->terminals.at(whome).console->changeDir(this->folderPath);
								this->terminals.at(whome).console->setFocus();
								break;
						}
				}
		});

	if((this->terminals.size()>0) && (plugprefs.value("usetabs").toBool()==false))
		{
			QSize	sz(this->terminals.at(this->terminals.size()-1).dockWidget->size());
			newconsole->setSize(QSize(sz.width()/8,sz.height()));
			newdw->resize(sz);
		}

	newconsole->startShellProgram ();
	if(newdw->isFloating()==true)
		{
			if((plugprefs.value("geom").toRect().width()==0) || (plugprefs.value("geom").toRect().height()==0))
				newdw->setFloating(false);
			else
				newdw->setGeometry(plugprefs.value("geom").toRect());
		}

	whome=this->terminals.size();
	QObject::connect(newconsole,&QTermWidget::termGetFocus,[this,whome]()
		{
			this->currentTerminal=whome;
		});

	ts.console=newconsole;
	ts.dockWidget=newdw;

	ts.toggleTerm=ts.dockWidget->toggleViewAction();
	ts.toggleTerm->setText("Toggle Terminal");
	QObject::connect(ts.toggleTerm,&QAction::triggered,[this,whome]() { this->doMenuItem(VISCHANGED,whome); });

	this->dw=newdw;
	this->console=newconsole;
	this->terminals.push_back(ts);
	this->terminals.at(whome).dockWidget->setWindowTitle(QString("Terminal %1").arg(whome+1));

	QMenu *termmenu=new QMenu(QString("Terminal %1").arg(this->terminals.size()));
	QAction *act=new QAction("CD TO Doc Folder ...");
	QObject::connect(act,&QAction::triggered,[this,ts,whome]()
		{
			ts.console->setFocus();
			this->doMenuItem(CDTOFOLDER,whome);
		});
	termmenu->addAction(act);

	ts.toggleTerm->setText(QString("Terminal %1").arg(whome+1));
	termmenu->addAction(ts.toggleTerm);

	this->terminalsMenu->addMenu(termmenu);

	if((this->terminals.size()>1) && (plugprefs.value("usetabs").toBool()==true))
		this->mainKKEditClass->mainWindow->tabifyDockWidget(this->terminals.at(this->terminals.size()-2).dockWidget,this->terminals.at(this->terminals.size()-1).dockWidget);
}

void TerminalPluginPlug::initPlug(KKEditClass *kk,QString pathtoplug)
{
	QSettings	plugprefs("KDHedger","TerminalPlugin");

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

	this->cbnum=plugprefs.value("themenumber").toInt();
	this->TerminalPluginMenu=new QMenu("TerminalPlugin");
	this->TerminalPluginMenu->setIcon(QIcon(QString("%1/TerminalPlugin.png").arg(QFileInfo(pathtoplug).absolutePath())));
	this->mainKKEditClass->pluginMenu->addMenu(TerminalPluginMenu);

	this->openOnStart=plugprefs.value("openonstart").toBool();
	this->saveCurrentVis=plugprefs.value("savevis").toBool();

	this->terminalsMenu=new QMenu("Terminals");
	this->addTerminal();

	this->newAct=new QAction("New Terminal ...");
	this->TerminalPluginMenu->addAction(this->newAct);
	QObject::connect(this->newAct,&QAction::triggered,[this]() { this->doMenuItem(NEWTERM,-1); });

	this->toggleTabsAct=new QAction("Open in Tabs ...");
	if(plugprefs.value("usetabs").toBool()==true)
		this->toggleTabsAct->setText("Opening In Tabs ...");
	else
		this->toggleTabsAct->setText("Opening In Window ...");

	{
		QIcon	itemicon=QIcon::fromTheme("edit-copy");
		QAction	*act=new QAction(itemicon,"Copy");
		//act->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_C));
		act->setShortcut(QKeySequence(Qt::CTRL|Qt::SHIFT|Qt::Key_C));
		QObject::connect(act,&QAction::triggered,[this]()
			{
				this->terminals.at(this->currentTerminal).console->copyClipboard();
			});
		this->TerminalPluginMenu->addAction(act);
	}

	{
		QIcon	itemicon=QIcon::fromTheme("edit-paste");
		QAction	*act=new QAction(itemicon,"Paste");
		//act->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_V));
		act->setShortcut(QKeySequence(Qt::CTRL|Qt::SHIFT|Qt::Key_V));
		QObject::connect(act,&QAction::triggered,[this]()
			{
				this->terminals.at(this->currentTerminal).console->pasteClipboard();
			});
		this->TerminalPluginMenu->addAction(act);
	}

	{
		QIcon	itemicon=QIcon::fromTheme("edit-find");
		QAction	*act=new QAction(itemicon,"Find");
		QObject::connect(act,&QAction::triggered,[this]()
			{
				this->terminals.at(this->currentTerminal).console->toggleShowSearchBar();
			});
		this->TerminalPluginMenu->addAction(act);
	}

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
}

void TerminalPluginPlug::unloadPlug(void)
{
	delete this->TerminalPluginMenu;
	for(int j=0;j<this->terminals.size();j++)
		delete this->terminals.at(j).dockWidget;
}

void TerminalPluginPlug::plugAbout(void)
{
	QMessageBox msgBox;

	QString txt="KKEditQT TerminalPlugin Plugin\n\n©K.D.Hedger 2024\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";
	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("Plugin About");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.exec();
}

void TerminalPluginPlug::plugSettings(void)
{
	QDialog		settings(this->mainKKEditClass->pluginPrefsWindow);
	QComboBox	themebox;
	QVBoxLayout	*vlayout;
	QHBoxLayout	*hlayout;
	QLabel		*label;
	QPushButton	*btn;
	QStringList	themenames;
	QCheckBox	*openonstart;
	QCheckBox	*savevis;
	QCheckBox	*usetabs;
	QSettings	plugprefs("KDHedger","TerminalPlugin");

	themenames=QTermWidget::availableColorSchemes();
	themenames.sort();
	settings.setWindowTitle("TerminalPlugin Settings");

	themebox.addItems(themenames);
	themebox.setCurrentIndex(this->cbnum);

	vlayout=new QVBoxLayout(&settings);

	hlayout=new QHBoxLayout();
	label=new QLabel("Themes:");
	hlayout->addWidget(label);
	hlayout->addWidget(&themebox);
	vlayout->addLayout(hlayout);

	openonstart=new QCheckBox("Open On Start");
	openonstart->setChecked(this->openOnStart);
#ifdef _USEQT6_
	QObject::connect(openonstart,&QCheckBox::checkStateChanged,[this](Qt::CheckState state) { this->openOnStart=(bool)state; });
#else
	QObject::connect(openonstart,&QCheckBox::stateChanged,[this](bool what) { this->openOnStart=what; });
#endif
	vlayout->addWidget(openonstart);

	savevis=new QCheckBox("Save Current State");
	savevis->setChecked(this->saveCurrentVis);
#ifdef _USEQT6_
	QObject::connect(openonstart,&QCheckBox::checkStateChanged,[this](Qt::CheckState state) { this->saveCurrentVis=(bool)state; });
#else
	QObject::connect(savevis,&QCheckBox::stateChanged,[this](bool what) { this->saveCurrentVis=what; });
#endif
	vlayout->addWidget(savevis);

	usetabs=new QCheckBox("Open In Tabs");
	usetabs->setChecked(plugprefs.value("usetabs").toBool());
	vlayout->addWidget(usetabs);

	hlayout=new QHBoxLayout();
	btn=new QPushButton("Apply");
	QObject::connect(btn,&QPushButton::clicked,[&settings]() { settings.done(1); });
	hlayout->addWidget(btn);

	btn=new QPushButton("Cancel");
	QObject::connect(btn,&QPushButton::clicked,[&settings]() { settings.done(0); });
	hlayout->addWidget(btn);
	vlayout->addLayout(hlayout);

	settings.exec();

	if(settings.result()==1)
		{
			QSettings	plugprefs("KDHedger","TerminalPlugin");
			this->console->setColorScheme(themebox.currentText());
			this->cbnum=themebox.currentIndex();
			plugprefs.setValue("themenumber",this->cbnum);
			plugprefs.setValue("openonstart",this->openOnStart);
			plugprefs.setValue("savevis",this->saveCurrentVis);
			plugprefs.setValue("usetabs",usetabs->isChecked());
			if(usetabs->isChecked()==true)
				this->toggleTabsAct->setText("Opening In Tabs ...");
			else
				this->toggleTabsAct->setText("Opening In Window ...");
		}
}

unsigned int TerminalPluginPlug::plugWants(void)
{
	return(DOABOUT|DOSETTINGS|DOSWITCHPAGE|DOCONTEXTMENU|DOSHUTDOWN);
}

void TerminalPluginPlug::plugRun(plugData *data)
{
	QSettings plugprefs("KDHedger","TerminalPlugin");

	if(data==NULL)
		return;
	if(data->what==DOSHUTDOWN)
		{	
 			plugprefs.setValue("floating",this->dw->isFloating());
 			plugprefs.setValue("geom",this->dw->geometry());
 			plugprefs.setValue("currentstate",this->dw->isVisible());
		}
		
	if(data->what==DOSWITCHPAGE)
		{
			this->filePath=data->userStrData1;
			this->folderPath=data->userStrData3;
		}

	if(data->what==DOCONTEXTMENU)
		{
			QAction *act=new QAction("CD to Doc Folder ...");
			data->menu->addAction(act);
			QObject::connect(act,&QAction::triggered,[this]() { this->doMenuItem(CDTOFOLDER,this->currentTerminal); });
		}
}

void TerminalPluginPlug::doMenuItem(int what,int whome)
{
	QSettings plugprefs("KDHedger","TerminalPlugin");
	switch(what)
		{
			case VISCHANGED:
				this->terminals.at(whome).toggleTerm->setText(QString("Terminal %1").arg(whome+1));
				plugprefs.setValue("currentstate",this->terminals.at(0).dockWidget->isVisible());
				break;
			case CDTOFOLDER:
				this->terminals.at(whome).dockWidget->activateWindow();
				this->terminals.at(whome).dockWidget->raise();
				this->terminals.at(whome).console->changeDir(this->folderPath);
				this->terminals.at(whome).console->setFocus();
				break;
			case NEWTERM:
				this->addTerminal();
				break;
		}
}

