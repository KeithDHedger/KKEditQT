/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:39:55 GMT 2021 keithdhedger@gmail.com

 * This file (KKEditClass.cpp) is part of KKEditQT.

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

#include "KKEditClass.h"

KKEditClass::KKEditClass(QApplication *app)
{
	this->application=app;
	this->history=new HistoryClass(this);
	this->fileWatch=new QFileSystemWatcher(this);

}

KKEditClass::~KKEditClass()
{
	QDir	fold(this->tmpFolderName);

	delete this->fileWatch;

	for(int j=0;j<NOMORESHORTCUT;j++)
		delete this->appShortcuts[j];
	delete this->recentFiles;
	delete this->history;

	for(int j=0;j<this->plugins.count();j++)
		{
			if(this->plugins[j].loaded==true)
				{
					this->unloadPlug(&this->plugins[j]);
				}
		}

	fold.removeRecursively();

}

void KKEditClass::setUpToolBar(void)
{
	this->toolBar->clear();
	for(int j=0;j<this->prefsToolBarLayout.length();j++)
		{
			switch(this->prefsToolBarLayout.at(j).toLatin1())
				{
//new
					case 'N':
						this->toolBar->addAction(qobject_cast<QAction*>(this->newMenuItem));
						break;
//open+recent
					case 'O':
						{
							QPushButton *recent=new QPushButton(NULL);
							recent->setMenu(this->recentFiles->recentMenu);
							recent->setFlat(true);
							recent->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);	
							recent->setMaximumWidth(16);
							this->toolBar->addAction(qobject_cast<QAction*>(this->openMenuItem));
							this->toolBar->addWidget(qobject_cast<QWidget*>(recent));
						}
						break;
//save
					case 'S':
						this->toolBar->addAction(qobject_cast<QAction*>(this->saveMenuItem));
						break;

					case 's':
						this->toolBar->addSeparator();
						break;
//cut
					case 'X':
						this->toolBar->addAction(qobject_cast<QAction*>(this->cutMenuItem));
						break;
//copy
					case 'C':
						this->toolBar->addAction(qobject_cast<QAction*>(this->copyMenuItem));
						break;
//delete
					case 'd':
						this->toolBar->addAction(qobject_cast<QAction*>(this->deleteMenuItem));
						break;
//paste
					case 'P':
						this->toolBar->addAction(qobject_cast<QAction*>(this->pasteMenuItem));
						break;
//undo
					case 'U':
						this->toolBar->addAction(qobject_cast<QAction*>(this->undoMenuItem));
						break;
//redo
					case 'R':
						this->toolBar->addAction(qobject_cast<QAction*>(this->redoMenuItem));
						break;
//find
					case 'F':
						this->toolBar->addAction(qobject_cast<QAction*>(this->findMenuItem));
						break;
//navigation
					case 'G':
						this->toolBar->addAction(qobject_cast<QAction*>(this->goToDefineMenuItem));
						break;
//go back
					case 'B':
						this->toolBar->addAction(qobject_cast<QAction*>(this->goBackMenu));
						break;
//go foward
					case 'W':
						this->toolBar->addAction(qobject_cast<QAction*>(this->goFowardMenu));
						break;

					case '9':
						if(this->lineNumberWidget!=NULL)
							delete this->lineNumberWidget;
						this->lineNumberWidget=new QLineEdit;
						this->lineNumberWidget->setValidator(new QIntValidator);
						this->lineNumberWidget->setObjectName(QString("%1").arg(DOLINEBOX));
						this->lineNumberWidget->setToolTip("Go To Line");
						this->lineNumberWidget->setMaximumWidth(48);
						QObject::connect(this->lineNumberWidget,&QLineEdit::returnPressed,[this]()
							{
								this->gotoLine(this->lineNumberWidget->text().toInt());
							});
						QObject::connect(this->lineNumberWidget,&QLineEdit::textEdited,[this](const QString)
							{
								this->gotoLine(this->lineNumberWidget->text().toInt());
							});

						this->toolBar->addWidget(this->lineNumberWidget);
						break;
//find in gtkdoc
					case 'A':
						if(this->findGtkApiWidget!=NULL)
							delete this->findGtkApiWidget;
						this->findGtkApiWidget=new QLineEdit;
						this->findGtkApiWidget->setObjectName(QString("%1").arg(DOGTKSEARCH));
						this->findGtkApiWidget->setToolTip("Find API In Gtk Docs");
						QObject::connect(this->findGtkApiWidget,SIGNAL(returnPressed()),this,SLOT(doOddButtons()));
						this->toolBar->addWidget(this->findGtkApiWidget);
						break;

//find in qt5doc
					case 'Q':
						if(this->findQtApiWidget!=NULL)
							delete this->findQtApiWidget;
						this->findQtApiWidget=new QLineEdit;
						this->findQtApiWidget->setObjectName(QString("%1").arg(DOQT5SEARCH));
						this->findQtApiWidget->setToolTip("Find API In Qt5 Docs");
						QObject::connect(this->findQtApiWidget,SIGNAL(returnPressed()),this,SLOT(doOddButtons()));
						this->toolBar->addWidget(this->findQtApiWidget);
						break;

//find in function def
					case 'D':
						if(this->findDefWidget!=NULL)
							delete this->findDefWidget;
						this->findDefWidget=new QLineEdit;
						this->findDefWidget->setObjectName(QString("%1").arg(DOAPISEARCH));
						this->findDefWidget->setToolTip("Search For Define");
						QObject::connect(this->findDefWidget,SIGNAL(returnPressed()),this,SLOT(doOddButtons()));
						this->toolBar->addWidget(this->findDefWidget);
						break;
//livesearch
					case 'L':
						if(this->liveSearchWidget!=NULL)
							delete this->liveSearchWidget;
						this->liveSearchWidget=new QLineEdit;
						this->liveSearchWidget->setToolTip("Live Search");
						this->liveSearchWidget->setObjectName(QString("%1").arg(DOLIVESEARCH));
						QObject::connect(this->liveSearchWidget,SIGNAL(textChanged(QString)),this,SLOT(doLiveSearch(QString)));
						QObject::connect(this->liveSearchWidget,SIGNAL(returnPressed()),this,SLOT(doOddButtons()));
						this->toolBar->addWidget(this->liveSearchWidget);
						break;

//expander
					case 'E':
						{
							QHBoxLayout *hbox=new QHBoxLayout;
							QWidget		*widg=new QWidget;
    						hbox->addStretch(1);
    						widg->setLayout(hbox);
    						this->toolBar->addWidget(widg);
    					}
						break;
				}
		}
}

void KKEditClass::switchPage(int index)
{
	DocumentClass	*doc=NULL;
	plugData			pd;

	if(this->sessionBusy==true)
		return;

	doc=qobject_cast<DocumentClass*>(this->mainNotebook->widget(index));
	if(doc==0)
		return;
	if(doc==NULL)
		return;

	doc->setStatusBarText();
	doc->clearHilites();
	if(doc->state==CHANGEDONDISKTAB)
		{
			doc->state=NORMALTAB;
			doc->setTabColourType(NORMALTAB);
		}
	this->rebuildFunctionMenu(index);
	this->rebuildTabsMenu();

	this->currentFilepath=doc->getFilePath();
	this->currentFilename=doc->getFileName();

//plugins
	pd.doc=doc;
	pd.tabNumber=this->mainNotebook->currentIndex();
	pd.userIntData1=index;
	pd.what=DOSWITCHPAGE;
	this->runAllPlugs(pd);
}

void KKEditClass::rebuildBookMarkMenu()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
	MenuItemClass	*menuItemSink;

	this->bookMarkMenu->clear();
	menuItemSink=this->makeMenuItemClass(BOOKMARKSMENU,"Remove All Bookmarks",0,"list-remove",REMOVEALLBOOKMARKS,REMOVEALLBOOKMARKSMENUITEM);
	menuItemSink=this->makeMenuItemClass(BOOKMARKSMENU,"Toggle Bookmark",QKeySequence::fromString("Ctrl+T"),DATADIR"/pixmaps/BookMark.png",TOGGLEBOOKMARK,TOGGLEBOOKMARKMENUITEM);

	this->bookMarkMenu->addSeparator();
	this->bookMarks.clear();
#pragma GCC diagnostic pop
}

void KKEditClass::handleBMMenu(QWidget *widget,int what,QTextCursor curs)
{
	DocumentClass	*doc=this->pages.value(qobject_cast<DocumentClass*>(widget)->pageIndex);
	QTextCursor		cursor;
	bookMarkStruct	bms;

	if(curs.isNull()==true)
		cursor=doc->textCursor();
	else
		cursor=curs;

	switch(what)
		{
			case REMOVEBOOKMARKSFROMPAGE:
				foreach(bookMarkStruct value,this->bookMarks)
					{
						if(value.docIndex==doc->pageIndex)
							{
									this->bookMarkMenu->removeAction(value.menu);
									delete value.menu;
									this->bookMarks.remove(value.bmKey);
							}
					}
				return;
				break;
			case TOGGLEBOOKMARKMENUITEM:
				{
					foreach(bookMarkStruct value,this->bookMarks)
						{
							if((value.docIndex==doc->pageIndex) && (value.line==cursor.blockNumber()+1))
								{
									this->bookMarkMenu->removeAction(value.menu);
									this->bookMarks.remove(value.bmKey);
									return;
								}
						}

					bms.bmLabel=this->truncateWithElipses(cursor.block().text().simplified(),this->prefsMaxMenuChars);
					bms.line=cursor.blockNumber()+1;
					if(bms.bmLabel.isEmpty()==true)
						bms.bmLabel=doc->getFileName() + QString(" Line %1").arg(bms.line); 
					bms.docIndex=doc->pageIndex;
					bms.bmKey=this->bookMarksIndex;
					bms.menu=this->makeMenuItemClass(BOOKMARKSMENU,bms.bmLabel,0,NULL,"NOTNEEDED",this->bookMarksIndex);
					this->bookMarks[this->bookMarksIndex++]=bms;
				}
				break;

			default:
				this->history->pushToBackList(doc->getCurrentLineNumber(),doc->getFilePath());
				bms=this->bookMarks.value(what);
				doc=this->pages.value(bms.docIndex);
				this->mainNotebook->setCurrentWidget(doc);
				this->setTabVisibilty(this->mainNotebook->currentIndex(),true);
				this->gotoLine(bms.line);
				break;
		}
}

void KKEditClass::initApp(int argc,char** argv)
{
	char		tmpfoldertemplate[]="/run/KKEditQT-XXXXXX";
	QRect	r(0,0,1024,768);
	QDir		tdir;
	QString	tstr;
	QFile	file;

	this->homeFolder=QString("%1").arg(tdir.homePath());
	this->homeDataFolder=QString("%1/%2").arg(this->homeFolder).arg(KKEDITFOLDER);
	this->sessionFolder=QString("%1/%2/%3").arg(this->homeFolder).arg(KKEDITFOLDER).arg("sesssions");
	this->toolsFolder=QString("%1/%2/%3").arg(this->homeFolder).arg(KKEDITFOLDER).arg("tools");
	this->recentFiles=new RecentMenuClass(this);
	this->theme=new ThemeClass(this);

	QObject::connect(this->fileWatch,&QFileSystemWatcher::fileChanged,[this](const QString &path)
		{
			this->fileChangedOnDisk(path);
		});

	tdir.mkpath(this->sessionFolder);
	for(int j=0;j<MAXSESSIONS;j++)
		{
			QProcess::execute("touch",QStringList()<<this->sessionFolder+"/Session-"+QString::number(j));

			file.setFileName(QString("%1/Session-%2").arg(this->sessionFolder).arg(j));
			if(file.open(QIODevice::Text | QIODevice::ReadOnly))
				{
					tstr=QTextStream(&file).readLine();
					file.close();
					if(tstr.isEmpty()==true)
						{
							if(file.open(QIODevice::Text | QIODevice::WriteOnly))
								{
									if(j>0)
										{
											QTextStream(&file) << "New Session-" << j <<Qt::endl;
											this->sessionNames[j]=QString("New Session-%1").arg(j);
										}
									else
										{
											QTextStream(&file) << "Default Session" <<Qt::endl;
											this->sessionNames[0]=QString("Default Session");
										}
									file.close();
								}
						}
					else
						this->sessionNames[j]=tstr;
				}
		}

	this->tmpFolderName=mkdtemp(tmpfoldertemplate);

	this->gotDoxygen=QProcess::execute("sh",QStringList()<<"-c"<<"which doxygen 2>&1 >/dev/null");
	this->gotManEditor=QProcess::execute("sh",QStringList()<<"-c"<<"which manpageeditor 2>&1 >/dev/null");

//	if(getuid()!=0)
//		styleName="classic";
//	else
//		styleName="Root Source";
	//this->highlightColour="#808080";
	this->mainWindow=new QMainWindow;

	for(int j=0;j<NOMORESHORTCUT;j++)
		this->appShortcuts[j]=new QShortcut(this->mainWindow);

	this->setAppShortcuts();
	this->readConfigs();
	if(this->queueID==-1)
		{
			if((this->queueID=msgget(this->sessionID,IPC_CREAT|0660))==-1)
				fprintf(stderr,"Can't create message queue, scripting wont work :( ...\n");
		}
	this->checkMessages=new QTimer();
	QObject::connect(this->checkMessages,SIGNAL(timeout()),this,SLOT(doTimer()));
	this->checkMessages->start(this->prefsMsgTimer);
	this->theme->loadTheme(this->prefStyleName);
	this->buildMainGui();
	this->buildPrefsWindow();
	this->buildToolOutputWindow();
	this->loadPlugins();

#ifdef _BUILDDOCVIEWER_
	this->buildDocViewer();
#endif

	this->buildFindReplace();
#ifdef _ASPELL_
	AspellCanHaveError	*possible_err;
	this->aspellConfig=new_aspell_config();
	possible_err=new_aspell_speller(this->aspellConfig);

	if(aspell_error_number(possible_err)!= 0)
		puts(aspell_error_message(possible_err));
	else
		spellChecker=to_aspell_speller(possible_err);

	this->spellCheckMenuItem=new MenuItemClass("Spell Check");
	QIcon	itemicon=QIcon::fromTheme("tools-check-spelling");
	this->spellCheckMenuItem->setMenuID(SPELLCHECKMENUITEM);
	this->spellCheckMenuItem->setIcon(itemicon);
	QObject::connect(this->spellCheckMenuItem,SIGNAL(triggered()),this,SLOT(doOddMenuItems()));
	this->buildSpellCheckerGUI();
#endif

	this->htmlFile=QString("%1/Docview-%2.html").arg(this->tmpFolderName).arg(this->randomName(6));
	this->htmlURI="file://"+this->htmlFile;

	this->recentFiles->updateRecents();

	if(this->forceDefaultGeom==false)
		r=this->prefs.value("app/geometry",QVariant(QRect(50,50,1024,768))).value<QRect>();
	this->mainWindow->setGeometry(r);

//this->onExitSaveSession //TODO//
	this->setToolbarSensitive();
	this->mainWindow->show();
}

QString KKEditClass::randomName(int len)
{
	QString	retval="";

	const char alphanum[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	for(int i=0;i<len;++i)
		retval+=alphanum[rand()%(sizeof(alphanum)-1)];
   return(retval);
}

void KKEditClass::readConfigs(void)
{
//editor
	this->prefsFunctionMenuLayout=this->prefs.value("editor/funcsort",4).toInt();
	this->prefsDepth=this->prefs.value("editor/prefsdepth",1).toInt();
	this->prefsToolBarLayout=this->prefs.value("editor/toolbarlayout","NSOsURsBWsFGsE9ELEDEE").toString();
	this->prefsMaxTabChars=this->prefs.value("editor/maxtabchars",20).toInt();
	this->prefsMaxMenuChars=this->prefs.value("editor/maxfuncchars",64).toInt();
	this->prefsTerminalCommand=this->prefs.value("editor/terminalcommand","xterm -e").toString();
	this->prefsRootCommand=this->prefs.value("editor/rootcommand","gtksu -- ").toString();
	this->prefsQtDocDir=this->prefs.value("editor/qtdocdir","/usr/share/doc/qt5").toString();
	this->prefsNoOpenduplicate=this->prefs.value("editor/noopendup",QVariant(bool(true))).value<bool>();
	this->prefsNoWarnings=this->prefs.value("editor/nowarnings",QVariant(bool(false))).value<bool>();
	this->recentFiles->maxFiles=this->prefs.value("editor/maxrecents",10).toInt();
	this->prefsPrintCommand=this->prefs.value("editor/printcommand","").toString();

//document
	this->prefsHighLightline=this->prefs.value("document/highlightline",QVariant(bool(true))).value<bool>();
	this->prefsShowLineNumbers=this->prefs.value("document/showlinenumbers",QVariant(bool(true))).value<bool>();
	this->prefsDocumentFont=this->prefs.value("document/font",QVariant(QFont("Monospace",10))).value<QFont>();
	this->prefsSyntaxHilighting=this->prefs.value("document/syntaxhilighting",QVariant(bool(true))).value<bool>();
	this->prefsTabWidth=this->prefs.value("document/tabwidth",4).toInt();
	this->prefsLineWrap=this->prefs.value("document/wrap",QVariant(bool(true))).value<bool>();
	this->prefsIndent=this->prefs.value("document/indent",QVariant(bool(true))).value<bool>();
	this->prefsAutoShowCompletions=this->prefs.value("document/autoshowcompletions",QVariant(bool(true))).value<bool>();
	this->autoShowMinChars=this->prefs.value("document/autoshowminchars",6).toInt();

//theme
	this->prefStyleName=this->prefs.value("theme/style","default").toString();
	this->prefStyleNameHold=this->prefStyleName;
	this->prefsHiLiteLineColor=this->prefs.value("theme/hilitelinecol",QVariant(QColor(0xff,0xff,0xff,0x40))).value<QColor>();
	this->prefsBookmarkHiLiteColor=this->prefs.value("theme/bmhilitecol",QVariant(QColor(0,0,0,0x40))).value<QColor>();

//application
	this->prefsMenuStyleString=this->prefs.value("app/prefsmenustylestring","QMenu{menu-scrollable: true;padding: 0px;margin: 0px}").toString();
	this->prefsMsgTimer=this->prefs.value("app/msgtimer",1000).toInt();
	this->prefsUseSingle=this->prefs.value("app/usesingle",QVariant(bool(true))).value<bool>();
	this->prefsNagScreen=this->prefs.value("app/bekind",QVariant(bool(false))).value<bool>();
	this->defaultShortCutsList=this->prefs.value("app/shortcuts",QVariant(QStringList({"Ctrl+H","Ctrl+Y","Ctrl+?","Ctrl+K","Ctrl+Shift+H","Ctrl+D","Ctrl+Shift+D","Ctrl+L","Ctrl+M","Ctrl+Shift+M","Ctrl+@","Ctrl+'","Ctrl+Shift+C"}))).toStringList();
	this->onExitSaveSession=this->prefs.value("app/onexitsavesession",QVariant(bool(true))).value<bool>();
	this->disabledPlugins=this->prefs.value("app/disabledplugins").toStringList();

//find
	this->findList=this->prefs.value("find/findlist").toStringList();
	this->replaceList=this->prefs.value("find/replacelist").toStringList();
	this->wrapSearch=this->prefs.value("find/wrapsearch",QVariant(bool(false))).value<bool>();
	this->findInAllFiles=this->prefs.value("find/findinallfiles",QVariant(bool(false))).value<bool>();
	this->insensitiveSearch=this->prefs.value("find/insensitivesearch",QVariant(bool(false))).value<bool>();
	this->useRegex=this->prefs.value("find/useregex",QVariant(bool(false))).value<bool>();
	this->hightlightAll=this->prefs.value("find/hightlightall",QVariant(bool(false))).value<bool>();
	this->replaceAll=this->prefs.value("find/replaceall",QVariant(bool(false))).value<bool>();
	this->searchBack=this->prefs.value("find/searchback",QVariant(bool(false))).value<bool>();
	this->findAfterReplace=this->prefs.value("find/findafterreplace",QVariant(bool(false))).value<bool>();
	this->maxFRHistory=this->prefs.value("find/maxfrhistory",5).toInt();

	this->setAppShortcuts();	
}

void KKEditClass::tabContextMenu(const QPoint &pt)
{
	MenuItemClass	*menuitem;
	MenuItemClass	*menuitem1;
	int				tabIndex;
	QIcon			itemicon;
	DocumentClass	*doc=this->getDocumentForTab(-1);
	plugData			pd;

	if(pt.isNull())
		return;

	tabIndex=this->mainNotebook->tabBar()->tabAt(pt);

	if (tabIndex!=-1)
		{
			QMenu	menu(this->mainNotebook);
			QMenu	srcmenu(this->tabContextMenuItems[(SRCHILTIE-COPYFOLDERPATH)/0x100].label);
			QMenu	filemenu(this->tabContextMenuItems[(OPENFROMHERE-COPYFOLDERPATH)/0x100].label);
			for(int cnt=0;cnt<TABCONTEXTMENUCNT;cnt++)
				{
					if(cnt==(SRCHILTIE-COPYFOLDERPATH)/0x100)
						{
							menu.addMenu(&srcmenu);
							itemicon=QIcon::fromTheme(this->tabContextMenuItems[cnt].icon);
							srcmenu.setIcon(itemicon);
							for(int j=0;j<doc->highlighter->langPlugins.count();j++)
								{
									menuitem1=new MenuItemClass(doc->highlighter->langPlugins[j].langName);
									srcmenu.addAction(menuitem1);
									QObject::connect(menuitem1,&QAction::triggered,[doc,j]()
										{
											doc->highlighter->setLanguage(doc->highlighter->langPlugins[j].langName);
											doc->highlighter->setTheme();
										});
								}
							continue;
						}

					if(cnt==(OPENFROMHERE-COPYFOLDERPATH)/0x100)
						{
							menu.addMenu(&filemenu);
							DocumentClass	*doc=this->getDocumentForTab(tabIndex);
							QDir				dir(doc->getDirPath());
							QStringList		filters;
							QStringList		flist=dir.entryList(QDir::Files);
							itemicon=QIcon::fromTheme(this->tabContextMenuItems[cnt].icon);
							filemenu.setIcon(itemicon);
							for(int k=0;k<flist.count();k++)
								{
									if(flist.at(k).endsWith(".o")==false)
										{
											menuitem1=new MenuItemClass(flist.at(k));
											menuitem1->setMenuID(OPENFROMHERE+tabIndex);
											filemenu.addAction(menuitem1);
											QObject::connect(menuitem1,SIGNAL(triggered()),this,SLOT(doTabBarContextMenu()));
										}
								}
							continue;
						}
			
					menuitem=new MenuItemClass(this->tabContextMenuItems[cnt].label);
					menuitem->setMenuID(this->tabContextMenuItems[cnt].what+tabIndex);
					menu.addAction(menuitem);

					DocumentClass	*doc=this->getDocumentForTab(tabIndex);
					if(cnt==(LOCKCONTENTS-COPYFOLDERPATH)/0x100)
						{
							if(doc->isReadOnly()==true)
								{
									menuitem->setText("Unlock Contents");
									itemicon=QIcon::fromTheme("emblem-default");
								}
							else
								{
									itemicon=QIcon::fromTheme(this->tabContextMenuItems[cnt].icon);
									menuitem->setText(this->tabContextMenuItems[cnt].label);
								}
						}
					else
						itemicon=QIcon::fromTheme(this->tabContextMenuItems[cnt].icon);
					menuitem->setIcon(itemicon);
					QObject::connect(menuitem,SIGNAL(triggered()),this,SLOT(doTabBarContextMenu()));
				}
//plugins
			pd.menu=&menu;
			pd.tabNumber=tabIndex;
			pd.userStrData1=this->homeDataFolder;
			pd.userStrData2=this->getDocumentForTab(tabIndex)->getDirPath();
			pd.userStrData3=this->getDocumentForTab(tabIndex)->getFileName();
			pd.what=DOTABPOPUP;
			this->runAllPlugs(pd);
			menu.exec(this->mainNotebook->mapToGlobal(pt));
		}
}

void KKEditClass::writeExitData(void)
{
	if(this->verySafeFlag==true)
		return;
//editor
	if(this->forceDefaultGeom==false)
		this->prefs.setValue("app/geometry",this->mainWindow->geometry());

#ifdef _BUILDDOCVIEWER_
	this->prefs.setValue("app/viewergeometry",this->docView->geometry());
#endif

	this->prefs.setValue("editor/funcsort",this->prefsFunctionMenuLayout);
	this->prefs.setValue("editor/prefsdepth",this->prefsDepth);
	this->prefs.setValue("editor/toolbarlayout",this->prefsToolBarLayout);
	this->prefs.setValue("editor/maxtabchars",this->prefsMaxTabChars);
	this->prefs.setValue("editor/maxfuncchars",this->prefsMaxMenuChars);
	this->prefs.setValue("editor/terminalcommand",this->prefsTerminalCommand);
	this->prefs.setValue("editor/rootcommand",this->prefsRootCommand);
	this->prefs.setValue("editor/toolbarlayout",this->prefsToolBarLayout);
	this->prefs.setValue("editor/qtdocdir",this->prefsQtDocDir);
	this->prefs.setValue("editor/noopendup",this->prefsNoOpenduplicate);
	this->prefs.setValue("editor/nowarnings",this->prefsNoWarnings);
	this->prefs.setValue("editor/maxrecents",this->recentFiles->maxFiles);
	this->prefs.setValue("editor/printcommand",this->prefsPrintCommand);
	
//document
	this->prefs.setValue("document/indent",this->prefsIndent);
	this->prefs.setValue("document/wrap",this->prefsLineWrap);
	this->prefs.setValue("document/tabwidth",this->prefsTabWidth);
	this->prefs.setValue("document/syntaxhilighting",this->prefsSyntaxHilighting);
	this->prefs.setValue("document/font",this->prefsDocumentFont);
	this->prefs.setValue("document/showlinenumbers",this->prefsShowLineNumbers);
	this->prefs.setValue("document/highlightline",this->prefsHighLightline);
	this->prefs.setValue("document/autoshowcompletions",this->prefsAutoShowCompletions);
	this->prefs.setValue("document/autoshowminchars",this->autoShowMinChars);

//theme
	this->prefs.setValue("theme/style",this->prefStyleName);
	this->prefs.setValue("theme/hilitelinecol",this->prefsHiLiteLineColor);
	this->prefs.setValue("theme/bmhilitecol",this->prefsBookmarkHiLiteColor);

//application
	this->prefs.setValue("app/prefsmenustylestring",this->prefsMenuStyleString);
	this->prefs.setValue("app/msgtimer",this->prefsMsgTimer);
	this->prefs.setValue("app/usesingle",this->prefsUseSingle);
	this->prefs.setValue("app/bekind",this->prefsNagScreen);
	this->prefs.setValue("app/toolsopgeometry",this->toolOutputWindow->geometry());
	this->prefs.setValue("app/shortcuts",this->defaultShortCutsList);
	this->prefs.setValue("app/onexitsavesession",this->onExitSaveSession);

//find
	this->setSearchPrefs();
	this->findList=this->tailStringList(this->findList,this->maxFRHistory);
	this->replaceList=this->tailStringList(this->replaceList,this->maxFRHistory);
	this->prefs.setValue("find/findlist",this->findList);
	this->prefs.setValue("find/replacelist",this->replaceList);
	this->prefs.setValue("find/wrapsearch",this->wrapSearch);
	this->prefs.setValue("find/findinallfiles",this->findInAllFiles);
	this->prefs.setValue("find/insensitivesearch",this->insensitiveSearch);
	this->prefs.setValue("find/useregex",this->useRegex);
	this->prefs.setValue("find/hightlightall",this->hightlightAll);
	this->prefs.setValue("find/replaceall",this->replaceAll);
	this->prefs.setValue("find/searchback",this->searchBack);
	this->prefs.setValue("find/findafterreplace",this->findAfterReplace);
	this->prefs.setValue("find/maxfrhistory",this->maxFRHistory);

	if(this->safeFlag==false)
		{
			this->disabledPlugins.clear();
			for(int j=0;j<this->plugins.count();j++)
				{
					if(this->plugins[j].loaded==false)
						this->disabledPlugins<<this->plugins[j].plugPath;
				}
		}
	this->prefs.setValue("app/disabledplugins",this->disabledPlugins);
}

void KKEditClass::findFile(void)
{
	DocumentClass	*document=this->getDocumentForTab(-1);
	QString			selection;
	QString			filename;
	QString			results;
	QStringList		retval;

	if(document==NULL)
		return;

	selection=document->textCursor().block().text().trimmed();

	if((selection.isEmpty()==true) || (selection.startsWith('#')==false))
		return;

	filename=selection.replace(QRegExp("#.*include\\s*[\"<](.*)[\">]"),"\\1").trimmed();

	if(this->openFile(QString("%1/%2").arg(document->getDirPath()).arg(filename))==true)
		return;

	results=this->runPipeAndCapture(QString("find \"%1\" -iname \"%2\"").arg(document->getDirPath()).arg(filename));
	if(results.isEmpty()==false)
		{
			retval=results.split("\n",Qt::SkipEmptyParts);
			for(int j=0;j<retval.count();j++)
				this->openFile(retval.at(j));
		}

	results=this->runPipeAndCapture(QString("find \"/usr/include\" -iname \"%1\"").arg(filename));
	if(results.isEmpty()==true)
		return;

	retval=results.split("\n",Qt::SkipEmptyParts);
	for(int j=0;j<retval.count();j++)
		this->openFile(retval.at(j));
}

void KKEditClass::showBarberPole(QString windowtitle,QString bodylabel,QString cancellabel,QString maxitems,QString controlfile)//TODO//
{
	QStringList	arguments;

#ifdef _DEBUGCODE_
	QString		app="KKEditQT/app/KKEditQTProgressBar";
#else
	QString		app="KKEditQTProgressBar";
#endif
	arguments<<"-c"<<QString("\"%6\" \"%1\" \"%2\" \"%3\" \"%4\" \"%5\"").arg(windowtitle).arg(bodylabel).arg(cancellabel).arg(maxitems).arg(controlfile).arg(app);
	QProcess::startDetached("sh",arguments);
}

void KKEditClass::buildDocs(void)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);
	struct stat		sb;
	FILE				*fp;
	char				line[4096];
	QString			pipecom;
	QFileInfo		fileinfo;
	QDir				currentdir;

	if(doc==NULL)
		return;
	this->showBarberPole("Building Docs","Please Wait","","0",QString("%1/progress").arg(this->tmpFolderName));

	currentdir=QDir::current();
	QDir::setCurrent(doc->getDirPath());
	stat("Doxyfile",&sb);
	if(!S_ISREG(sb.st_mode))
		QProcess::execute("cp",QStringList()<<DATADIR "/docs/Doxyfile .");

	fileinfo=QString("%1/html/index.html").arg(doc->getDirPath());
	fp=popen("doxygen Doxyfile","r");
	while(fgets(line,4095,fp))
		{
			line[strlen(line)-1]=0;
			this->runNoOutput(QString("echo -n \"%1\n0\" >\"%2/progress\"").arg(line).arg(this->tmpFolderName));
		}
	pclose(fp);

	QString com=QString("/bin/echo '<meta http-equiv=\"refresh\" content=\"0; URL='file://%1'\" />' > %2").arg(fileinfo.absoluteFilePath()).arg(this->htmlFile);
	QProcess::execute("/bin/sh",QStringList()<<"-c"<<com);

	this->showWebPage("Doxygen Documentation","file://" + this->htmlFile);
	this->runNoOutput(QString("echo -e \"quit\n100\">\"%1/progress\"").arg(this->tmpFolderName));
	QDir::setCurrent(currentdir.canonicalPath());
}

void KKEditClass::showDocs(void)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);
	QFileInfo		fileinfo=QString("%1/html/index.html").arg(doc->getDirPath());

	if(fileinfo.exists()==false)
		this->buildDocs();
	else
		{
			QString com=QString("/bin/echo '<meta http-equiv=\"refresh\" content=\"0; URL='file://%1'\" />' > %2").arg(fileinfo.absoluteFilePath()).arg(this->htmlFile);
			QProcess::execute("/bin/sh",QStringList()<<"-c"<<com);
			this->showWebPage("Doxygen Documentation","file://" + this->htmlFile);
		}
}

void KKEditClass::closeAllTabs(void)
{
	bool retval;
	this->sessionBusy=true;
	int	numtabs=(this->mainNotebook)->count();
	for(int loop=0; loop<numtabs; loop++)
		{
			closingAllTabs=true;
			retval=this->closeTab(0);
			if(retval==false)
				{
					this->sessionBusy=false;
					return;
				}
		}

	this->sessionBusy=false;
	this->rebuildBookMarkMenu();
	this->rebuildTabsMenu();
	this->currentSessionNumber=0xdeadbeef;
	this->closingAllTabs=false;
	this->setToolbarSensitive();
	this->pages.clear();
	this->newPageIndex=1;
	this->rebuildFunctionMenu(-1);
}

bool KKEditClass::closeTab(int index)
{
	DocumentClass	*doc=NULL;
	int				thispage=index;
	QTextCursor		tc;
	bool				thisislasttab=false;
	plugData			pd;

	this->sessionBusy=true;

	if(this->closingAllTabs==true)
		thispage=0;
	else
		{
			if(index==-1)
				thispage=this->mainNotebook->currentIndex();
			else
				thispage=index;
		}

//plugins
	pd.doc=doc;
	pd.tabNumber=thispage;
	pd.what=DOCLOSE;
	this->runAllPlugs(pd);

	doc=qobject_cast<DocumentClass*>(this->mainNotebook->widget(thispage));
	if(doc!=0)
		{
			if(doc->dirty==true)
				{
					int result=this->askSaveDialog(doc->fileName);
					switch(result)
						{
							case QMessageBox::Save:
								this->saveFile(thispage,false);
       // Save was clicked
								break;
							case QMessageBox::Discard:
       // Don't Save was clicked
								break;
							case QMessageBox::Cancel:
       // Cancel was clicked
								this->sessionBusy=false;
								this->rebuildTabsMenu();
								return(false);
								break;
						}
				}

			if(this->closingAllTabs==false)
				this->handleBMMenu(this->mainNotebook->widget(thispage),REMOVEBOOKMARKSFROMPAGE,tc);

			if(thispage==this->mainNotebook->count()-1)
				thisislasttab=true;

			this->mainNotebook->removeTab(thispage);
			delete doc;
		}

	if(this->closingAllTabs==false)
		{
			if((thisislasttab==true) && (this->mainNotebook->count()>0))
				this->setTabVisibilty(this->mainNotebook->count()-1,true);
			this->sessionBusy=false;
			this->rebuildTabsMenu();
			this->setToolbarSensitive();
			this->rebuildFunctionMenu(-1);
		}

	return(true);
}

void KKEditClass::shutDownApp()
{
//	if(this->forcedMultInst==false)//TODO//
	this->writeExitData();

	if(this->onExitSaveSession==true)
		this->doSessionsMenuItems();

	if(this->saveAllFiles(true)==true)
		{
#ifdef _ASPELL_
			delete_aspell_config(this->aspellConfig);
			delete_aspell_speller(this->spellChecker);
#endif
			QApplication::quit();
		}
}

QString KKEditClass::truncateWithElipses(const QString str,int maxlen)
{
	QString newlabel;
	if(str.length()>maxlen)
		newlabel=str.left((maxlen-3)/2)+"..."+str.right((maxlen-3)/2);
	else
		newlabel=str;
	return(newlabel);
}

void KKEditClass::setAppShortcuts(void)
{
	for(int j=0;j<NOMORESHORTCUT;j++)
		{
			if(this->appShortcuts[j]!=NULL)
				{
					delete this->appShortcuts[j];
					this->appShortcuts[j]=new QShortcut(this->mainWindow);
				}
			if(this->defaultShortCutsList.size()>j)
				{
					this->appShortcuts[j]->setKey(QKeySequence(this->defaultShortCutsList.at(j)));
					this->appShortcuts[j]->setObjectName(QString("%1").arg(j));
					QObject::connect(this->appShortcuts[j],SIGNAL(activated()),this,SLOT(doAppShortCuts()));
				}
			else
				{
					this->defaultShortCutsList<<"";
					this->appShortcuts[j]->setObjectName(QString("%1").arg(NOSHORTCUT));
				}
		}
}

void KKEditClass::setToolbarSensitive(void)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);
	bool				override;
	bool				gotdoc=true;
	bool				hasselection=false;
	plugData			pd;

	if(this->sessionBusy==true)
		return;

	if(doc==NULL)
		{
			override=false;
			gotdoc=false;
		}
	else
		{
			override=doc->dirty;
			hasselection=doc->textCursor().hasSelection();
		}

	if(this->mainNotebook->count()==0)
		{
			this->saveSessionsMenu->setEnabled(false);
			this->currentSessionNumber=0xdeadbeef;
			this->mainNotebook->setScrollButtonStatus(BOTHSCROLLBUTTONS,false,false);
		}
	else
		{
			this->mainNotebook->setScrollButtonStatus(BOTHSCROLLBUTTONS,true,true);
			if(this->mainNotebook->currentIndex()==0)
				this->mainNotebook->setScrollButtonStatus(LEFTSCROLLBUTTON,false,true);
			else if(this->mainNotebook->currentIndex()==this->mainNotebook->count()-1)
				this->mainNotebook->setScrollButtonStatus(RIGHTSCROLLBUTTON,false,true);
			if(this->mainNotebook->count()==1)
				this->mainNotebook->setScrollButtonStatus(BOTHSCROLLBUTTONS,false,true);
			this->saveSessionsMenu->setEnabled(true);
		}

	if(this->currentSessionNumber==0xdeadbeef)
		this->saveCurrentSessionMenuItem->setEnabled(false);
	else
		this->saveCurrentSessionMenuItem->setEnabled(true);
		
	this->restoreDefaultSessionMenuItem->setEnabled(this->onExitSaveSession);

	this->pluginMenu->setEnabled(!this->pluginMenu->isEmpty());

	for(int j=0;j<this->prefsToolBarLayout.length();j++)
		{
			switch(this->prefsToolBarLayout.at(j).toLatin1())
				{
//new
					case 'N':
						this->newMenuItem->setEnabled(true);
						break;
//open+recent
					case 'O':
						this->newMenuItem->setEnabled(true);
						break;
//save
					case 'S':
						this->saveMenuItem->setEnabled(override);
						break;

//cut
					case 'X':
						this->cutMenuItem->setEnabled((gotdoc==true) && ((hasselection) & (!doc->isReadOnly())));
						break;
//copy
					case 'C':
						this->copyMenuItem->setEnabled(hasselection);
						break;
//paste
					case 'P':
						this->pasteMenuItem->setEnabled((gotdoc==true) && (doc->canPaste()));
						break;
//delete
					case 'd':
						this->deleteMenuItem->setEnabled((gotdoc==true) && (hasselection==true));
						break;
//undo
					case 'U':
						this->undoMenuItem->setEnabled((gotdoc==true) && (doc->gotUndo));
						this->undoAllMenuItem->setEnabled((gotdoc==true) && (doc->gotUndo));
						break;
//redo
					case 'R':
						this->redoMenuItem->setEnabled((gotdoc==true) && (doc->gotRedo));
						this->redoAllMenuItem->setEnabled((gotdoc==true) && (doc->gotRedo));
						break;
//find
					case 'F':
						this->findMenuItem->setEnabled(true);
						break;
//navigation
					case 'G':
						this->goToDefineMenuItem->setEnabled(hasselection);
						break;
//go back
					case 'B':
						this->goBackMenu->setEnabled(true);
						break;
//go foward
					case 'W':
						this->goFowardMenu->setEnabled(true);
						break;

					case '9':
						this->lineNumberWidget->setEnabled(gotdoc);
						this->goToLineDialogMenuItem->setEnabled(gotdoc);
						break;
//find in gtkdoc
					case 'A':
						this->findGtkApiWidget->setEnabled(true);
						break;

//find in qt5doc
					case 'Q':
						this->findQtApiWidget->setEnabled(true);
						break;
//find in function def
					case 'D':
						this->findDefWidget->setEnabled(true);
						break;
//livesearch
					case 'L':
						this->liveSearchWidget->setEnabled(gotdoc);
						break;
				}
		}
//plugins
	pd.tabNumber=this->mainNotebook->currentIndex();
	pd.what=DOSETSENSITVE;
	this->runAllPlugs(pd);
}

void KKEditClass::runCLICommands(int quid)
{
	msgStruct	message;
	int			msglen;
	QString		opensessionname;
	QStringList	list;
	char			*pathtopwd;

	if(quid==-1)
		fprintf(stderr,"From KKeditQT Can't create message queue, scripting wont work :( ...\n");
	else
		{
			if(this->parser.isSet("quit"))
				{
 					msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s","quit");
					message.mType=QUITAPPMSG;
					msgsnd(quid,&message,msglen,0);
				}

			if(this->parser.isSet("restore-session"))
				{
					opensessionname=this->parser.value("restore-session");
 					msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s",opensessionname.toStdString().c_str());
					message.mType=RESTORESESSIONMSG;
					msgsnd(quid,&message,msglen,0);
				}

			pathtopwd=get_current_dir_name();
			list=this->parser.positionalArguments();
			for(int j=0;j<list.count();j++)
				{
					if(list.at(j).at(0)!="/")
						msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s/%s",pathtopwd,list.at(j).toStdString().c_str());
					else
						msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s",list.at(j).toStdString().c_str());
					message.mType=OPENFILEMSG;
					msgsnd(quid,&message,msglen,0);
				}
			msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s","ACTIVATE");
			message.mType=ACTIVATEAPPMSG;
			msgsnd(quid,&message,msglen,0);

			if(pathtopwd!=NULL)
				free(pathtopwd);
		}
}

void KKEditClass::setDocMenu(void)
{
#ifdef _BUILDDOCVIEWER_
	if(this->docView->isVisible()==true)//ugly hack!!//
		{
			this->toggleDocViewMenuItem->setText("Hide Docviewer");
			this->docviewerVisible=true;
		}
	else
		{
			this->toggleDocViewMenuItem->setText("Show Docviewer");
			this->docviewerVisible=false;
		}
#endif
}

void KKEditClass::showWebPage(QString windowtitle,QString url)
{
#ifdef _BUILDDOCVIEWER_
	if(windowtitle.isEmpty()==false)
		this->docView->setWindowTitle(windowtitle);

	this->webView->load(QUrl::fromUserInput(url));
	this->docView->show();
	this->docView->activateWindow();
	this->docView->raise();

	this->docView->setWindowState(Qt::WindowNoState);//TODO//doesnt work

	this->setDocMenu();
#else
	QDesktopServices::openUrl(QUrl(url));
#endif
}

void KKEditClass::printDocument(void)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);

	if(doc==NULL)
		return;

	if(this->prefsPrintCommand.isEmpty()!=true)
		{
			QProcess::startDetached(this->prefsPrintCommand,QStringList(doc->getFilePath()));
			return;
		}

//N.B. CUPS/QT5 has a problem with QT5/CUPS so only print to pdf is available, this aint my fault! 
	QPrinter		printer(QPrinter::HighResolution);
	printer.setOutputFileName(QString("%1/Documents/%2.pdf").arg(this->homeFolder).arg(doc->getFileName()));
 
    QPrintDialog	dialog(&printer,this->mainWindow);
	if(dialog.exec()!=QDialog::Accepted)
		return;

	doc->print(&printer);
}

void KKEditClass::setCompWordList(void)
{
	QString				results;
	QString				command;
	QString				paths;
	DocumentClass		*doc;
	QAbstractItemModel	*model;

	for(int j=0;j<this->mainNotebook->count();j++)
		{
			doc=this->getDocumentForTab(j);
			paths+="'"+doc->getFilePath()+"' ";
		}

	command=QString("grep -Eho '[[:alpha:]_]{%1,}' %2|sort -u").arg(this->autoShowMinChars).arg(paths);
	results=this->runPipeAndCapture(command);
	this->completionWords=results.split("\n",Qt::SkipEmptyParts);

	if(this->completer!=NULL)
		delete this->completer;

    this->completer=new QCompleter(this->completionWords,this);
	this->completer->setCaseSensitivity(Qt::CaseInsensitive);
	model=new QStringListModel(this->completionWords,this->completer);
	this->completer->setCompletionMode(QCompleter::PopupCompletion);
	this->completer->setModel(model);
	this->completer->setWrapAround(false);
	QObject::connect(this->completer,QOverload<const QString &>::of(&QCompleter::activated),[=](const QString &text)
		{
			this->insertCompletion(text);
		});
}

void KKEditClass::insertCompletion(const QString& completion)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);
	QTextCursor		tc;
	if(this->completer->widget()!=doc)
		return;
	tc=doc->textCursor();
	tc.movePosition(QTextCursor::PreviousWord,QTextCursor::MoveAnchor);
	tc.movePosition(QTextCursor::NextWord,QTextCursor::KeepAnchor);
	tc.removeSelectedText();
    tc.insertText(completion);
    doc->setTextCursor(tc);
}

void KKEditClass::loadPlugins(void)
{
	int				cnt=0;
	QDir				pluginsDir(this->homeDataFolder+"/plugins/");

	if(this->verySafeFlag==true)
		return;

//local plugins
	QDirIterator		lit(pluginsDir.canonicalPath() ,QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);
	while (lit.hasNext())
		{
			QString s=lit.next();
			pluginStruct	ps;

			ps.plugPath=s;
			ps.local=true;
			if(this->loadPlug(&ps)==false)
				qWarning()<<"Error loading plug > " << s;

			this->plugins[cnt++]=ps;
		}

//global plugins
	pluginsDir.setPath(QString("%1/plugins/").arg(DATADIR));
	QDirIterator				git(pluginsDir.canonicalPath() ,QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);
	while (git.hasNext())
		{
			QString			s=git.next();
			pluginStruct	ps;

			ps.plugPath=s;
			ps.local=false;
			if(this->loadPlug(&ps)==false)
				qWarning()<<"Error loading plug > " << s;

			this->plugins[cnt++]=ps;
		}
}

bool KKEditClass::loadPlug(pluginStruct *ps,bool force)
{
	QObject	*plugininst=NULL;

	ps->statusChanged=false;

	if(ps->loaded==true)
		return(true);

	ps->pluginLoader=new QPluginLoader(ps->plugPath);
	plugininst=ps->pluginLoader->instance();
	if(plugininst!=nullptr)
		{
			ps->instance=qobject_cast<kkEditQTPluginInterface*>(plugininst);
			if((this->disabledPlugins.contains(ps->plugPath)==false) || (force==true))
				{
					if((this->safeFlag==false)||(force==true))
						{
							ps->instance->initPlug(this,ps->plugPath);//TODO//return false if cant init
							ps->loaded=true;
							ps->wants=ps->instance->plugWants();
						}
					else
						{
							ps->loaded=false;
							ps->wants=DONONE;
						}
				}
			else
				{
					ps->loaded=false;
					ps->wants=DONONE;
				}
					
			ps->plugName=ps->pluginLoader->metaData().value("MetaData").toObject().value("name").toString();
			ps->plugVersion=ps->pluginLoader->metaData().value("MetaData").toObject().value("version").toString();
		}
	else
		{
			ps->loaded=false;
			ps->broken=true;
			ps->plugName=QFileInfo(ps->plugPath).fileName();
			return(false);
		}

	return(true);
}

bool KKEditClass::unloadPlug(pluginStruct *ps)
{
	ps->statusChanged=false;

	if(ps->loaded==false)
		return(true);

	ps->instance->unloadPlug();
	ps->pluginLoader->unload();
	delete ps->pluginLoader;
	ps->pluginLoader=NULL;
	ps->loaded=false;

	return(true);
}

void KKEditClass::setTabVisibilty(int tab,bool visible)
{
	DocumentClass	*doc;
	int				tabnum=tab;
	bool				vis=visible;

	if(this->sessionBusy==false)
		if(tabnum==this->mainNotebook->count()-1)//ui bug fix no last tab invisible
			vis=true;

	this->mainNotebook->setTabVisible(tabnum,vis);
	doc=this->getDocumentForTab(tabnum);
	doc->visible=vis;

	if(vis==false)//hacks for tab gliches
		{
			if(this->mainNotebook->currentIndex()==tabnum)
				this->mainNotebook->scrollTabsLeft();
			tabnum=this->mainNotebook->currentIndex();
			this->mainNotebook->setCurrentIndex(0);
			this->mainNotebook->setCurrentIndex(tabnum);
		}
	else
		{
			this->mainNotebook->setCurrentIndex(0);
			this->mainNotebook->setCurrentIndex(tabnum);
		}
}

void KKEditClass::runAllPlugs(plugData pd)
{
	for(int j=0;j<this->plugins.count();j++)
		{
			if((this->plugins[j].loaded) && ((this->plugins[j].wants & pd.what)==pd.what))
				{
					pd.plugName=this->plugins[j].plugName;
					pd.plugPath=this->plugins[j].plugPath;
					pd.plugVersion=this->plugins[j].plugVersion;
#ifdef _DEBUGCODE_
					pd.printIt();
#endif
					this->plugins[j].instance->plugRun(&pd);
				}
		}
}

void KKEditClass::setBit(int *data,int bit)
{
	*data=*data | (1<<bit);
}

void KKEditClass::resetBit(int *data,int bit)
{
	*data=*data & ~(-1 & (1<<bit));
}

int KKEditClass::getBit(int data,int bit)
{
	return((data & (1<<bit)) && true);
}

QStringList KKEditClass::tailStringList(QStringList list,int maxsize)
{	
	QStringList tlist2=list;
	QStringList tlist;

	if(list.size()<maxsize)
		return(list);

	tlist2.removeDuplicates();
	for(int j=tlist2.size()-maxsize;j<tlist2.size();j++)
		tlist.append(tlist2.at(j));
	return(tlist);
}

void KKEditClass::runNoOutput(QString command,bool sync,bool asroot)
{
	QStringList	args;
	QString		com;

	if(asroot==false)
		{
			com="sh";
			args<<"-c"<<QString("cd %1;%2").arg(this->toolsFolder).arg(command);
		}
	else
		{
			args=QProcess::splitCommand(this->prefsRootCommand);
			com=args.at(0);
			args.removeFirst();
			args<<"sh"<<"-c"<<QString("cd %1;%2").arg(this->toolsFolder).arg(command);
		}

	if(sync==true)
		QProcess::execute(com,args);
	else
		QProcess::startDetached(com,args);
}

void KKEditClass::sendMessgage(QString msg)
{
	msgStruct	buffer;

	strncpy(buffer.mText,msg.toStdString().c_str(),MAXMSGSIZE-1);
	buffer.mType=SENDMSG;
	if((msgsnd(queueID,&buffer,strlen(buffer.mText)+1,0))==-1)
		{
			qDebug()<<"Can't send message :"<<msg;
		}
}
