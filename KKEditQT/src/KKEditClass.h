/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:40:06 GMT 2021 keithdhedger@gmail.com

 * This file (KKEditClass.h) is part of KKEditQT.

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

#ifndef _KKEDITCLASS_
#define _KKEDITCLASS_

#define STATUSBARTIMEOUT 5000

enum {FILEMENU=0x4000,EDITMENU,VIEWMENU,NAVMENU,FUNCTIONSMENU,BOOKMARKSMENU,TOOLSMENU,PLUGINSMENU,HELPMENU,SAVESESSIONSMENU,RESTORESESSIONSMENU,CURRENTSESSION,NOMENU};

//file
enum {NEWMENUITEM=0x8000,OPENMENUITEM,HEXDUMPMENUITEM,NEWADMINEDMENUITEM,NEWEDMENUITEM,MANPAGEEDMENUITEM,BUILDDOCSMENUITEM,SAVEMENUITEM,SAVEASMENUITEM,SAVEALLMENUITEM,SAVESESSIONMENUITEM,RESTORESESSIONMENUITEM,PRINTMENUITEM,CLOSEMENUITEM,CLOSEALLMENUITEM,REVERTMENUITEM,QUITMENUITEM};
//edit
enum {UNDOMENUITEM=0x9000,REDOMENUITEM,UNDOALLMENUITEM,REDOALLMENUITEM,EDSEP1,CUTMENUITEM,COPYMENUITEM,PASTEMENUITEM,EDSEP2,FINDMENUITEM,FINDNEXTMENUITEM,EDSEP3,SORTTABSMENUITEM,SHOWALLTABSMENUITEM,SELECTTABMENUITEM,EDSEP4,PREFSMENUITEM,PLUGPREFSMENUITEM};
//view
enum {DOCSMENUITEM=0xa000,TOGGLETOOLBARMENUITEM,TOGGLETOOLWINDOWMENUITEM,TOGGLESTATUSBARMENUITEM,TOGGLEDOCVIEWMENUITEM,TOGGLELINENUMBERSMENUITEM,TOGGLELINEWRAPMENUITEM,TOGGLEHILITELINEMENUITEM,TOGGLEWHITESPACEMENUITEM,TOGGLESYNTAXHILITINGMENUITEM,TOGGLECOMPLETIONSMENUITEM};
//nav
enum {GOTODEFINEMENUITEM=0xb000,OPENINCLUDEMENUITEM,GOTOLINEMENUITEM,SEARCHFORDEFINEMENUITEM,SEARCHGTKDOCS,SEARCHQT5DOCS,SEARCHDOXYDOCS,GOBACKMENUITEM,GOFORWARDMENUITEM};
//func
//bms
enum {REMOVEALLBOOKMARKSMENUITEM=0xc000,TOGGLEBOOKMARKMENUITEM,REMOVEBOOKMARKSFROMPAGE};

//tools
enum {MANAGETOOLSMENUITEM=0xd000,TOOLNUMBER};
//plugins
//help
enum {ABOUTMENUITEM=0xe000,ABOUTQTMENUITEM,HELPMENUITEM};

enum	{AUTOINDENT=0,SHOWNUMS,WRAP,HIGHLIGHT,SYNTAXHILITE,USESINGLE,AUTOSAVE,NODUPLICATE,NOWARN,AUTOSHOW,BEKIND,SETFONT,MAXPREFSWIDGETS};
enum {MAXTABCHARS=0,MAXHISTORY,MAXFUNCDEPTH,COMPLETIONSIZE,TABWIDTH,MENUWIDTH,MAXBMWIDTH,MAXRECENTS,MAXPREFSINTWIDGETS};
enum {FUNCTIONCOMBO=0,THEMECOMBO,FONTNAMECOMBO,FONTSIZECOMBO,PREFSTERMCOMMAND,PREFSROOTCOMMAND,PREFSQTDOCDIR,PREFSBROWSERCOMMAND,PREFSCURRENTFONT,BMHIGHLIGHTCOLOUR,CURRENTLINECOLOUR,SHORTCUTSCOMBO,MAXPREFSOTHERWIDGETS};

enum {FINDNEXT=1,FINDPREV,FINDREPLACE};

//od enums
enum {SPELLCHECKMENUITEM=0x2000,APPLYWORDBUTTON,IGNOREWORDBUTTON,ADDWORDBUTTON,CANCELSPELLCHECK,CANCELPREFS,DOLINEBOX,DOLIVESEARCH,DOAPISEARCH,DOQT5SEARCH,DOGTKSEARCH,DOCVIEWERGOHOME,TOOLSSAVE,TOOLSCANCEL,TOOLSDELETE,TOOLSSAVEAS,TOOLSEDIT};

struct tabMenuStruct
{
	unsigned int	what;
	const char		*label;
	const char		*icon;
};

enum {COPYFOLDERPATH=0xf000,COPYFILEPATH=0xf100,COPYFILENAME=0xf200,SPELLCHECKDOC=0xf300,SRCHILTIE=0xf400,HIDETAB=0xf500,LOCKCONTENTS=0xf600,OPENFROMHERE=0xf700};

enum {TABCONTEXTMENUCNT=(OPENFROMHERE-COPYFOLDERPATH) / 0x100 +1};

enum {NONESRCCODE=0,CPPSRCCODE=0x100,CSRCCODE=0x200,BASHSRCCODE=0x300,PYTHONSRCCODE=0x400,GOSRCCODE=0x500,LUASRCCODE=0x600,YAMLSRCCODE=0x700,PHPSRCCODE=0x800,XMLSRCCODE=0x900,CSSSRCCODE=0xa00,JSSRCCODE=0xb00,MAKESRCCODE=0xc00};

enum {HIDETABSHORTCUT=0,DELETELINESHORTCUT,DELETETOEOLSHORTCUT,DELETETOSOLSHORTCUT,SELECTWORDSHORTCUT,DELETEWORDSHORTCUT,DUPLICATELINESHORTCUT,SELECTLINESHORTCUT,MOVELINEUPSHORTCUT,MOVELINEDOWNSHORTCUT,MOVESELECTIONUPSHORTCUT,MOVESELECTIONDOWNSHORTCUT,NOMORESHORTCUT};

enum {FRCASE=0,FRUSEREGEX,FRWRAP,FRALLFILES,FRHIGHLIGHTALL,FRREPLACEALL,FRSEARCHBACK,FRMAXSWITCHES};

#include "QT_AboutBox.h"
#include "QT_menuitem.h"
#include "QT_historyClass.h"
#include "QT_recentMenu.h"
#include "QT_highlighter.h"
#include "QT_document.h"
#include "QT_toolbar.h"
#include "QT_notebook.h"

#include "kkeditqtPluginInterface.h"

class NoteBookClass;
class MenuItemClass;
class RecentMenuClass;
class HistoryClass;
class ToolBarClass;
class kkEditQTPluginInterface;

struct pluginStruct
{
	QPluginLoader					*pluginLoader=NULL;
	QString							plugPath="";
	QString							plugName="";
	QString							plugVersion="";
	kkEditQTPluginInterface			*instance=NULL;
	unsigned int						wants=DONONE;
	bool								loaded=false;
	bool								statusChanged=false;
	bool								broken=false;
};

class KKEditClass : public QObject
{
	Q_OBJECT

	public:
		KKEditClass(QApplication *app);
		~KKEditClass();

//app vars
		QMainWindow					*mainWindow;
		NoteBookClass				*mainNotebook=NULL;
		QApplication					*application;
		QTimer 						*checkMessages=NULL;
		QString						tmpFolderName;
		QString						htmlFile;
		QString						htmlURI;
		QSettings					prefs;
		int							gotManEditor;
		int							gotDoxygen;
		QHash<int,DocumentClass*>	pages;
		int							newPageIndex=1;
		int							sessionID=-1;
		int							queueID=-1;
		int							currentWorkSpace=-1;
		bool							forcedMultInst=false;
		bool							forceDefaultGeom=false;
		bool							sessionBusy=false;
		tabMenuStruct				tabContextMenuItems[TABCONTEXTMENUCNT]={{COPYFOLDERPATH,"Copy Folder Path","edit-copy"},{COPYFILEPATH,"Copy File Path","edit-copy"},{COPYFILENAME,"Copy File Name","edit-copy"},{SPELLCHECKDOC,"Spellcheck Document","tools-check-spelling"},{SRCHILTIE,"Source Hilighting","text-x-script"},{HIDETAB,"Hide Tab","list-remove"},{LOCKCONTENTS,"Lock Contents","emblem-readonly"},{OPENFROMHERE,"Open From Here","document-open"}};
		const char					*srcMenuNames[20]={"None","C++","C","SH","Python","Go","Lua","YAML","PHP","XML","CSS","Javascript","Make",NULL};
#ifdef _ASPELL_
		AspellConfig					*aspellConfig=NULL;
		AspellSpeller				*spellChecker=0;
		QDialog						*spellCheckGUI=NULL;
		QComboBox					*wordDropBox;
		QLabel						*infoLabel;
		MenuItemClass				*spellCheckMenuItem;
		bool							returnWord=false;
		QString						goodWord;
		QString						badWord;
		bool							cancelCheck=false;
#endif
		QShortcut					*appShortcuts[NOMORESHORTCUT];
		QStringList					defaultShortCutsList={"Ctrl+H","Ctrl+Y","Ctrl+?","Ctrl+K","Ctrl+Shift+H","Ctrl+D","Ctrl+Shift+D","Ctrl+L","Ctrl+M","Ctrl+Shift+M","Ctrl+@","Ctrl+'"};
//recent files
		RecentMenuClass				*recentFiles=NULL;

		//sesions
		QHash<int,QString>			sessionNames;
		QString						homeFolder;
		QString						homeDataFolder;
		QString						sessionFolder;
		unsigned int					currentSessionNumber=0xdeadbeef;
		QString						toolsFolder;

//app functions
		void							initApp(int argc,char** argv);
		void							buildMainGui(void);
		void							readConfigs(void);
		void							findFile(void);
		void							buildTools(void);
		void							showBarberPole(QString windowtitle,QString bodylabel,QString cancellabel,QString controlfile);
		void							buildDocs(void);
		void							showDocs(void);
		void							shutDownApp(void);

		void							buildSpellCheckerGUI(void);
		void							setUpSpellGUI(QString word,DocumentClass *doc);
		bool							checkSelection(QString selection);
		void							checkDoc(DocumentClass *doc);
		void							setAppShortcuts(void);

		void							setToolbarSensitive(void);
		QString						randomName(int len);

		void							writeExitData(void);
		int							yesNoDialog(QString txt,QString info);

//webpage
		void							showWebPage(QString windowtitle,QString url);
//scripting
		void							runCLICommands(int quid);
		QCommandLineParser			parser;

//app prefs
//document
		bool							prefsIndent=true;
		bool							prefsShowLineNumbers=true;
		bool							prefsLineWrap=true;
		bool							prefsHighLightline=true;
		bool							prefsSyntaxHilighting=true;
		QFont						prefsDocumentFont;
		unsigned int					prefsTabWidth=4;
		bool							prefsAutoShowCompletions;
//theme
		QColor						prefsHiLiteLineColor;
		QColor						prefsBookmarkHiLiteColor;
		QString						prefStyleName;
		QString						prefStyleNameHold;
//editor
		int							prefsFunctionMenuLayout=0;
		unsigned int					prefsDepth=1;
		QString						prefsToolBarLayout;
		unsigned int					prefsMaxTabChars=20;
		unsigned int					prefsMaxFuncChars=MAXTEXTWIDTH;
		QString						prefsTerminalCommand;
		QString						prefsRootCommand;
		QString						prefsQtDocDir;
		bool							prefsNoOpenduplicate=false;
//app
		int							prefsMsgTimer;
		bool							prefsUseSingle=true;
		bool							prefsNagScreen=false;
		bool							onExitSaveSession=false;

//editor vars
		QStatusBar					*statusBar;
		QLabel						*statusText;
		bool							wrapSearch=true;
		bool							insensitiveSearch=true;
		bool							useRegex=false;
		bool							replaceAll=false;
		bool							findInAllFiles=false;
		bool							hightlightAll=true;
		bool							searchBack=false;
		int							currentPage=0;
		bool							closingAllTabs=false;
		bool							noWarnings=false;
		unsigned int					autoShowMinChars=6;
		unsigned int					maxBMChars;
		unsigned int 				untitledNumber=1;
		HistoryClass					*history;

//editor functions
		QString						truncateWithElipses(const QString str,unsigned int maxlen);
		void							sortTabs(void);
		void							rebuildTabsMenu(void);
		void							functionSearchDialog(void);
		bool							checkForOpenFile(QString filepath);
		void							printDocument(void);
		void							rebuildFunctionMenu(int tab);

//menubar
		QMenuBar						*menuBar;

//menu vars
//file menu
		QMenu						*fileMenu;
		QMenu						*saveSessionsMenu;
		QMenu						*restoreSessionsMenu;
		MenuItemClass				*saveCurrentSessionMenuItem;
		MenuItemClass				*restoreDefaultSessionMenuItem;
		QList<MenuItemClass*>		restoreSessionMenuItemsList;


		MenuItemClass				*newMenuItem;
		MenuItemClass				*openMenuItem;
		MenuItemClass				*openRecentMenuItem;
		MenuItemClass				*saveMenuItem;
		MenuItemClass				*saveAsMenuItem;
		MenuItemClass				*saveAllMenuItem;
		MenuItemClass				*printMenuItem;
		MenuItemClass				*closeMenuItem;
		MenuItemClass				*closeAllMenuItem;
		MenuItemClass				*revertMenuItem;

//edit menu
		QMenu						*editMenu;
		MenuItemClass				*undoMenuItem;
		MenuItemClass				*redoMenuItem;
		MenuItemClass				*undoAllMenuItem;
		MenuItemClass				*redoAllMenuItem;
		MenuItemClass				*cutMenuItem;
		MenuItemClass				*copyMenuItem;
		MenuItemClass				*pasteMenuItem;
		MenuItemClass				*findMenuItem;
		MenuItemClass				*findNextMenuItem;
		MenuItemClass				*sortTabsMenuItem;
		MenuItemClass				*showAllTabsMenuItem;
		QMenu						*selectTabMenu;

//view menu
		QMenu						*viewMenu;
		bool							toolbarVisible=true;
		bool							toolWindowVisible=false;
		bool							statusBarVisible=true;
		bool							lineNumbersVisible;
		bool							wrapLine;
		bool							hiliteCurrentLine;
		bool							showWhiteSpace=false;
		bool							showHighLighting;
		bool							showCompletions;
		MenuItemClass				*toggleLineNumbersMenuItem;
		MenuItemClass				*toggleWrapLineMenuItem;
		MenuItemClass				*toggleHiliteCurrentLineMenuItem;
		MenuItemClass				*toggleWhiteSpaceMenuItem;
		MenuItemClass				*toggleSyntaxHilightingMenuItem;
		MenuItemClass				*toggleCompletionsMenuItem;
		MenuItemClass				*toggleToolBarMenuItem;
		MenuItemClass				*toggleToolWindowMenuItem;
		MenuItemClass				*toggleStatusBarMenuItem;

//nav menu
		QMenu						*navMenu;
		MenuItemClass				*goToDefineMenuItem;
		MenuItemClass				*goBackMenu;
		MenuItemClass				*goFowardMenu;
		MenuItemClass				*goToLineDialogMenuItem;

//function menu
		QMenu						*funcMenu;

//bookmarks menu
		QMenu						*bookMarkMenu;

//help menu
		QMenu						*helpMenu;

//menu functions
		MenuItemClass				*makeMenuItemClass(int mainmenu,const QString name,const QKeySequence key,const QString iconname,const QString objectname,int userdata);

//toolbar vars
		ToolBarClass					*toolBar=NULL;
		QLineEdit					*lineNumberWidget=NULL;
		QLineEdit					*findGtkApiWidget=NULL;
		QLineEdit					*findQtApiWidget=NULL;
		QLineEdit					*findDefWidget=NULL;
		QLineEdit					*liveSearchWidget=NULL;
//toolbar functions
		void							setUpToolBar(void);
//file vars
		bool							openFromDialog=false;
//file functions
		void							newFile(const QString data="",const QString filename="");
		bool							openFile(QString filepath,int linenumber=1,bool warn=false);
		bool							openFileDialog(void);
		void							openAsHexDump(void);
		QString						runPipeAndCapture(QString command);
		bool							saveFile(int tabnum,bool ask=true);
		void							newEditor(int what);
		void							runPipe(QString command);
		bool							saveAllFiles(void);
		bool							saveFileAs(int tabnum);
		int							askSaveDialog(const QString filename);
		QStringList					getNewRecursiveTagList(QString filepath);

//document vars
		QStringList					completionWords;
		QCompleter					*completer=NULL;

//document functions
		DocumentClass				*getDocumentForTab(int tabnum);
		void							resetAllFilePrefs(void);
		void							goToDefinition(const QString txt);
		void							showLineEntry(void);
		void							gotoLine(int linenumber);
		void							reloadDocument(void);
		void							setCompWordList(void);
		void							insertCompletion(const QString &completion);

//bookmark vars
		QHash<int,bookMarkStruct>	bookMarks;
		int							bookMarksIndex=100;
//bookmark functions
		void							rebuildBookMarkMenu(void);
		void							handleBMMenu(QWidget *widget,int what,QTextCursor curs);

//tools menu
		QMenu						*toolsMenu;
//tools vars
		QDialog						*toolsWindow;
		QComboBox					*toolSelect;
		QMainWindow					*toolOutputWindow;
		QPlainTextEdit				*toolsOPText=NULL;

//tools functions
		void							rebuildToolsMenu(void);
		QStringList					verifyTool(QString filepath);
		void							buildToolOutputWindow(void);

//docviewer vars
#ifdef _BUILDDOCVIEWER_
		QMainWindow					*docView=NULL;
		QWebView						*webView=NULL;
		bool							docviewerVisible=false;
		MenuItemClass				*toggleDocViewMenuItem;
		QString						currentURL;
//docviewer functions
#endif
		void							buildDocViewer(void);
		void							setDocMenu(void);

//prefswindow
//prefswindow vars
		QWidget						*prefsWidgets[MAXPREFSWIDGETS];
		QWidget						*prefsIntWidgets[MAXPREFSINTWIDGETS];
		QWidget						*prefsOtherWidgets[MAXPREFSOTHERWIDGETS];
		QGridLayout					*table;
		QDialog						*prefsWindow;
		QListWidget					*listWidget;
		QToolBar						*fromHBox;
		QAction						*tool[19];
		QDialog						*keyShortcutsWindow;

//prefswindow functions
		void							makePrefsCheck(int widgnum,const QString label,bool onoff,int posx,int posy);
		void							makePrefsDial(int widgnum,const QString label,int value,int minvalue,int maxvalue,int posy);
		void							addIcon(const char* icon,const char* data,int toolnumber,const char* tooltip);
		void							populateDnD(void);
		void							buildPrefsWindow(void);
		void							doPrefs(void);
		void							addIconToList(const char* name,int type);
		void							populateStore(void);
		void							resetKeyCombo(void);

//find replace vars
		QWidget						*findReplaceDialog;
		QComboBox					*findDropBox;
		QComboBox					*replaceDropBox;
		QCheckBox					*frSwitches[FRMAXSWITCHES];
		QPushButton					*frReplace;
		QStringList					findList;
		QStringList					replaceList;
		unsigned int					maxFRHistory=5;
		int							startingTab=0;
		int							currentTab=0;

//find replace functions
		void							buildFindReplace(void);
		void							doFindReplace(int response_id);

//search vars
//search functions
		void							searchAPIDocs(const QString txt,int what);

//plugins
//plugin menu
		QMenu						*pluginMenu;

//plug vars
		QDialog						*pluginPrefsWindow;
		QStringList					disabledPlugins;
		QHash<int,pluginStruct>		plugins;

//plug functions
		bool							loadPlug(pluginStruct *ps);
		bool							unloadPlug(pluginStruct *ps);
		void							loadPlugins(void);
		void							buildPlugPrefs(void);

	public slots:
		void							debugSignalSlot(int);
		void							doTimer(void);
		void							doFileMenuItems();
		void							doEditMenuItems();
		void							doViewMenuItems();
		void							doNavMenuItems();
		void							doBookmarkMenuItems();
		void							doHelpMenuItems();
		void							doToolsMenuItems();
		void							doTabBarContextMenu(void);
		void							doOddMenuItems(void);
		void							doOddButtons(void);

		void							doDoubleClickPrefs(QListWidgetItem *item);
		void							setPreferences(void);
		void							setBMColour(void);
		void							setLineColour(void);
		void							setFont(void);
		void							addToToolBar(void);
		void							buildGetKeyShortCut(int index);

		void							switchPage(int index);
		bool							closeTab(int index);
		void							closeAllTabs(void);

		void							doFindButton(void);
		void							tabContextMenu(const QPoint &pt);

		void							doAppShortCuts(void);

		void							setSearchPrefs(int state);

		void							doSessionsMenuItems(void);

		void							setToolsData(int);

		void							doLiveSearch(const QString text);

	protected:
	private:
//menu functions
		void							clickMenu(QMenu *menu,QString name);
		void							notDoneYet(QString string);
//search functions
		void							setHighlightAll(void);
//editor functions
		void							setTabVisibilty(int tab,bool visible);
};

#endif





