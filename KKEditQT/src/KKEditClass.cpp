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

#include "MainWindow.h"
#include "docBrowser.h"
#include "QT_notebook.h"
#include "QT_toolbar.h"
#include "QT_recentMenu.h"
#include "QT_historyClass.h"
#include "QT_menuitem.h"
#include "QT_document.h"
#include "QT_ProxyStyle.h"
#include "QT_themeClass.h"
#include "QT_highlighter.h"
#include "KKEditClass.h"
#include "ChooserDialog.h"

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
	if(this->tmpFolderName.isEmpty()==true)
		return;

	delete this->fileWatch;

	for(int j=0;j<NOMORESHORTCUT;j++)
		delete this->appShortcuts[j];

#ifdef _ASPELL_
	if(this->aspellConfig!=NULL)
		delete_aspell_config(this->aspellConfig);
	if(this->spellChecker!=NULL)
		delete_aspell_speller(this->spellChecker);
#endif

	for(int j=0;j<this->plugins.count();j++)
		{
			if(this->plugins[j].loaded==true)
			{
				this->unloadPlug(&this->plugins[j]);
			//if(this->plugins[j].instance!=NULL)
			//	delete this->plugins[j].instance;
			//if(this->plugins[j].pluginLoader!=NULL)
			//	delete this->plugins[j].pluginLoader;
				}
		}

	system(QString("rm %1/* 2>/dev/null").arg(this->tmpFolderName).toStdString().c_str());/**/
	system(QString("rmdir %1 2>/dev/null").arg(this->tmpFolderName).toStdString().c_str());

	if(this->docView!=NULL)
		delete this->docView;

	delete this->menuBar;
	delete this->toolBar;
	delete this->mainNotebook;

	for(int j=0;j<this->pages.size();j++)
		delete this->pages[j];

	delete this->toolsWindow;
	delete this->goToDefineMenuSingleItem;
	delete this->prefsWindow;
	delete this->checkMessages;
	for(int j=0;j<20;j++)
		if(this->tool[j]!=NULL)
			delete this->tool[j];
	delete this->toolsOPText;
	delete this->toolOutputWindow;
	delete this->findReplaceDialog;
	delete this->spellCheckGUI;
	delete this->spellCheckMenuItem;

	delete this->recentFiles;
	delete this->history;
	delete this->theme;
}

void KKEditClass::handleSignal(int signum)
{
	switch(signum)
		{
			case SIGUSR1:
				//this->checkMessages->stop();
				this->doTimer();
				//this->checkMessages->start();
				break;
			case SIGTERM:
			case SIGINT:
				this->shutDownApp();
				break;
			default:
				break;
		}
}

void KKEditClass::activateMainWindow(void)
{
	this->mainWindow->show();
	this->mainWindow->activateWindow();
	this->mainWindow->raise();
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
							QPushButton *recent=new QPushButton(NULL,this->toolBar);
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
						this->lineNumberWidget=new QLineEdit(this->toolBar);
						this->lineNumberWidget->setValidator(new QIntValidator(this->lineNumberWidget));
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
						this->findDefWidget=new QLineEdit(this->toolBar);
						this->findDefWidget->setObjectName(QString("%1").arg(DOAPISEARCH));
						this->findDefWidget->setToolTip("Search For Define");
						
						
						QObject::connect(this->findDefWidget,&QLineEdit::returnPressed,[this]()
							{
								this->doOddButtons(DOAPISEARCH);
							});

						this->toolBar->addWidget(this->findDefWidget);
						break;
//livesearch
					case 'L':
						if(this->liveSearchWidget!=NULL)
							delete this->liveSearchWidget;
						this->liveSearchWidget=new QLineEdit(this->toolBar);
						this->liveSearchWidget->setToolTip("Live Search");
						this->liveSearchWidget->setObjectName(QString("%1").arg(DOLIVESEARCH));
						
						QObject::connect(this->liveSearchWidget,&QLineEdit::textChanged,[this](QString text)
							{
								this->doLiveSearch(text);
							});
						
						QObject::connect(this->liveSearchWidget,&QLineEdit::returnPressed,[this]()
							{
								this->doOddButtons(DOLIVESEARCH);
							});
						this->toolBar->addWidget(this->liveSearchWidget);
						break;

//expander
					case 'E':
						{
							QWidget		*widg=new QWidget(this->toolBar);
							QHBoxLayout *hbox=new QHBoxLayout(widg);
    							hbox->addStretch(1);
    							widg->setLayout(hbox);
    							this->toolBar->addWidget(widg);

//							QHBoxLayout hbox;
//							QWidget		*widg=new QWidget(this->toolBar);
//    							hbox.addStretch(1);
//    							widg->setLayout(hbox);
//    							this->toolBar->addWidget(widg);
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

	//doc=qobject_cast<DocumentClass*>(this->mainNotebook->widget(index));
	doc=(DocumentClass*)this->mainNotebook->widget(index);
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
	DocumentClass	*tdoc=(DocumentClass*)(widget);
	DocumentClass	*doc=this->pages.value(tdoc->pageIndex);
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

//#include <QEvent>
//
//class EventFilter1 : public QObject
//{
//protected:
//    bool eventFilter(QObject * obj, QEvent * event) override
//    {
//    qDebug()<<"closeing ...";
// //       if (event->type() == QEvent::Close)
////        {
////            qDebug() << obj->objectName()<<"closing";
////        }
//
//        return QObject::eventFilter(obj, event);
//    }
//};

void KKEditClass::initApp(int argc,char** argv)
{
	char		tmpfoldertemplate[]="/tmp/KKEditQT-XXXXXX";
	QRect	r(0,0,1024,768);
	QDir		tdir;
	QString	tstr;
	QFile	file;
	QString	s="";

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
	this->maxSessions=this->prefs.value("app/maxsessions",24).toInt();
	for(int j=0;j<this->maxSessions;j++)
		{
			this->runPipeAndCapture(QString("touch '%1/Session-%2'").arg(this->sessionFolder).arg(j));
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
		if(this->tmpFolderName.isEmpty()==true)
			{
				qDebug()<<"Can't create temporary folder, quitting ...";
				exit (100);
			}

	this->gotManEditor=QStandardPaths::findExecutable("manpageeditorqt").isEmpty();
	this->gotPDFToText=QStandardPaths::findExecutable("pdftotext").isEmpty();
	this->gotPDFCrop=QStandardPaths::findExecutable("pdfcrop").isEmpty();

	this->mainThemeProxy=new ProxyStyle();
// take ownership to avoid memleak
	this->mainThemeProxy->setParent(this->application);
	this->application->setStyle(this->mainThemeProxy);

//	if(getuid()!=0)
//		styleName="classic";
//	else
//		styleName="Root Source";

//
	this->mainWindow=new MainWindowClass(this);

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
	QObject::connect(this->checkMessages,&QTimer::timeout,[this]()
		{
			this->doTimer();
		});

	//////////this->checkMessages->start(this->prefsMsgTimer);
	this->theme->loadTheme(this->prefStyleName);
	this->buildMainGui();
	this->buildPrefsWindow();
	this->buildToolOutputWindow();
	//this->loadPlugins();

	this->buildDocViewer();
	this->buildFindReplace();

#ifdef _ASPELL_
	AspellCanHaveError	*possible_err;
	this->aspellConfig=new_aspell_config();
	possible_err=new_aspell_speller(this->aspellConfig);

	if(aspell_error_number(possible_err)!= 0)
		puts(aspell_error_message(possible_err));
	else
		this->spellChecker=to_aspell_speller(possible_err);

	this->spellCheckMenuItem=new MenuItemClass("Spell Check");
	QIcon	itemicon=QIcon::fromTheme("tools-check-spelling");
	this->spellCheckMenuItem->setMenuID(SPELLCHECKMENUITEM);
	this->spellCheckMenuItem->setIcon(itemicon);

	QObject::connect(this->spellCheckMenuItem,&QAction::triggered,[this]()
		{
			this->doOddMenuItems(this->spellCheckMenuItem);
		});
	this->buildSpellCheckerGUI();
#endif

	this->htmlFile=QString("%1/Docview-%2.html").arg(this->tmpFolderName).arg(this->randomName(6));
	this->htmlURI="file://"+this->htmlFile;

	this->recentFiles->updateRecents();

	if(this->forceDefaultGeom==false)
		r=this->prefs.value("app/geometry",QVariant(QRect(50,50,1024,768))).value<QRect>();

	this->mainWindow->setGeometry(r);
 
//this->onExitSaveSession //TODO//
	this->mainWindow->show();
	this->loadPlugins();

	plugData		pd;
	pd.userStrData1=this->tmpFolderName;
	pd.userIntData1=this->sessionID;
	pd.what=DOPOSTLOAD;
	this->runAllPlugs(pd);

	this->setToolbarSensitive();
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
	this->maxSessions=this->prefs.value("app/maxsessions",24).toInt();
	this->autoOneTab=this->prefs.value("app/autoonetab",QVariant(bool(false))).value<bool>();
	this->openTabToRight=this->prefs.value("app/opentabtoright",QVariant(bool(true))).value<bool>();
	this->openFirstTabWithSession=this->prefs.value("app/selectfirsttab",QVariant(bool(true))).value<bool>();

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
											
											QObject::connect(menuitem1,&QAction::triggered,[this,menuitem1]()
												{
													this->doTabBarContextMenu(menuitem1);
												});
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
					
					QObject::connect(menuitem,&QAction::triggered,[this,menuitem]()
						{
							this->doTabBarContextMenu(menuitem);
						});
				}
//plugins
//this->saved_stdout = dup(fileno(stdout));
			pd.menu=&menu;
			pd.tabNumber=tabIndex;
			pd.userStrData1=this->homeDataFolder;
			pd.userStrData2=this->getDocumentForTab(tabIndex)->getDirPath();
			pd.userStrData3=this->getDocumentForTab(tabIndex)->getFileName();
			pd.what=DOTABPOPUP;
			this->runAllPlugs(pd);
			menu.exec(this->mainNotebook->mapToGlobal(pt));
//dup2(this->saved_stdout, fileno(stdout));
 //   close(this->saved_stdout); 
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
	this->prefs.setValue("app/maxsessions",this->maxSessions);
	this->prefs.setValue("app/autoonetab",this->autoOneTab);
	this->prefs.setValue("app/opentabtoright",this->openTabToRight);
	this->prefs.setValue("app/selectfirsttab",this->openFirstTabWithSession);

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
	DocumentClass		*document=this->getDocumentForTab(-1);
	QString				selection;
	QString				filename;
	QString				results;
	QStringList			retval;

	if(document==NULL)
		return;

	selection=document->textCursor().block().text().trimmed();

	if((selection.isEmpty()==true) || (selection.startsWith('#')==false))
		return;

	filename=selection.replace(QRegularExpression(R"RX(#.*include\s*[\"<](.*)[\">].*)RX"),"\\1").trimmed();

	if(filename.startsWith('/'))
		if(this->openFile(filename)==true)
			return;

	if(this->openFile(QString("%1/%2").arg(document->getDirPath()).arg(filename))==true)
		return;

	results=this->runPipeAndCapture(QString("find \"/usr/include\" -path \"*%1\"").arg(filename));
	if(results.isEmpty()==false)
		{
			retval=results.split("\n",Qt::SkipEmptyParts);
			for(int j=0;j<retval.count();j++)
				this->openFile(retval.at(j));
		}

	results=this->runPipeAndCapture(QString("find \"/usr/local/include\" -path \"*%1\"").arg(filename));
	if(results.isEmpty()==false)
		{
			retval=results.split("\n",Qt::SkipEmptyParts);
			for(int j=0;j<retval.count();j++)
				this->openFile(retval.at(j));
		}
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
	int				ci=this->mainNotebook->currentIndex()-1;

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

	//doc=qobject_cast<DocumentClass*>(this->mainNotebook->widget(thispage));
	doc=(DocumentClass*)this->mainNotebook->widget(thispage);
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

	this->mainNotebook->setCurrentIndex(ci);
	return(true);
}

void KKEditClass::shutDownApp()
{
	this->writeExitData();

	if(this->onExitSaveSession==true)
		this->doSessionsMenuItems(NULL);

	if(this->saveAllFiles(true)==true)
		{
//#ifdef _ASPELL_
//			delete_aspell_config(this->aspellConfig);
//			delete_aspell_speller(this->spellChecker);
//#endif
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
					
					QObject::connect(this->appShortcuts[j],&QShortcut::activated,[this,j]()
						{
							this->doAppShortCuts(this->appShortcuts[j]);
						});
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
			if(doc->verticalSelectMatch.size()>0)
				hasselection=true;
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

	for(int j=0;j<this->mainNotebook->count();j++)
		{
			doc=this->getDocumentForTab(j);
			this->mainNotebook->setTabToolTip(j,doc->getFilePath());
		}
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
				//system(qPrintable(QString("echo \"j=%1 -- --->>>%2<<<--\n\" >> /tmp/log").arg(j).arg(list.at(j))));
					if(list.at(j).at(0)!='/')
						msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s/%s",pathtopwd,list.at(j).toStdString().c_str());
					else
						msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s",list.at(j).toStdString().c_str());
					message.mType=OPENFILEMSG;
					msgsnd(quid,&message,msglen,0);
				}

			if(pathtopwd!=NULL)
				free(pathtopwd);
		}
}

void KKEditClass::setDocMenu(void)
{
	if((this->docView!=NULL) && (this->docView->winWidget!=NULL))
		{
			if(this->docView->winWidget->isVisible()==true)//ugly hack!!//
				{
					this->toggleDocViewMenuItem->setText("Hide Docviewer");
					this->docviewerVisible=true;
				}
			else
				{
					this->toggleDocViewMenuItem->setText("Show Docviewer");
					this->docviewerVisible=false;
				}
		}
}

void KKEditClass::showWebPage(QString windowtitle,QString url)
{
	this->docView->windowTitle=windowtitle;
	this->docView->createNewWindow(url);
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
					pd.tempFolder=this->tmpFolderName;
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

