#ifndef _GLOBALINCS_
#define _GLOBALINCS_

#include "config.h"

#include <QApplication>
#include <QtWidgets>
#include <QSettings>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QCompleter>
#include <QObject>
#include <QtPlugin>
#include <QWebEngineSettings>
#include <QProxyStyle>

#include <fcntl.h>
#include <libgen.h>
#include <linux/limits.h>
#include <map>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <vector>

#include <sqlite3.h>

#ifdef _USEQT6_
#include <QWebEngineContextMenuRequest>
#else
#include <QWebEngineContextMenuData>
#endif

#ifdef _ASPELL_
#include <aspell.h>
#endif

#ifdef _BUILDDOCVIEWER_
#include <QWebEngineView>
#endif

#define MAXMSGSIZE 4096
#define MSGKEY 0x8000
#define NOSENDMSG 3

#define MSGANY 0
#define ALLMSGTYPES 0xfff
#define CONTINUEMSG 0x8000
#define SENDMSG 0x1000
#define RAISEMSG 0x4000

#define MAXTEXTWIDTH 500

#define KKEDITFOLDER	".KKEditQT"

#define	POLEPATH PREFIX "/bin/KKEditProgressBarQT"
#define CUSTOMWORDFILE ".KKEditQT/customcompletions"

#define NUMSHORTCUTS 14

#define GLOBALPLUGS 1
#define LOCALPLUGS 0

#define NEWMENUNAME "newmenu"
#define OPENMENUNAME "openmenu"
#define HEXDUMPMENUNAME "hexdumpmenu"
#define NEWADMINMENUNAME "newadminmenu"

#define NEWEDITORMENUNAME "neweditormenu"
#define MANEDITORMENUNAME "maneditormenuqt"
#define DOXYBUILDMENUNAME "doxybuildmenu"
#define DOCSETBUILDMENUNAME "dosetbuildmenu"
#define RECENTMENUNAME "recentmenu"
#define SAVEMENUNAME "savemenu"
#define SAVEASMENUNAME "saveasmenu"
#define SAVEALLMENUNAME "saveallmenu"
#define SAVESESSIONMENUNAME "savesessionmenu"
#define RESTORESESSIONMENUNAME "restoresessionmenu"
#define SAVECURRENTSESSIONMENUNAME "savecurrentsessionmenu"
#define RESTOREDEFAULTSESSIONMENUNAME "restoredefaultsessionmenu"

#define PRINTMENUNAME "printmenu"
#define EXPORTTOPDFMENUNAME "printtopdf"
#define IMPORTFROMPDFMENUNAME "printfrompdf"
#define CLOSEMENUNAME "closemenu"
#define CLOSEALLMENUNAME "closeallmenu"
#define SORTTABSMENUNAME "sorttabsmenu"
#define SHOWALLTABSMENUNAME "showalltabsmenu"
#define SELECTTABMENUNAME "selecttabmenu"
#define REVERTMENUNAME "revertmenu"
#define QUITMENUNAME "quitmenu"
#define UNDOMENUNAME "undomenu"
#define REDOMENUNAME "redomenu"
#define UNDOALLMENUNAME "undoallmenu"
#define REDOALLMENUNAME "redoallmenu"
#define CUTMENUNAME "cutmenu"
#define COPYMENUNAME "copymenu"
#define PASTEMENUNAME "pastemenu"
#define VERTICALPASTEMENUNAME "verticalpastemenu"
#define DELETEMENUNAME "deletemenu"
#define SELECTALLMENUNAME "selectallmenu"
#define FINDMENUNAME "findmenu"
#define FINDNEXTMENUNAME "findnextmenu"
#define PREFSMENUNAME "prefsmenu"
#define PLUGPREFSMENUNAME "plugprefsmenu"
#define SHOWDOCSMENUNAME "showdocsmenu"
#define SHOWBMBARMENUNAME "showbmbarmenu"
#define SHOWTOOLBARMENUNAME "showtoolbarmenu"
#define SHOWTOOLOUTMENUNAME "showtoolout"
#define SHOWSTATUSMENUNAME "showstatusmenu"
#define SHOWDOCVIEWERMENUNAME "showdocviewer"
#define RAISEDOCVIEWERMENUNAME "raisedocviewer"
#define GOTODEFMENUNAME "gotodefinemenu"
#define GOTODEFMENUSINGLENAME "gotodefinemenusingle"
#define OPENINCLUDEMENUNAME "openincludemenu"
#define GOTOLINEMENUNAME "gotolinemenu"
#define SEARCHFORDEFMENUNAME "searchfordefmenu"
#define SEARCHGTKMENUNAME "searchgtkmenu"
#define SEARCHQT5MENUNAME "searchqt5menu"
#define SEARCHDOXYMENUNAME "searchdoxymenu"
#define GOBACKMENUNAME "gobackmenu"
#define GOFORWARDMENUNAME "goforwardmenu"
#define ABOUTMENUNAME "aboutmenu"
#define ABOUTQTMENUNAME "aboutmenuqt"
#define HELPMENUNAME "helpmenu"
#define GETPLUGSMENUNAME "getplugsmenu"
#define REMOVEALLBOOKMARKS "removeallbookmarks"
#define TOGGLEBOOKMARK "togglebookmark"

#define FINDNEXTOBJECTNAME "1"
#define FINDPREVOBJECTNAME "2"
#define FINDREPLACEOBJECTNAME "3"

#define TOOLCOMBOBOX "toolcombobox"
#define TOOLNAME "name"
#define TOOLCOMMAND "command"
#define TOOLKEY "shortcutkey"
#define TOOLCOMMENT "comment"
#define TOOLFLAGS "flags"
#define TOOLRUNINTERM "interm"
#define TOOLSHOWINPOPUP "inpopup"
#define TOOLALWAYSINPOPUP "alwayspopup"
#define TOOLRUNSYNC "toolrunsync"
#define TOOLSHOWDOC "toolshowdoc"
#define TOOLCLEAROP "clearview"
#define TOOLRUNASROOT "runasroot"
#define TOOLUSEPOLE "usebar"
#define TOOLIGNOREOUT "toolignoreout"
#define TOOLPASTEOUT "toolpasteout"
#define TOOLREPLACEALL "toolreplaceall"
#define TOOLVIEWOUT "toolviewout"

#define TOOL_ASYNC 8
#define TOOL_IGNORE_OP 0
#define TOOL_PASTE_OP 1
#define TOOL_REPLACE_OP 2
#define TOOL_SHOW_DOC 4
#define	TOOL_INSERT_MASK 0b10011
#define	TOOL_VIEW_OP 16

#define MAXSESSIONS 128
#define STATUSBARTIMEOUT 5000
#define MARGINS 4

enum msgActions {ACTIVATEAPPMSG=100,OPENFILEMSG,NEWFILEMSG,SAVEFILEMSG,SAVEFILEASMSG,QUITAPPMSG,SAVECURRENTSESSIONMSG,RESTORESESSIONMSG,GOTOLINEMSG,SEARCHDEFMSG,SELECTTABMSG,SELECTTABBYNAMEMSG,SELECTTABBYPATHMSG,BOOKMARKMSG,CLOSETABMSG,CLOSEALLTABSMSG,SETUSERMARKMSG,UNSETUSERMARKMASG,MOVETOMSG,PASTEMSG,COPYMSG,CUTMSG,INSERTTEXTMSG,INSERTNLMSG,SELECTBETWEENMSG,INSERTFILEMSG,PRINTFILESMSG,RUNTOOLMSG,ACTIVATEMENUBYLABELEDMSG,OPENINDOCVIEWMSG,SENDPOSDATAMSG,SENDSELECTEDTEXTMSG,SENDCURRENTURL,SENDSESSIONNAMEMSG,LASTMSG};//TODO//seperate info comands etc

enum toolEnums {TOOL_ALWAYS_IN_POPUP=0,TOOL_CLEAR_VIEW,TOOL_COMMAND,TOOL_COMMENT,TOOL_FLAGS,TOOL_INPOPUP,TOOL_IN_TERM,TOOL_NAME,TOOL_RUN_AS_ROOT,TOOL_SHORTCUT_KEY,TOOL_USE_BAR,TOOL_END};

enum {FILEMENU=0x4000,EDITMENU,VIEWMENU,NAVMENU,FUNCTIONSMENU,BOOKMARKSMENU,TOOLSMENU,PLUGINSMENU,HELPMENU,SAVESESSIONSMENU,RESTORESESSIONSMENU,CURRENTSESSION,NOMENU};

//file
enum {NEWMENUITEM=0x8000,OPENMENUITEM,HEXDUMPMENUITEM,NEWADMINEDMENUITEM,NEWEDMENUITEM,MANPAGEEDMENUITEM,BUILDDOCSMENUITEM,BUILDDOCSETMENUITEM,SAVEMENUITEM,SAVEASMENUITEM,SAVEALLMENUITEM,SAVESESSIONMENUITEM,RESTORESESSIONMENUITEM,PRINTMENUITEM,CLOSEMENUITEM,CLOSEALLMENUITEM,REVERTMENUITEM,EXPORTTOPDFMENUITEM,IMPORTFROMPDFMENUITEM,QUITMENUITEM};
//edit
enum {UNDOMENUITEM=0x9000,REDOMENUITEM,UNDOALLMENUITEM,REDOALLMENUITEM,EDSEP1,CUTMENUITEM,COPYMENUITEM,PASTEMENUITEM,VERTICALPASTEMENUITEM,DELETEMENUITEM,EDSEP2,SELECTALLMENUITEM,EDSEP3,FINDMENUITEM,FINDNEXTMENUITEM,EDSEP4,SORTTABSMENUITEM,SHOWALLTABSMENUITEM,SELECTTABMENUITEM,EDSEP5,PREFSMENUITEM,PLUGPREFSMENUITEM};
//view
enum {DOCSMENUITEM=0xa000,TOGGLETOOLBARMENUITEM,TOGGLETOOLWINDOWMENUITEM,TOGGLESTATUSBARMENUITEM,TOGGLEDOCVIEWMENUITEM,RAISEDOCVIEWMENUITEM,TOGGLELINENUMBERSMENUITEM,TOGGLELINEWRAPMENUITEM,TOGGLEHILITELINEMENUITEM,TOGGLEWHITESPACEMENUITEM,TOGGLESYNTAXHILITINGMENUITEM,TOGGLECOMPLETIONSMENUITEM};
//nav
enum {GOTODEFINEMENUITEM=0xb000,OPENINCLUDEMENUITEM,GOTOLINEMENUITEM,SEARCHFORDEFINEMENUITEM,SEARCHDOXYDOCS,GOBACKMENUITEM,GOFORWARDMENUITEM,GOTODEFINEMENUSINGLEITEM};
//func
//bms
enum {REMOVEALLBOOKMARKSMENUITEM=0xc000,TOGGLEBOOKMARKMENUITEM,REMOVEBOOKMARKSFROMPAGE};

//tools
enum {MANAGETOOLSMENUITEM=0xd000,TOOLNUMBER};
//plugins
//help
enum {ABOUTMENUITEM=0xe000,ABOUTQTMENUITEM,HELPMENUITEM};

enum	{AUTOINDENT=0,SHOWNUMS,WRAP,HIGHLIGHT,SYNTAXHILITE,USESINGLE,AUTOSAVE,NODUPLICATE,NOWARN,AUTOSHOW,AUTOONETAB,OPENTABPOS,SELECTFIRSTTAB,BEKIND,MAXPREFSWIDGETS};
enum {MAXTABCHARS=0,MAXHISTORY,MAXFUNCDEPTH,COMPLETIONSIZE,TABWIDTH,MENUWIDTH,MAXRECENTS,MSGCHECKTIME,MAXMENUSESSIONS,MAXPREFSINTWIDGETS};
enum {FUNCTIONCOMBO=0,THEMECOMBO,FONTNAMECOMBO,PREFSTERMCOMMAND,PREFSPRINTCOMMAND,PREFSPAGESIZE,PREFSROOTCOMMAND,PREFSCURRENTFONT,BMHIGHLIGHTCOLOUR,CURRENTLINECOLOUR,SHORTCUTSCOMBO,MAXPREFSOTHERWIDGETS};

enum {FINDNEXT=1,FINDPREV,FINDREPLACE};

//odd enums
enum {SPELLCHECKMENUITEM=0x2000,APPLYWORDBUTTON,IGNOREWORDBUTTON,ADDWORDBUTTON,CANCELSPELLCHECK,CANCELPREFS,DOLINEBOX,DOLIVESEARCH,DOAPISEARCH,DOCVIEWERGOHOME,TOOLSSAVE,TOOLSCANCEL,TOOLSDELETE,TOOLSSAVEAS,TOOLSEDIT};

enum {COPYFOLDERPATH=0xf000,COPYFILEPATH=0xf100,COPYFILENAME=0xf200,SPELLCHECKDOC=0xf300,SRCHILTIE=0xf400,HIDETAB=0xf500,LOCKCONTENTS=0xf600,OPENFROMHERE=0xf700};

enum {TABCONTEXTMENUCNT=(OPENFROMHERE-COPYFOLDERPATH) / 0x100 +1};

enum {NONESRCCODE=0,CPPSRCCODE=0x100,CSRCCODE=0x200,BASHSRCCODE=0x300,PYTHONSRCCODE=0x400,GOSRCCODE=0x500,LUASRCCODE=0x600,YAMLSRCCODE=0x700,PHPSRCCODE=0x800,XMLSRCCODE=0x900,CSSSRCCODE=0xa00,JSSRCCODE=0xb00,MAKESRCCODE=0xc00};

enum {HIDETABSHORTCUT=0,DELETELINESHORTCUT,DELETETOEOLSHORTCUT,DELETETOSOLSHORTCUT,SELECTWORDSHORTCUT,DELETEWORDSHORTCUT,DUPLICATELINESHORTCUT,SELECTLINESHORTCUT,MOVELINEUPSHORTCUT,MOVELINEDOWNSHORTCUT,MOVESELECTIONUPSHORTCUT,MOVESELECTIONDOWNSHORTCUT,FORCESHOWCOMPLETE,NOMORESHORTCUT,NOSHORTCUT};

enum {FRCASE=0,FRUSEREGEX,FRREPLACEFIND,FRWRAP,FRALLFILES,FRHIGHLIGHTALL,FRREPLACEALL,FRSEARCHBACK,FRMAXSWITCHES};

class KKEditClass;
class DocumentClass;

struct tabMenuStruct
{
	unsigned int		what;
	const char		*label;
	const char		*icon;
};

struct msgStruct
{
	long mType=0;
	char mText[MAXMSGSIZE];
};

struct bookMarkStruct
{
	DocumentClass*		doc;
	int					line;
	int					docIndex;
	QString				bmLabel;
	QAction				*menu;
	int					bmKey;
};

#include "kkeditqtPluginInterface.h"
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
	bool								local=false;
#ifdef _DEBUGCODE_
	void				printIt(void)
		{
		//	qSetMessagePattern("[%{type}] %{appname} (%{file}:%{line}) - %{message}");
			qDebug() << "pluginLoader" << pluginLoader;
			qDebug() << "plugPath" << plugPath;
			qDebug() << "plugName" << plugName;
			qDebug() << "plugVersion" << plugVersion;
			qDebug() << "instance" << instance;
			qDebug() << "loaded" << loaded;
			qDebug() << "statusChanged" << statusChanged;
			qDebug() << "broken" << broken;

			switch(wants)
				{
					case DONONE:
						qDebug() << "wants DONONE";
						break;
					case DOSAVE:
						qDebug() << "wants DOSAVE";
						break;
					case DOLOAD:
						qDebug() << "wants DOLOAD";
						break;
					case DOCLOSE:
						qDebug() << "wants DOCLOSE";
						break;
					case DORESSESSION:
						qDebug() << "wants DORESSESSION";
						break;
					case DOSAVESESSION:
						qDebug() << "wants DOSAVESESSION";
						break;
					case DOCONTEXTMENU:
						qDebug() << "wants DOCONTEXTMENU";
						break;
					case DOTABPOPUP:
						qDebug() << "wants DOTABPOPUP";
						break;
					case DOSETSENSITVE:
						qDebug() << "wants DOSETSENSITVE";
						break;
					case DOSWITCHPAGE:
						qDebug() << "wants DOSWITCHPAGE";
						break;
					case DONEWDOCUMENT:
						qDebug() << "wants DONEWDOCUMENT";
						break;
					case DOSHUTDOWN:
						qDebug() << "wants DOSHUTDOWN";
						break;
					case DOABOUT:
						qDebug() << "wants DOABOUT";
						break;
					case DOSETTINGS:
						qDebug() << "wants DOSETTINGS";
						break;
					default://qDebug("My hex number is: %x", hexnum);
						qDebug() << "wants" << Qt::bin << Qt::showbase << wants;
				}
			qDebug()<<"";
		};
#endif
};

struct miniPrefsReturnStruct
{
	QDialog					*theDialog;
	QHash<int,QLineEdit*>	boxes;
};

class ProxyStyle;
class ThemeClass;
class TabColours;
class NoteBookClass;
class MenuItemClass;
class RecentMenuClass;
class HistoryClass;
class ToolBarClass;
class kkEditQTPluginInterface;

#include "QT_document.h"
#include "tagClass.h"
#include "QT_ProxyStyle.h"
#include "ChooserDialog.h"
#include "QT_themeClass.h"
#include "QT_AboutBox.h"
#include "QT_menuitem.h"
#include "QT_historyClass.h"
#include "QT_recentMenu.h"
#include "QT_highlighter.h"
#include "QT_toolbar.h"
#include "QT_notebook.h"

#endif
