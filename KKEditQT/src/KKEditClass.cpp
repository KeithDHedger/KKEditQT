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
#ifndef moc_KKEditClass
#include "moc_KKEditClass.cpp"
#define moc_KKEditClass
#endif

static const char			*replacementShorts[]={"Ctrl+H","Ctrl+Y","Ctrl+?","Ctrl+K","Ctrl+Shift+H","Ctrl+D","Ctrl+Shift+D","Ctrl+L","Ctrl+M","Ctrl+Shift+M","Ctrl+@","Ctrl+'","Ctrl+Shift+F"};
static const QStringList		reservedShortcutKeys={"Ctrl+Shift+C","Ctrl+Shift+V"};

KKEditClass::KKEditClass(QApplication *app)
{
	this->application=app;
	this->history=new HistoryClass(this);
	this->fileWatch=new QFileSystemWatcher(this);

}

KKEditClass::~KKEditClass()
{
	plugData		pd;
	QDir			fold(this->tmpFolderName);

	if(this->tmpFolderName.isEmpty()==true)
		{
			return;
		}

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

	system(QString("rm %1/* 2>/dev/null").arg(this->tmpFolderName).toStdString().c_str());/**/
	system(QString("rmdir %1 2>/dev/null").arg(this->tmpFolderName).toStdString().c_str());
#ifdef _BUILDDOCVIEWER_
	delete this->webEngView;
#endif
}

void KKEditClass::handleSignal(int signum)
{
	switch(signum)
		{
			case SIGUSR1:
				this->doTimer();
				break;
			case SIGTERM:
			case SIGINT:
				this->shutDownApp();
				break;
			default:
				break;
		}
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
						this->toolBar->addAction(qobject_cast<QAction*>(this->goToDefineMenuSingleItem));
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

	this->mainWindow->setWindowTitle("KKEditQT - "+doc->getFileName());
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
	pd.userStrData1=this->currentFilepath;
	pd.userStrData2=currentFilename;
	pd.userStrData3=doc->getDirPath();
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
	char		tmpfoldertemplate[]="/tmp/KKEditQT-XXXXXX";
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

//qDebug()<<tmpfoldertemplate;
		this->tmpFolderName=mkdtemp(tmpfoldertemplate);
		if(this->tmpFolderName.isEmpty()==true)
			{
				qDebug()<<"Can't create temporary folder, quitting ...";
				exit (100);
			}

//qDebug()<<this->tmpFolderName;
	this->gotDoxygen=QProcess::execute("sh",QStringList()<<"-c"<<"which doxygen 2>&1 >/dev/null");
	this->gotManEditor=QProcess::execute("sh",QStringList()<<"-c"<<"which manpageeditor 2>&1 >/dev/null");
	this->gotPDFToText=QProcess::execute("sh",QStringList()<<"-c"<<"which pdftotext 2>&1 >/dev/null");
	this->gotPDFCrop=QProcess::execute("sh",QStringList()<<"-c"<<"which pdfcrop 2>&1 >/dev/null");

	this->mainThemeProxy=new ProxyStyle();
// take ownership to avoid memleak
	this->mainThemeProxy->setParent(this->application);
	this->application->setStyle(this->mainThemeProxy);

//	if(getuid()!=0)
//		styleName="classic";
//	else
//		styleName="Root Source";


	this->mainWindow=new QMainWindow();

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
	this->prefsRootCommand=this->prefs.value("editor/rootcommand","gtksu -- env QTWEBENGINE_DISABLE_SANDBOX=1 env QT_QPA_PLATFORMTHEME=qt5ct ").toString();
	this->prefsNoOpenduplicate=this->prefs.value("editor/noopendup",QVariant(bool(true))).value<bool>();
	this->prefsNoWarnings=this->prefs.value("editor/nowarnings",QVariant(bool(false))).value<bool>();
	this->recentFiles->maxFiles=this->prefs.value("editor/maxrecents",10).toInt();
	this->prefsPrintCommand=this->prefs.value("editor/printcommand","").toString();
	this->prefsPageSize=this->prefs.value("editor/prefspagesize","").toInt();

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
	this->prefsMsgTimer=this->prefs.value("app/msgtimer",1000).toInt();
	this->prefsUseSingle=this->prefs.value("app/usesingle",QVariant(bool(true))).value<bool>();
	this->prefsNagScreen=this->prefs.value("app/bekind",QVariant(bool(false))).value<bool>();
	this->defaultShortCutsList=this->prefs.value("app/shortcuts",QVariant(QStringList({"Ctrl+H","Ctrl+Y","Ctrl+?","Ctrl+K","Ctrl+Shift+H","Ctrl+D","Ctrl+Shift+D","Ctrl+L","Ctrl+M","Ctrl+Shift+M","Ctrl+@","Ctrl+'","Ctrl+Shift+F"}))).toStringList();
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
	QRect rg;
	QRect rf;

	if(this->verySafeFlag==true)
		return;
//editor
	if(this->forceDefaultGeom==false)
		{
			rg=this->mainWindow->geometry();
			rf=this->mainWindow->frameGeometry();
			rf.setHeight(rf.height()-(rf.height()-rg.height()));
			rf.setWidth(rf.width()-(rf.width()-rg.width()));
			this->prefs.setValue("app/geometry",rf);
		}

#ifdef _BUILDDOCVIEWER_
	rg=this->docView->geometry();//TODO//
	rf=this->docView->frameGeometry();//TODO//
	rf.setHeight(rf.height()-(rf.height()-rg.height()));
	rf.setWidth(rf.width()-(rf.width()-rg.width()));
	this->prefs.setValue("app/viewergeometry",rf);
#endif

	this->prefs.setValue("editor/funcsort",this->prefsFunctionMenuLayout);
	this->prefs.setValue("editor/prefsdepth",this->prefsDepth);
	this->prefs.setValue("editor/toolbarlayout",this->prefsToolBarLayout);
	this->prefs.setValue("editor/maxtabchars",this->prefsMaxTabChars);
	this->prefs.setValue("editor/maxfuncchars",this->prefsMaxMenuChars);
	this->prefs.setValue("editor/terminalcommand",this->prefsTerminalCommand);
	this->prefs.setValue("editor/rootcommand",this->prefsRootCommand);
	this->prefs.setValue("editor/toolbarlayout",this->prefsToolBarLayout);
	this->prefs.setValue("editor/noopendup",this->prefsNoOpenduplicate);
	this->prefs.setValue("editor/nowarnings",this->prefsNoWarnings);
	this->prefs.setValue("editor/maxrecents",this->recentFiles->maxFiles);
	this->prefs.setValue("editor/printcommand",this->prefsPrintCommand);
	this->prefs.setValue("editor/prefspagesize",this->prefsPageSize);
	
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

	filename=selection.replace(QRegularExpression("#.*include\\s*[\"<](.*)[\">]"),"\\1").trimmed();

	if(this->openFile(QString("%1/%2").arg(document->getDirPath()).arg(filename))==true)
		return;

	results=this->runPipeAndCapture(QString("find \"%1\" -iwholename \"%2\" 2>/dev/null").arg(document->getDirPath()).arg(filename));
	if(results.isEmpty()==false)
		{
			retval=results.split("\n",Qt::SkipEmptyParts);
			for(int j=0;j<retval.count();j++)
				this->openFile(retval.at(j));
		}

	results=this->runPipeAndCapture(QString("find \"/usr/include\" -iwholename \"/usr/include/%1\" 2>/dev/null").arg(filename));
	if(results.isEmpty()==false)
		{
			retval=results.split("\n",Qt::SkipEmptyParts);
			for(int j=0;j<retval.count();j++)
				this->openFile(retval.at(j));
		}

	results=this->runPipeAndCapture(QString("find \"/usr/include/\" -name \"%1\" 2>/dev/null").arg(filename));
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
	QString		app="KKEditQT/app/kkeditqtprogressbar";
#else
	QString		app="kkeditqtprogressbar";
#endif
	arguments<<"-c"<<QString("\"%6\" \"%1\" \"%2\" \"%3\" \"%4\" \"%5\"").arg(windowtitle).arg(bodylabel).arg(cancellabel).arg(maxitems).arg(controlfile).arg(app);
	QProcess::startDetached("sh",arguments);
}

miniPrefsReturnStruct KKEditClass::miniPrefsDialog(QString prefsname,QStringList items,bool liveupdate)
{
	miniPrefsReturnStruct	prefs;
	QSettings				miniprefsname("KDHedger",prefsname);
	QWidget					*hbox;
	QVBoxLayout				*docvlayout=new QVBoxLayout;
	QHBoxLayout				*hlayout;
	QPushButton				*cancelbutton=new QPushButton("&Cancel");
	QPushButton				*okbutton=new QPushButton("&Apply");
	std::string				prefsentry;

	prefs.theDialog=new QDialog();

	QObject::connect(cancelbutton,&QPushButton::clicked,[this,prefs]()
		{
			prefs.theDialog->reject();
		});
	QObject::connect(okbutton,&QPushButton::clicked,[this,prefs]()
		{
			prefs.theDialog->accept();
		});

	for(int j=0;j<items.size();j++)
		{
			hbox=new QWidget;
			hlayout=new QHBoxLayout;
			hlayout->setContentsMargins(0,0,0,0);
			hbox->setLayout(hlayout);
			hlayout->addWidget(new QLabel(items.at(j)),0,Qt::AlignLeft);
			prefsentry=LFSTK_UtilityClass::LFSTK_strStrip(items.at(j).toStdString());
			prefsentry=LFSTK_UtilityClass::LFSTK_strReplaceAllChar(prefsentry," ","",true);
			prefs.boxes[j]=new QLineEdit(miniprefsname.value(prefsentry.c_str(),"").toString().simplified());	
			if(liveupdate==true)
				{
					QObject::connect(prefs.boxes[j],&QLineEdit::textEdited,[j,items,prefsname,prefs](const QString)
						{
							QSettings	miniprefsname("KDHedger",prefsname);
							std::string	prefsentry=LFSTK_UtilityClass::LFSTK_strStrip(items.at(j).toStdString());
							prefsentry=LFSTK_UtilityClass::LFSTK_strReplaceAllChar(prefsentry," ","",true);
							miniprefsname.setValue(prefsentry.c_str(),prefs.boxes[j]->text());
						});
				}
			hlayout->addWidget(prefs.boxes[j],1,Qt::AlignRight);
			docvlayout->addWidget(hbox);
		}
	hbox=new QWidget;
	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	hbox->setLayout(hlayout);
	hlayout->addWidget(cancelbutton);
	hlayout->addStretch(0);
	hlayout->addWidget(okbutton);

	docvlayout->addWidget(hbox);

	prefs.theDialog->setLayout(docvlayout);
	return(prefs);
}

void KKEditClass::buildDocset(void)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);
	FILE				*fp;
	char				line[4096];
	QDir				currentdir;

	if(doc==NULL)
		return;

	QSettings	buildDocsetPrefs("KDHedger","DocsetPrefs");
	QString		makedocsfolder=QString("%1/docs/").arg(DATADIR);
	bool			gotdoxyfile;
	QString		thename;
	QString		thetype;
	QString		thepath;
	QString		theanchor;
	sqlite3		*DB;
	QString		sql;
	char			*messaggeError=NULL; 
	int			exit=0;
	QDir			d;
	miniPrefsReturnStruct myprefs;

	QString		projectname;
	QString		versionbox;
	QString		iconbox;
	QString		destdir;

	if(QFile::exists(doc->getDirPath()+"/Doxyfile"))
		{
			gotdoxyfile=true;
			QProcess::execute("cp",QStringList()<<doc->getDirPath()+"/Doxyfile"<<this->tmpFolderName);
		}
	else
		{
			QProcess::execute("cp",QStringList()<<DATADIR "/docs/Doxyfile"<<this->tmpFolderName);
			gotdoxyfile=false;
		}

	if(gotdoxyfile==false)
		{
			projectname=buildDocsetPrefs.value("ProjectName","MyProject").toString().simplified();
			versionbox=buildDocsetPrefs.value("ProjectVersion","0.0.1").toString().simplified();
		}
	else
		{
			projectname=runPipeAndCapture(QString("sed -n 's/PROJECT_NAME=\\(.*\\)/\\1/p' %1").arg(doc->getDirPath()+"/Doxyfile")).simplified();
			versionbox=runPipeAndCapture(QString("sed -n 's/PROJECT_NUMBER=\\(.*\\)/\\1/p' %1").arg(doc->getDirPath()+"/Doxyfile")).simplified();	
		}
	destdir=buildDocsetPrefs.value("DestinationFolder",QString("%1/.local/share/Zeal/Zeal/docsets").arg(QDir::homePath())).toString();
	iconbox=buildDocsetPrefs.value("IconPath",QString("%1/LFSTux.png").arg(makedocsfolder)).toString();

	myprefs=this->miniPrefsDialog("DocsetPrefs",QStringList()<<"Project Name"<<"Project Version"<<"Icon Path"<<"Destination Folder");
	myprefs.boxes[0]->setText(projectname);
	myprefs.boxes[1]->setText(versionbox);
	myprefs.boxes[2]->setText(iconbox);
	myprefs.boxes[3]->setText(destdir);
	int res=myprefs.theDialog->exec();

	if(res==1)
		{
			runPipeAndCapture(QString("sed -i 's/^PROJECT_NAME=.*$/PROJECT_NAME=%1/;s/^PROJECT_NUMBER=.*$/PROJECT_NUMBER=%2/;s@^OUTPUT_DIRECTORY.*=.*$@OUTPUT_DIRECTORY=%3@;s/^GENERATE_DOCSET.*=.*$/GENERATE_DOCSET=YES/;s/^SEARCHENGINE.*=.*$/SEARCHENGINE=NO/;s/^DOT_IMAGE_FORMAT.*=.*$/DOT_IMAGE_FORMAT=png/' '%3/Doxyfile'").arg(myprefs.boxes[0]->text()).arg(myprefs.boxes[1]->text()).arg(this->tmpFolderName));

			this->showBarberPole("Building Docset","Please Wait","","0",QString("%1/progress").arg(this->tmpFolderName));
			fp=popen(QString("pushd '%1'&>/dev/null;doxygen '%2/Doxyfile';popd &>/dev/null").arg(doc->getDirPath()).arg(this->tmpFolderName).toStdString().c_str(),"r");
			if(fp!=NULL)
				{
					while(fgets(line,4095,fp))
						{
							line[strlen(line)-1]=0;
							this->runNoOutput(QString("echo -n \"%1\n0\" >\"%2/progress\"").arg(line).arg(this->tmpFolderName));
						}
					pclose(fp);
				}
			else
				{
					this->runNoOutput(QString("echo -e \"quit\n100\">\"%1/progress\"").arg(this->tmpFolderName));
					return;
				}

			currentdir=QDir::current();
			QDir::setCurrent(this->tmpFolderName+"/html");

			d.mkpath(myprefs.boxes[3]->text()+"/"+myprefs.boxes[0]->text()+".docset/Contents/Resources");
			exit=sqlite3_open(QString("%1/%2.docset/%3").arg(myprefs.boxes[3]->text()).arg(myprefs.boxes[0]->text()).arg("Contents/Resources/docSet.dsidx").toStdString().c_str(),&DB);
			if(exit!=SQLITE_OK)
				{ 
					std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl; 
					return; 
				}

			sql="CREATE TABLE searchIndex(id INTEGER PRIMARY KEY, name TEXT, type TEXT, path TEXT);CREATE UNIQUE INDEX anchor ON searchIndex (name, type, path);";
			exit=sqlite3_exec(DB,sql.toStdString().c_str(),NULL,0,&messaggeError); 

			if(exit != SQLITE_OK)
				{ 
					std::cerr << "Error Creating Table" << messaggeError << std::endl; 
					sqlite3_free(messaggeError); 
				} 
 
			sqlite3_exec(DB,"BEGIN TRANSACTION",NULL,NULL,&messaggeError);
			this->runNoOutput(QString("cp -rp %1 %2").arg(this->tmpFolderName+"/html/").arg(myprefs.boxes[3]->text()+"/"+myprefs.boxes[0]->text()+".docset/Contents/Resources/Documents"));
			this->runNoOutput(QString("cp %1/html/Info.plist %2").arg(this->tmpFolderName).arg(myprefs.boxes[3]->text()+"/"+myprefs.boxes[0]->text()+".docset/Contents/"));
			this->runNoOutput(QString("convert %1 -resize 16x16 %2/icon.png").arg(myprefs.boxes[2]->text()).arg(myprefs.boxes[3]->text()+"/"+myprefs.boxes[0]->text()+".docset"));
			this->runNoOutput(QString("convert %1 -resize 32x32 %2/icon@2x.png").arg(myprefs.boxes[2]->text()).arg(myprefs.boxes[3]->text()+"/"+myprefs.boxes[0]->text()+".docset"));
			fp=popen("kkeditqttagreader Tokens.xml Token Name Type Path Anchor","r");
			if(fp!=NULL)
				{
					while(fgets(line,4095,fp))
						{
							line[strlen(line)-1]=0;
							thename=QString(line).section(' ',0,0).section('=',1,1);
							thetype=QString(line).section(' ',1,1).section('=',1,1);
							thepath=QString(line).section(' ',2,2).section('=',1,1);
							theanchor=QString(line).section(' ',3,3).section('=',1,1);
							if((thename.isEmpty()==true) || (thepath.isEmpty()==true))
								continue;
							if(thename.length()<NAME_MAX)
								{
									this->runNoOutput(QString("echo -n \"%1 %3 ...\n0\" >\"%2/progress\"").arg("Adding").arg(this->tmpFolderName).arg(thename));
									if(theanchor.isEmpty()==false)
										thepath=thepath+"#"+theanchor;
									sql="insert into searchindex (name,type,path) values ('"+thename+"','"+thetype+"','"+thepath+"');";
									exit=sqlite3_exec(DB,sql.toStdString().c_str(),NULL,0,&messaggeError);
									if(exit != SQLITE_OK)
										{ 
											std::cerr << "Error Insert: " << messaggeError<<std::endl; 
											sqlite3_free(messaggeError); 
										}
								}
						}
					pclose(fp);
				}

			sqlite3_exec(DB,"END TRANSACTION",NULL,NULL,&messaggeError);
			sqlite3_close(DB); 

			QDir::setCurrent(currentdir.canonicalPath());

			this->runNoOutput(QString("echo -e \"quit\n100\">\"%1/progress\"").arg(this->tmpFolderName));
		}
	delete myprefs.theDialog;
}

void KKEditClass::buildDocs(void)
{
	DocumentClass			*doc=this->getDocumentForTab(-1);
	FILE						*fp;
	char						line[4096];
	QDir						currentdir;
	bool						gotdoxyfile;
	QSettings				buildDocsPrefs("KDHedger","DocsPrefs");
	miniPrefsReturnStruct	myprefs;
	QString					projectname;
	QString					versionbox;

	if(doc==NULL)
		return;

	currentdir=QDir::current();
	QDir::setCurrent(doc->getDirPath());

	if(QFile::exists(doc->getDirPath()+"/Doxyfile"))
		{
			gotdoxyfile=true;
		}
	else
		{
			QProcess::execute("cp",QStringList()<<DATADIR "/docs/Doxyfile"<<doc->getDirPath());
			gotdoxyfile=false;
		}


	if(gotdoxyfile==false)
		{
			projectname=buildDocsPrefs.value("ProjectName","MyProject").toString().simplified();
			versionbox=buildDocsPrefs.value("ProjectVersion","0.0.1").toString().simplified();
		}
	else
		{
			projectname=runPipeAndCapture(QString("sed -n 's/PROJECT_NAME=\\(.*\\)/\\1/p' %1").arg(doc->getDirPath()+"/Doxyfile")).simplified();
			versionbox=runPipeAndCapture(QString("sed -n 's/PROJECT_NUMBER=\\(.*\\)/\\1/p' %1").arg(doc->getDirPath()+"/Doxyfile")).simplified();	
		}


	myprefs=this->miniPrefsDialog("DocsPrefs",QStringList()<<"Project Name"<<"Project Version");
	myprefs.boxes[0]->setText(projectname);
	myprefs.boxes[1]->setText(versionbox);

	int res=myprefs.theDialog->exec();	
	if(res==1)
		{
			QString com=QString("sed -i 's|^PROJECT_NAME=.*$|PROJECT_NAME=%1|;s|^PROJECT_NUMBER=.*$|PROJECT_NUMBER=%2|;s|^GENERATE_DOCSET=.*$|GENERATE_DOCSET=YES|;s|^SERVER_BASED_SEARCH=.*$|SERVER_BASED_SEARCH=NO|' '%3'").arg(myprefs.boxes[0]->text()).arg(myprefs.boxes[1]->text()).arg("Doxyfile");
			runPipeAndCapture(com);
			delete myprefs.theDialog;
		}
	else
		{
			delete myprefs.theDialog;
			return;
		}

	this->showBarberPole("Building Docs","Please Wait","","0",QString("%1/progress").arg(this->tmpFolderName));
	this->runNoOutput(QString("echo -n \"%1\n0\" >\"%2/progress\"").arg("Building Docs, Please Wait...").arg(this->tmpFolderName));
	fp=popen("doxygen Doxyfile","r");
	if(fp!=NULL)
		{
			while(fgets(line,4095,fp))
				{
					line[strlen(line)-1]=0;
					this->runNoOutput(QString("echo -n \"%1\n0\" >\"%2/progress\"").arg(line).arg(this->tmpFolderName));
				}
			pclose(fp);
		}
	else
		{
			this->runNoOutput(QString("echo -e \"quit\n100\">\"%1/progress\"").arg(this->tmpFolderName));
			return;
		}		

	this->runNoOutput(QString("echo -e \"quit\n100\">\"%1/progress\"").arg(this->tmpFolderName));
	QDir::setCurrent(currentdir.canonicalPath());
	this->showDocs();
}

void KKEditClass::showDocs(void)
{
	DocumentClass	*doc=this->getDocumentForTab(-1);

	if(doc==NULL)
		return;

	QFileInfo		fileinfo(QString("%1/html/index.html").arg(doc->getDirPath()));

	if(fileinfo.exists()==false)
		this->buildDocs();
	else
		{
			this->showWebPage("Doxygen Documentation",QString("file://%1/html/index.html").arg(doc->getDirPath()));
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
	this->writeExitData();

	if(this->onExitSaveSession==true)
		this->doSessionsMenuItems();

	if(this->saveAllFiles(true)==true)
		{
#ifdef _ASPELL_
			delete_aspell_config(this->aspellConfig);
			delete_aspell_speller(this->spellChecker);
#endif
		}
					plugData pd;
					pd.what=DOSHUTDOWN;
					this->runAllPlugs(pd);
			QApplication::quit();
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

bool KKEditClass::checkShortCuts(void)
{
	bool retval=true;

	for(int k=0;k<defaultShortCutsList.size();k++)
		{
			for(int i=0;i<reservedShortcutKeys.size();i++)
				{
					if(LFSTK_UtilityClass::LFSTK_strStr(defaultShortCutsList.at(k).toStdString(),reservedShortcutKeys.at(i).toStdString(),true).empty()==false)
						{
							if(prefsWindow!=NULL)
								{
									QMessageBox msgBox(QMessageBox::Warning,"Reserved Shortcut",QString("Warning! %1 is a reserved shortcut").arg(defaultShortCutsList.at(k)));
									msgBox.exec();
								}
							else
								{
									qDebug()<<"Warning"<<defaultShortCutsList.at(k)<<"in use";
								}
							this->defaultShortCutsList[k]=replacementShorts[k];
							retval=false;
							if(prefsWindow!=NULL)
								this->resetKeyCombo();
						}
				}
		}
	return(retval);
}

void KKEditClass::setAppShortcuts(void)
{
	if(this->checkShortCuts()==false)
		return;

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
						this->goToDefineMenuSingleItem->setEnabled(hasselection);
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
	pd.doc=doc;
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
				//qDebug()<<"session"<<opensessionname<<quid<<message.mText;
				}

			pathtopwd=get_current_dir_name();
			list=this->parser.positionalArguments();
			for(int j=0;j<list.count();j++)
				{
					if(list.at(j).at(0)!='/')
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

	this->webEngView->load(QUrl::fromUserInput(url));
	this->docView->setWindowState(Qt::WindowNoState);//TODO//doesnt work
	this->setDocMenu();
	this->docView->show();
	this->docView->activateWindow();
	this->docView->raise();

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

	if(this->mainNotebook->count()==0)
		return;

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
	QDirIterator lit(pluginsDir.canonicalPath() ,QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);
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
	QDirIterator git(pluginsDir.canonicalPath(),QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);
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
	if(doc==NULL)
		return;

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

void KKEditClass::setMouseState(bool mouseon)
{
	if((mouseon==false) && (this->mouseVisible==true))
		{
			this->application->setOverrideCursor(QCursor(Qt::BlankCursor));
			this->mouseVisible=false;
			return;
		}

	if((mouseon==true) && (this->mouseVisible==false))
		{
			this->application->restoreOverrideCursor();
			this->mouseVisible=true;
		}
}