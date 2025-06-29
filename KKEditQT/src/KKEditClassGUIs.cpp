/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:39:44 GMT 2021 keithdhedger@gmail.com

 * This file (KKEditClassGUIs.cpp) is part of KKEditQT.

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

void KKEditClass::makePrefsDial(int widgnum,const QString label,int value,int minvalue,int maxvalue,int posy)
{
	QLabel*	widgetlabel;
	prefsIntWidgets[widgnum]=new QSpinBox;
	prefsIntWidgets[widgnum]->setProperty("maximum",maxvalue);
	prefsIntWidgets[widgnum]->setProperty("minimum",minvalue);
	prefsIntWidgets[widgnum]->setProperty("value",value);
	widgetlabel=new QLabel(label);
	this->table->addWidget(widgetlabel,posy,0);
	this->table->addWidget(prefsIntWidgets[widgnum],posy,1,1,-1);
}

void KKEditClass::makePrefsCheck(int widgnum,const QString label,bool onoff,int posx,int posy)
{
	prefsWidgets[widgnum]=new QCheckBox(label);
	prefsWidgets[widgnum]->setProperty("checked", onoff);
	if(posx!=-1)
		this->table->addWidget(prefsWidgets[widgnum],posy,posx,Qt::AlignTop);
}

void KKEditClass::doPrefs(void)
{
	this->resetKeyCombo();
	this->prefsWindow->setWindowModality(Qt::ApplicationModal);
	this->prefsWindow->raise();
	this->prefsWindow->setWindowState(Qt::WindowActive);
	this->prefsWindow->show();
}

//{"<html>Set line auto indenting\nLines are indented with space/tabs on pressing '<b>RETURN</b>'<br><img src=\"" DATADIR "/help/addtoolbutton.gif\" height=\"100\" width=\"200\"></html>",
static const char *whatIsPrefsBool[MAXPREFSWIDGETS]={\
"<html>Set line auto indenting\nLines are indented with space/tabs on pressing '<b>RETURN</b>'",\
"<html>Show line numbers.</html>",\
"<html>Wrap long lines.</html>",\
"<html>Highlight current line.</html>",\
"<html>Use syntax highlighting.</html>",\
"<html>Use single instance app.</html>",\
"<html>Automatically save the currently open tabs on exit.<br><br>Use '<b>Restore Autosave Session</b>' to restore.</html>",\
"<html>Don't open duplicate file, just switch tabs.</html>",\
"<html>Don't warn if file changed on disk, just update document.</html>",\
"<html>Auto show completions.</html>",\
"<html>Auto set tabs to one space when ALT selecting and restore on mouse release.</html>",\
"<html>Open new tab to right of current tab ( true ) or at end (false ).</html>",\
"<html>Select first tab after resoring session.</html>",\
"<html><div align=\"center\">Check this if you have sent me big bags of cash.<br>Big bags of jewels also acceptable.<br><center><span style='font-size:24px;'>&#128519;</span></center></div></html>"};

//prefsOtherWidgets
static const char *whatIsPrefsOther[MAXPREFSOTHERWIDGETS]={\
"<html>Layout of 'Funtions' menu, default is '<b>Display functions etc in menu by type and alphabetically</b>'</html>",\
"<html>Select document theme.</html>",\
"<html>Set font and size etc.</html>",\
"<html>Enter a command to run a command in a terminal, the command is added to the end of this string.<br><br>Default is '<b>xterm -e</b>'</html>",\
"<html>QT5 does not play well with CUPS ( nothing to do with me! )<br><br>You can set an external program to print your document, just enter the name or full path here, the full path to the current document will be added to the end as the file to print.<br>E.g. lpr to usr the line printer.<br><br>You can also use a gui application e.g. lprgui available here:<br><b>https://github.com/KeithDHedger/LprGUI</b><br><br>Leave this blank to use the built in QT5 print dialog ( but you may only get the native print to pdf option ).</html>",\
"<html>Enter a command to run a command as root, the command is added to the end of this string.<br><br>Default is '<b>gtksu -- env QTWEBENGINE_DISABLE_SANDBOX=1 env QT_QPA_PLATFORMTHEME=qt5ct </b>'</html>",\
"<html>Set current font.</html>",\
"<html>Set bookmark highlight colour.</html>",\
"<html>Set current line colour.</html>",\
"<html>Set keyboard shortcuts.</html>"};

//prefs int widgets
static const char *whatIsPrefsInt[MAXPREFSINTWIDGETS]={\
"<html>Number of chacters in document tabs, default is <b>20</b></html>",\
"<html>Maximum number of find/replace terms stored, default <b>5</b>.</html>",\
"<html>Tags recursive search depth, default is <b>1</b>.<br><br>More than 1 can significantly slow down function search if you have a lot of subfolders in you source tree.</html>",\
"<html>Minimum number of characters entered before the completion pop up shows, default <b>6</b>.</html>",\
"<html>Number of characters to a text tab, default <b>4</b>.</html>",\
"<html>Max characters in menu items and function definitions, default <b>64</b>.</html>",\
"<html>Maximum number of recently opened files stored, default <b>10</b>.</html>",\
"<html>Check for messages every X ms, default <b>1000</b>.</html>",\
"<html>Maximum number of sessions default <b>24</b>.</html>"};

void KKEditClass::buildPrefsWindow(void)
{
	QVBoxLayout			*mainvbox=new QVBoxLayout();
	QHBoxLayout			*hbox=new QHBoxLayout;
	QTabWidget			*prefsnotebook=new QTabWidget;
	QWidget				*button;
	QPushButton			*whatsbutton;
	QWidget				*tab;
	QLabel				*widgetlabel;
	int					posy;

	this->listWidget=new QListWidget;
	fromHBox=new QToolBar;

	this->prefsWindow=new QDialog(this->mainWindow);
	this->prefsWindow->setWindowTitle("Preferences");

	mainvbox->setContentsMargins(0,0,0,16);
	hbox->setContentsMargins(0,0,0,0);

	this->populateDnD();
	this->populateStore();

	QObject::connect(this->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(doDoubleClickPrefs(QListWidgetItem*)));

	this->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	this->listWidget->setDragEnabled(true);
	this->listWidget->setDragDropMode(QAbstractItemView::InternalMove);
	this->listWidget->viewport()->setAcceptDrops(true);
	this->listWidget->setFlow(QListView::LeftToRight);
	this->listWidget->setGridSize(QSize(32,32));
	this->listWidget->setSizePolicy (QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored));
	this->listWidget->setMinimumHeight(48);
	this->listWidget->setMinimumWidth(32*(strlen(this->prefsToolBarLayout.toStdString().c_str()))+4);

	mainvbox->addWidget(this->listWidget,2);
	mainvbox->addWidget(this->fromHBox);
//pages
//page1
	this->table=new QGridLayout;
	tab=new QWidget();

//appearence 1
//prefsIndent
	posy=0;
	makePrefsCheck(AUTOINDENT,"Auto Indent Lines",this->prefsIndent,0,posy);
	prefsWidgets[AUTOINDENT]->setWhatsThis(whatIsPrefsBool[AUTOINDENT]);
//linenumbers
	posy++;
	makePrefsCheck(SHOWNUMS,"Show Line Numbers",this->prefsShowLineNumbers,0,posy);
	prefsWidgets[SHOWNUMS]->setWhatsThis(whatIsPrefsBool[SHOWNUMS]);
//wraplines
	posy++;
	makePrefsCheck(WRAP,"Wrap Lines",this->prefsLineWrap,0,posy);
	prefsWidgets[WRAP]->setWhatsThis(whatIsPrefsBool[WRAP]);
//highlite
	posy++;
	makePrefsCheck(HIGHLIGHT,"Highlight Current Line",this->prefsHighLightline,0,posy);
	prefsWidgets[HIGHLIGHT]->setWhatsThis(whatIsPrefsBool[HIGHLIGHT]);
//no syntax colour
	posy++;
	makePrefsCheck(SYNTAXHILITE,"Syntax Highlighting",this->prefsSyntaxHilighting,0,posy);
	prefsWidgets[SYNTAXHILITE]->setWhatsThis(whatIsPrefsBool[SYNTAXHILITE]);
//single instance
	posy++;
	makePrefsCheck(USESINGLE,"Use Single Instance",this->prefsUseSingle,0,posy);
	prefsWidgets[USESINGLE]->setWhatsThis(whatIsPrefsBool[USESINGLE]);
//auto save session
	posy++;
	makePrefsCheck(AUTOSAVE,"Save Session On Exit",this->onExitSaveSession,0,posy);
	prefsWidgets[AUTOSAVE]->setWhatsThis(whatIsPrefsBool[AUTOSAVE]);
//no duplicates
	posy++;
	makePrefsCheck(NODUPLICATE,"Don't Open Duplicate File",this->prefsNoOpenduplicate,0,posy);
	prefsWidgets[NODUPLICATE]->setWhatsThis(whatIsPrefsBool[NODUPLICATE]);
//turn off warnings
	posy++;
	makePrefsCheck(NOWARN,"Don't Warn On File Change",this->prefsNoWarnings,0,posy);
	prefsWidgets[NOWARN]->setWhatsThis(whatIsPrefsBool[NOWARN]);
//autoshow completion
	posy++;
	makePrefsCheck(AUTOSHOW,"Show Completions",this->prefsAutoShowCompletions,0,posy);
	prefsWidgets[AUTOSHOW]->setWhatsThis(whatIsPrefsBool[AUTOSHOW]);

//auto set tabs to one space and back when ALT selecting
	posy++;
	makePrefsCheck(AUTOONETAB,"Auto Set Tabs",this->autoOneTab,0,posy);
	prefsWidgets[AUTOONETAB]->setWhatsThis(whatIsPrefsBool[AUTOONETAB]);

//open tab to right of current tab
	posy++;
	makePrefsCheck(OPENTABPOS,"Open New Tab To Right",this->openTabToRight,0,posy);
	prefsWidgets[OPENTABPOS]->setWhatsThis(whatIsPrefsBool[OPENTABPOS]);

	tab->setLayout(table);
	prefsnotebook->addTab(tab,"General Appearance");

//select 1st tab when restoring session
	posy++;
	makePrefsCheck(SELECTFIRSTTAB,"Select First Tab On Restore Session",this->openFirstTabWithSession,0,posy);
	prefsWidgets[SELECTFIRSTTAB]->setWhatsThis(whatIsPrefsBool[SELECTFIRSTTAB]);

	tab->setLayout(table);
	prefsnotebook->addTab(tab,"General Appearance");

//page2
	table=new QGridLayout;
	tab=new QWidget();

//tabwidth
	posy=1;
	makePrefsDial(TABWIDTH,"Tab width:",this->prefsTabWidth,1,64,posy);
	prefsIntWidgets[TABWIDTH]->setWhatsThis(whatIsPrefsInt[TABWIDTH]);

//style
	posy++;
	prefsOtherWidgets[THEMECOMBO]=new QComboBox;
	prefsOtherWidgets[THEMECOMBO]->setWhatsThis(whatIsPrefsOther[THEMECOMBO]);
	widgetlabel=new QLabel("Theme:");
	table->addWidget(widgetlabel,posy,0);
	table->addWidget(prefsOtherWidgets[THEMECOMBO],posy,1,1,-1);

//local
{
	QDir		languagesDir(QString("%1/themes/").arg(this->homeDataFolder));
	QDirIterator	it(languagesDir.canonicalPath(),QStringList("*.json"), QDir::Files,QDirIterator::Subdirectories);

	while (it.hasNext())
		{
			QString	s=it.next();
			qobject_cast<QComboBox*>(prefsOtherWidgets[THEMECOMBO])->addItem(QFileInfo(s).baseName());
		}
	QObject::connect(qobject_cast<QComboBox*>(this->prefsOtherWidgets[THEMECOMBO]),QOverload<int>::of(&QComboBox::activated),[this](int index)
		{
			this->prefStyleName=qobject_cast<QComboBox*>(this->prefsOtherWidgets[THEMECOMBO])->currentText();
			this->prefStyleNameHold=this->prefStyleName;
		});
}
//global
{
	QDir		languagesDir(QString("%1/themes/").arg(DATADIR));
	QDirIterator	it(languagesDir.canonicalPath(),QStringList("*.json"), QDir::Files,QDirIterator::Subdirectories);

	while (it.hasNext())
		{
			QString	s=it.next();
			qobject_cast<QComboBox*>(prefsOtherWidgets[THEMECOMBO])->addItem(QFileInfo(s).baseName());
		}
	QObject::connect(qobject_cast<QComboBox*>(this->prefsOtherWidgets[THEMECOMBO]),QOverload<int>::of(&QComboBox::activated),[this](int index)
		{
			this->prefStyleName=qobject_cast<QComboBox*>(this->prefsOtherWidgets[THEMECOMBO])->currentText();
			this->prefStyleNameHold=this->prefStyleName;
		});
}
	qobject_cast<QComboBox*>(this->prefsOtherWidgets[THEMECOMBO])->setCurrentText(this->prefStyleName);

//shortcuts
	posy++;
	prefsOtherWidgets[SHORTCUTSCOMBO]=new QComboBox;
	prefsOtherWidgets[SHORTCUTSCOMBO]->setWhatsThis(whatIsPrefsOther[SHORTCUTSCOMBO]);
	this->resetKeyCombo();
    QObject::connect(prefsOtherWidgets[SHORTCUTSCOMBO],SIGNAL(activated(int)),this,SLOT(buildGetKeyShortCut(int)));

	widgetlabel=new QLabel("Set Keyboard Shortcuts:");
	table->addWidget(widgetlabel,posy,0);
	table->addWidget(prefsOtherWidgets[SHORTCUTSCOMBO],posy,1,1,-1);

//font button
	posy++;
	widgetlabel = new QLabel("Font:");
	prefsOtherWidgets[PREFSCURRENTFONT]=new QLabel(QString("%1 %2").arg(this->prefsDocumentFont.family()).arg(this->prefsDocumentFont.pointSize()));
	prefsOtherWidgets[PREFSCURRENTFONT]->setWhatsThis(whatIsPrefsOther[PREFSCURRENTFONT]);
	qobject_cast<QLabel*>(prefsOtherWidgets[PREFSCURRENTFONT])->setFrameStyle(QFrame::Sunken|QFrame::Panel);
    QPushButton *fontbutton = new QPushButton("Set Font");
	table->addWidget(widgetlabel,posy,0);
	table->addWidget(prefsOtherWidgets[PREFSCURRENTFONT],posy,1);
	table->addWidget(fontbutton,posy,2);
	QObject::connect(fontbutton,SIGNAL(clicked()),this,SLOT(setFont()));

//current linecol
	posy++;
    widgetlabel = new QLabel("Highlight Current Line Colour:");
	prefsOtherWidgets[CURRENTLINECOLOUR]=new QLabel("LINE");
	prefsOtherWidgets[CURRENTLINECOLOUR]->setWhatsThis(whatIsPrefsOther[CURRENTLINECOLOUR]);
    qobject_cast<QLabel*>(prefsOtherWidgets[CURRENTLINECOLOUR])->setFrameStyle(QFrame::Raised | QFrame::Panel);
    QPushButton *colorButton = new QPushButton("Set Colour");
	table->addWidget(widgetlabel,posy,0);
	table->addWidget(prefsOtherWidgets[CURRENTLINECOLOUR],posy,1);
	table->addWidget(colorButton,posy,2);

	QColor colour=QColor(this->prefsHiLiteLineColor);
	prefsOtherWidgets[CURRENTLINECOLOUR]->setProperty("palette",QPalette(colour));
	prefsOtherWidgets[CURRENTLINECOLOUR]->setProperty("autoFillBackground",true);
	QObject::connect(colorButton,SIGNAL(clicked()),this,SLOT(setLineColour()));

//bm highlight colour
	posy++;
    widgetlabel = new QLabel("Bookmark Highlight Colour:");
	prefsOtherWidgets[BMHIGHLIGHTCOLOUR]=new QLabel("BOOKMARK");
	prefsOtherWidgets[BMHIGHLIGHTCOLOUR]->setWhatsThis(whatIsPrefsOther[BMHIGHLIGHTCOLOUR]);
    qobject_cast<QLabel*>(prefsOtherWidgets[BMHIGHLIGHTCOLOUR])->setFrameStyle(QFrame::Raised | QFrame::Panel);
    QPushButton *colorButton1 = new QPushButton("Set Colour");
	table->addWidget(widgetlabel,posy,0);
	table->addWidget(prefsOtherWidgets[BMHIGHLIGHTCOLOUR],posy,1);
	table->addWidget(colorButton1,posy,2);

	QColor colour1=QColor(this->prefsBookmarkHiLiteColor);
	prefsOtherWidgets[BMHIGHLIGHTCOLOUR]->setProperty("palette",QPalette(colour1));
	prefsOtherWidgets[BMHIGHLIGHTCOLOUR]->setProperty("autoFillBackground",true);
	QObject::connect(colorButton1,SIGNAL(clicked()),this,SLOT(setBMColour()));

//autoshow completion
	posy++;
	makePrefsDial(COMPLETIONSIZE,"Completion Minimum Word Size:",this->autoShowMinChars,2,20,posy);
	prefsIntWidgets[COMPLETIONSIZE]->setWhatsThis(whatIsPrefsInt[COMPLETIONSIZE]);

//sort functions
	posy++;
	prefsOtherWidgets[FUNCTIONCOMBO]=new QComboBox;
	prefsOtherWidgets[FUNCTIONCOMBO]->setWhatsThis(whatIsPrefsOther[FUNCTIONCOMBO]);
	
	qobject_cast<QComboBox*>(prefsOtherWidgets[FUNCTIONCOMBO])->addItem("Display functions etc in menu by type and alphabetically");
	qobject_cast<QComboBox*>(prefsOtherWidgets[FUNCTIONCOMBO])->addItem("Display functions etc in menu by type and file position");
	qobject_cast<QComboBox*>(prefsOtherWidgets[FUNCTIONCOMBO])->addItem("Display functions etc in menu by file position");
	qobject_cast<QComboBox*>(prefsOtherWidgets[FUNCTIONCOMBO])->addItem("Display functions etc in menu alphabetically");
	qobject_cast<QComboBox*>(prefsOtherWidgets[FUNCTIONCOMBO])->addItem("Display functions etc in menu in categorised format");
	prefsOtherWidgets[FUNCTIONCOMBO]->setProperty("currentIndex",this->prefsFunctionMenuLayout);

	widgetlabel=new QLabel("Function Layout:");
	table->addWidget(widgetlabel,posy,0);
	table->addWidget(prefsOtherWidgets[FUNCTIONCOMBO],posy,1,1,-1);

	QSpacerItem *space=new QSpacerItem(0,0,QSizePolicy::Maximum,QSizePolicy::Maximum);
	posy++;
	table->addItem(space,posy,0,100,-1);

	table->setColumnStretch(1,2);
	tab->setLayout(table);

	prefsnotebook->addTab(tab,"Text Style");

//page 3
//application
	table=new QGridLayout;
	tab=new QWidget();
//function search prefsDepth
	posy=0;
	makePrefsDial(MAXFUNCDEPTH,"Tag File Search Depth:",this->prefsDepth,0,20,posy);
	prefsIntWidgets[MAXFUNCDEPTH]->setWhatsThis(whatIsPrefsInt[MAXFUNCDEPTH]);

//terminalcommand
	posy++;
    widgetlabel=new QLabel("Terminal Command:");
	prefsOtherWidgets[PREFSTERMCOMMAND]=new QLineEdit(this->prefsTerminalCommand);
	prefsOtherWidgets[PREFSTERMCOMMAND]->setWhatsThis(whatIsPrefsOther[PREFSTERMCOMMAND]);
	table->addWidget(widgetlabel,posy,0,Qt::AlignVCenter);
	table->addWidget(prefsOtherWidgets[PREFSTERMCOMMAND],posy,1,1,-1,Qt::AlignVCenter);

//print command
	posy++;
    widgetlabel=new QLabel("Print Command:");
	prefsOtherWidgets[PREFSPRINTCOMMAND]=new QLineEdit(this->prefsPrintCommand);
	prefsOtherWidgets[PREFSPRINTCOMMAND]->setWhatsThis(whatIsPrefsOther[PREFSPRINTCOMMAND]);
	table->addWidget(widgetlabel,posy,0,Qt::AlignVCenter);
	table->addWidget(prefsOtherWidgets[PREFSPRINTCOMMAND],posy,1,1,-1,Qt::AlignVCenter);

//page size;
	posy++;
	prefsOtherWidgets[PREFSPAGESIZE]=new QComboBox;
	QPageSize ps;
	for(int j=0;j<QPageSize::LastPageSize;j++)
		{
			ps=QPageSize((QPageSize::PageSizeId)j);
			if(ps.name().isEmpty()==true)
				qobject_cast<QComboBox*>(prefsOtherWidgets[PREFSPAGESIZE])->addItem("Custom");
			else
				qobject_cast<QComboBox*>(prefsOtherWidgets[PREFSPAGESIZE])->addItem(ps.name());
		}
	QObject::connect(qobject_cast<QComboBox*>(this->prefsOtherWidgets[PREFSPAGESIZE]),QOverload<int>::of(&QComboBox::activated),[this](int index)
		{
			this->prefsPageSize=qobject_cast<QComboBox*>(this->prefsOtherWidgets[PREFSPAGESIZE])->currentIndex();
		});
	qobject_cast<QComboBox*>(prefsOtherWidgets[PREFSPAGESIZE])->setCurrentIndex(this->prefsPageSize);

	widgetlabel=new QLabel("PDF Page Size:");
	table->addWidget(widgetlabel,posy,0);
	table->addWidget(prefsOtherWidgets[PREFSPAGESIZE],posy,1,1,-1);

//root command
	posy++;
    widgetlabel=new QLabel("Run As Root Command:");
	prefsOtherWidgets[PREFSROOTCOMMAND]=new QLineEdit(this->prefsRootCommand);
	prefsOtherWidgets[PREFSROOTCOMMAND]->setWhatsThis(whatIsPrefsOther[PREFSROOTCOMMAND]);
	table->addWidget(widgetlabel,posy,0,Qt::AlignVCenter);
	table->addWidget(prefsOtherWidgets[PREFSROOTCOMMAND],posy,1,1,-1,Qt::AlignVCenter);

//recent history max
	posy++;
	makePrefsDial(MAXRECENTS,"Max Recent Files:",this->recentFiles->maxFiles,0,MAXTEXTWIDTH,posy);
	prefsIntWidgets[MAXRECENTS]->setWhatsThis(whatIsPrefsInt[MAXRECENTS]);

//max sessions
	posy++;
	makePrefsDial(MAXMENUSESSIONS,"Max Sessions:",this->maxSessions,0,MAXSESSIONS,posy);
	prefsIntWidgets[MAXMENUSESSIONS]->setWhatsThis(whatIsPrefsInt[MAXMENUSESSIONS]);//TODO//

//find replace history max
	posy++;
	makePrefsDial(MAXHISTORY,"Max Find/Replace History:",this->maxFRHistory,0,MAXTEXTWIDTH,posy);
	prefsIntWidgets[MAXHISTORY]->setWhatsThis(whatIsPrefsInt[MAXHISTORY]);
//max tab label width
	posy++;
	makePrefsDial(MAXTABCHARS,"Max Characters In Tab:",this->prefsMaxTabChars,5,MAXTEXTWIDTH,posy);
	prefsIntWidgets[MAXTABCHARS]->setWhatsThis(whatIsPrefsInt[MAXTABCHARS]);
//max menu strings
	posy++;
	makePrefsDial(MENUWIDTH,"Max Characters In Menus:",this->prefsMaxMenuChars,5,MAXTEXTWIDTH,posy);
	prefsIntWidgets[MENUWIDTH]->setWhatsThis(whatIsPrefsInt[MENUWIDTH]);
//check for msgs
	posy++;
	makePrefsDial(MSGCHECKTIME,"Check Messages Every (ms):",this->prefsMsgTimer,1,10000,posy);
	prefsIntWidgets[MSGCHECKTIME]->setWhatsThis(whatIsPrefsInt[MSGCHECKTIME]);

//end application

	posy++;
	table->addItem(space,posy,0,100,-1);

	table->setColumnStretch(1,1);

	tab->setLayout(this->table);
	prefsnotebook->addTab(tab,"Application");

	mainvbox->addWidget(prefsnotebook);
//nag
	posy++;
    widgetlabel=new QLabel("<b>Be Kind To Poor Programmers</b>");

	hbox=new QHBoxLayout;
    hbox->addStretch(1);
	hbox->addWidget(widgetlabel);
	hbox->addStretch(1);
	mainvbox->addLayout(hbox,0);

	makePrefsCheck(BEKIND,"I have donated",this->prefsNagScreen,-1,-1);
	prefsWidgets[BEKIND]->setWhatsThis(whatIsPrefsBool[BEKIND]);

	hbox=new QHBoxLayout;
    hbox->addStretch(1);
	hbox->addWidget(prefsWidgets[BEKIND]);
	hbox->addStretch(1);
	mainvbox->addLayout(hbox,0);

    widgetlabel=new QLabel("I have really donated some some money to the author.\nMy conscience is clear and my Karma is squeaky clean :)");
	hbox=new QHBoxLayout;
    hbox->addStretch(1);
	hbox->addWidget(widgetlabel);
	hbox->addStretch(1);
	mainvbox->addLayout(hbox,0);
 
    widgetlabel=new QLabel;
	widgetlabel->setFrameStyle(QFrame::Sunken | QFrame::HLine);
	mainvbox->addWidget(widgetlabel);

//buttons
	hbox=new QHBoxLayout;
	hbox->addStretch(1);
	button=new QPushButton("Save Prefs");
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(setPreferences()));
	hbox->addWidget(button);
	hbox->addStretch(1);

//whats this
	whatsbutton=new QPushButton("What's this?");
	QObject::connect(whatsbutton,&QPushButton::clicked,[this]()
		{
			QWhatsThis::enterWhatsThisMode();
		});
	hbox->addWidget(whatsbutton);
	hbox->addStretch(1);

	button=new QPushButton("Restore Prefs");
	button->setObjectName(QString("%1").arg(CANCELPREFS));
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(doOddButtons()));
	hbox->addWidget(button);
	hbox->addStretch(1);
	mainvbox->addLayout(hbox);

	this->prefsWindow->setLayout(mainvbox);

	this->prefsWindow->setWindowModality(Qt::WindowModal);
}

void KKEditClass::addIcon(const char* icon,const char* data,int toolnumber,const char* tooltip)
{
	QIcon qicon;
	MenuItemClass* menuitem=new MenuItemClass(icon);
	qicon=QIcon::fromTheme(icon);
	menuitem->setObjectName(data);
	menuitem->setIcon(qicon);
	QObject::connect(menuitem,SIGNAL(triggered()),this,SLOT(addToToolBar()));
	menuitem->setMenuID(toolnumber);
	menuitem->setToolTip(tooltip);

	this->fromHBox->addAction(menuitem);
	this->tool[toolnumber]=menuitem;
}

void KKEditClass::populateDnD(void)
{
	this->addIcon("document-new","N",0,"New File");
	this->addIcon("document-open","O",1,"Open File");
	this->addIcon("document-save","S",2,"Save File");
	this->addIcon("edit-cut","X",3,"Cut");
	this->addIcon("edit-copy","C",4,"Copy");
	this->addIcon("edit-paste","P",5,"Paste");
	this->addIcon("edit-undo","U",6,"Undo");
	this->addIcon("edit-redo","R",7,"Redo");
	this->addIcon("edit-find","F",8,"Find");
	this->addIcon("edit-delete","d",19,"Delete");
	this->addIcon("dialog-question","G",9,"Go To Definition");
	this->addIcon("go-previous","B",17,"Go Back");
	this->addIcon("go-next","W",18,"Go Forward");
	this->addIcon(DATADIR"/pixmaps/num.png","9",10,"Go To Line");
	this->addIcon(DATADIR"/pixmaps/finddef.png","D",12,"Search For Define");
	this->addIcon(DATADIR"/pixmaps/live.png","L",13,"Live Search");
	this->addIcon(DATADIR"/pixmaps/sep.png","s",14,"Separator");
	this->addIcon(DATADIR"/pixmaps/expand.png","E",15,"Expander");
}

void KKEditClass::addIconToList(const char* name,int type)
{
	QIcon icon;
	QListWidgetItem *iconw;

	icon=QIcon::fromTheme(name);
	iconw=new QListWidgetItem(icon,"",0,type);
	this->listWidget->addItem(iconw);
}

void KKEditClass::populateStore(void)
{
	for(int j=0;j<this->prefsToolBarLayout.length();j++)
		{
			switch(this->prefsToolBarLayout.at(j).toLatin1())
				{
//new
					case 'N':
						this->addIconToList("document-new",'N');
						this->tool[0]->setEnabled(false);
						break;

//open+recent
					case 'O':
						this->addIconToList("document-open",'O');
						this->tool[1]->setEnabled(false);
						break;
//save
					case 'S':
						this->addIconToList("document-save",'S');
						this->tool[2]->setEnabled(false);
						break;
//cut
					case 'X':
						this->addIconToList("edit-cut",'X');
						this->tool[3]->setEnabled(false);
						break;
//copy
					case 'C':
						this->addIconToList("edit-copy",'C');
						this->tool[4]->setEnabled(false);
						break;
//paste
					case 'P':
						this->addIconToList("edit-paste",'P');
						this->tool[5]->setEnabled(false);
						break;
//delete
					case 'd':
						this->addIconToList("edit-delete",'d');
						this->tool[19]->setEnabled(false);
						break;
//undo
					case 'U':
						this->addIconToList("edit-undo",'U');
						this->tool[6]->setEnabled(false);
						break;
//redo
					case 'R':
						this->addIconToList("edit-redo",'R');
						this->tool[7]->setEnabled(false);
						break;
//find
					case 'F':
						this->addIconToList("edit-find",'F');
						this->tool[8]->setEnabled(false);
						break;
//navigation
					case 'G':
						this->addIconToList("dialog-question",'G');
						this->tool[9]->setEnabled(false);
						break;
//go back
					case 'B':
						this->addIconToList("go-previous",'B');
						this->tool[17]->setEnabled(false);
						break;
//go forward
					case 'W':
						this->addIconToList("go-next",'W');
						this->tool[18]->setEnabled(false);
						break;
//go to line
					case '9':
						this->addIconToList(DATADIR"/pixmaps/num.png",'9');
						this->tool[10]->setEnabled(false);
						break;
//find def
					case 'D':
						this->addIconToList(DATADIR"/pixmaps/finddef.png",'D');
						this->tool[12]->setEnabled(false);
						break;
//live search
					case 'L':
						this->addIconToList(DATADIR"/pixmaps/live.png",'L');
						this->tool[13]->setEnabled(false);
						break;
//seperator
					case 's':
						this->addIconToList(DATADIR"/pixmaps/sep.png",'s');
						break;
//expander
					case 'E':
						this->addIconToList(DATADIR"/pixmaps/expand.png",'E');
						break;
				}
		}
}

void KKEditClass::buildFindReplace(void)
{
	QVBoxLayout*	vlayout=new QVBoxLayout;
	QWidget*		hbox;
	QHBoxLayout*	hlayout;
	QLabel			*label;
	QPushButton		*button;
	QIcon			icon;

	this->findReplaceDialog=new QDialog(this->mainWindow);
	this->findReplaceDialog->setWindowTitle("Find/Replace");
	vlayout->setContentsMargins(4,0,4,0);

//find
	label=new QLabel("Find");
	label->setAlignment(Qt::AlignCenter);
	vlayout->addWidget(label);

	this->findDropBox=new QComboBox;
	reinterpret_cast<QComboBox*>(this->findDropBox)->setEditable(true);
	reinterpret_cast<QComboBox*>(this->findDropBox)->setCompleter(0);
	reinterpret_cast<QComboBox*>(this->findDropBox)->setInsertPolicy(QComboBox::InsertAtBottom);
	this->findDropBox->addItems(this->findList);
	this->findDropBox->setCurrentIndex(this->findDropBox->count()-1);
	vlayout->addWidget(this->findDropBox);

//replace
	label=new QLabel("Replace With");
	label->setAlignment(Qt::AlignCenter);
	vlayout->addWidget(label);

	this->replaceDropBox=new QComboBox;
	reinterpret_cast<QComboBox*>(this->replaceDropBox)->setEditable(true);
	reinterpret_cast<QComboBox*>(this->replaceDropBox)->setCompleter(0);
	reinterpret_cast<QComboBox*>(this->replaceDropBox)->setInsertPolicy(QComboBox::InsertAtBottom);
	this->replaceDropBox->addItems(this->replaceList);
	this->replaceDropBox->setCurrentIndex(this->replaceDropBox->count()-1);
	vlayout->addWidget(replaceDropBox);

//switches 1st row
	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	hbox=new QWidget;
	hbox->setLayout(hlayout);

//case
	this->frSwitches[FRCASE]=new QCheckBox("Case insensitive");
	this->frSwitches[FRCASE]->setChecked(this->insensitiveSearch);
	hlayout->addWidget(this->frSwitches[FRCASE]);
//use regex
	this->frSwitches[FRUSEREGEX]=new QCheckBox("Use Regex");
	this->frSwitches[FRUSEREGEX]->setChecked(this->useRegex);
	hlayout->addWidget(this->frSwitches[FRUSEREGEX]);
//find after replace
	this->frSwitches[FRREPLACEFIND]=new QCheckBox("Find After Replace");
	this->frSwitches[FRREPLACEFIND]->setChecked(this->findAfterReplace);
	hlayout->addWidget(this->frSwitches[FRREPLACEFIND]);

	vlayout->addWidget(hbox);

//switches 2nd row
	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	hbox=new QWidget;
	hbox->setLayout(hlayout);
//wrap
	this->frSwitches[FRWRAP]=new QCheckBox("Wrap");
	this->frSwitches[FRWRAP]->setChecked(this->wrapSearch);
	hlayout->addWidget(this->frSwitches[FRWRAP]);

//all files
	this->frSwitches[FRALLFILES]=new QCheckBox("All Files");
	this->frSwitches[FRALLFILES]->setChecked(this->findInAllFiles);
	hlayout->addWidget(this->frSwitches[FRALLFILES]);
//hilite all
	this->frSwitches[FRHIGHLIGHTALL]=new QCheckBox("Highlight All");
	this->frSwitches[FRHIGHLIGHTALL]->setChecked(this->hightlightAll);
	hlayout->addWidget(this->frSwitches[FRHIGHLIGHTALL]);
//rep all
	this->frSwitches[FRREPLACEALL]=new QCheckBox("Replace All");
	this->frSwitches[FRREPLACEALL]->setChecked(this->replaceAll);
	QObject::connect(this->frSwitches[FRREPLACEALL],SIGNAL(stateChanged(int)),this,SLOT(setSearchPrefs()));
	hlayout->addWidget(this->frSwitches[FRREPLACEALL]);
//search back
	this->frSwitches[FRSEARCHBACK]=new QCheckBox("Search Backwards");
	this->frSwitches[FRSEARCHBACK]->setChecked(this->searchBack);
	QObject::connect(this->frSwitches[FRSEARCHBACK],SIGNAL(stateChanged(int)),this,SLOT(setSearchPrefs()));
	hlayout->addWidget(this->frSwitches[FRSEARCHBACK]);

	vlayout->addWidget(hbox);

//switches 3rd row
	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,4,0,4);
	hbox=new QWidget;
	hbox->setLayout(hlayout);

	button=new QPushButton("Find");
	button->setObjectName(FINDNEXTOBJECTNAME);
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(doFindButton()));
	icon=QIcon::fromTheme("edit-find");
	button->setIcon(icon);
	hlayout->addWidget(button);

	if(replaceAll==false)
		this->frReplace=new QPushButton("Replace");
	else
		this->frReplace=new QPushButton("Replace All");
	frReplace->setObjectName(FINDREPLACEOBJECTNAME);
	QObject::connect(frReplace,SIGNAL(clicked()),this,SLOT(doFindButton()));
	icon=QIcon::fromTheme("edit-find-replace");
	this->frReplace->setIcon(icon);
	hlayout->addWidget(this->frReplace);

	vlayout->addWidget(hbox);

	this->findReplaceDialog->setLayout(vlayout);
}

void KKEditClass::buildMainGui(void)
{
	MenuItemClass	*menuItemSink;

	this->mainNotebook=new NoteBookClass(this);
//this->mainNotebook->setStyleSheet(QString("QTabBar::tab {width: 256;}"));//TODO//
	this->mainNotebook->setContextMenuPolicy(Qt::CustomContextMenu);
	QObject::connect(this->mainNotebook,SIGNAL(customContextMenuRequested(const QPoint &)),SLOT(tabContextMenu(const QPoint &)));

	QObject::connect(this->mainNotebook,SIGNAL(currentChanged(int)),this,SLOT(switchPage(int)));
	QObject::connect(this->mainNotebook,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));

	this->menuBar=new QMenuBar;
	this->toolBar=new ToolBarClass(this);

//file menu
	this->fileMenu=new QMenu("&File");
	this->menuBar->addMenu(this->fileMenu);

//new
	this->newMenuItem=this->makeMenuItemClass(FILEMENU,"New",QKeySequence::New,"document-new",NEWMENUNAME,NEWMENUITEM);

//open
	this->openMenuItem=this->makeMenuItemClass(FILEMENU,"Open",QKeySequence::Open,"document-open",OPENMENUNAME,OPENMENUITEM);

//open recent
	this->fileMenu->addMenu(this->recentFiles->recentMenu);

//open as hexdump
	menuItemSink=this->makeMenuItemClass(FILEMENU,"Open As Hexdump",0,"document-open",HEXDUMPMENUNAME,HEXDUMPMENUITEM);

	this->fileMenu->addSeparator();

//extras
	menuItemSink=this->makeMenuItemClass(FILEMENU,"New Admin Editor",0,DATADIR"/pixmaps/ROOTKKEditQT.png",NEWADMINMENUNAME,NEWADMINEDMENUITEM);
	menuItemSink=this->makeMenuItemClass(FILEMENU,"New Editor",0,DATADIR"/pixmaps/MenuKKEditQT.png",NEWEDITORMENUNAME,NEWEDMENUITEM);
	
	if(gotManEditor==0)
		menuItemSink=this->makeMenuItemClass(FILEMENU,"Manpage Editor",0,DATADIR"/pixmaps/ManPageEditor.png",MANEDITORMENUNAME,MANPAGEEDMENUITEM);

//doxy
	if(gotDoxygen==0)
		{
			menuItemSink=this->makeMenuItemClass(FILEMENU,"Build Documentation",0,"edit-copy",DOXYBUILDMENUNAME,BUILDDOCSMENUITEM);
			menuItemSink=this->makeMenuItemClass(FILEMENU,"Build Docset",0,"edit-copy",DOCSETBUILDMENUNAME,BUILDDOCSETMENUITEM);
		}
	this->fileMenu->addSeparator();

//save
	this->saveMenuItem=this->makeMenuItemClass(FILEMENU,"Save",QKeySequence::Save,"document-save",SAVEMENUNAME,SAVEMENUITEM);
//savas
	this->saveAsMenuItem=this->makeMenuItemClass(FILEMENU,"Save As",QKeySequence("Ctrl+Shift+S"),"document-save-as",SAVEASMENUNAME,SAVEASMENUITEM);

//save all
	this->saveAllMenuItem=this->makeMenuItemClass(FILEMENU,"Save All",0,"document-save",SAVEALLMENUNAME,SAVEALLMENUITEM);

	this->fileMenu->addSeparator();

//save session
	this->saveSessionsMenu=new QMenu("&Save Session");
	this->fileMenu->addMenu(this->saveSessionsMenu);
	this->saveCurrentSessionMenuItem=this->makeMenuItemClass(SAVESESSIONSMENU,"Save Current Session",0,NULL,SAVESESSIONMENUNAME,CURRENTSESSION);
	this->saveSessionsMenu->addSeparator();
	for(int j=1;j<this->maxSessions;j++)
		{
			menuItemSink=this->makeMenuItemClass(SAVESESSIONSMENU,this->sessionNames.value(j),0,NULL,SAVESESSIONMENUNAME,j);
		}

//restore session
	this->restoreSessionsMenu=new QMenu("&Restore Session");
	this->fileMenu->addMenu(this->restoreSessionsMenu);
	this->restoreDefaultSessionMenuItem=this->makeMenuItemClass(RESTORESESSIONSMENU,"Restore Autosave Session",0,NULL,RESTORESESSIONMENUNAME,CURRENTSESSION);
	this->restoreSessionsMenu->addSeparator();
	for(int j=1;j<this->maxSessions;j++)
		{
			menuItemSink=this->makeMenuItemClass(RESTORESESSIONSMENU,this->sessionNames.value(j),0,NULL,RESTORESESSIONMENUNAME,j);
			this->restoreSessionMenuItemsList.append(menuItemSink);
		}
	this->fileMenu->addSeparator();

//printfile
	this->printMenuItem=this->makeMenuItemClass(FILEMENU,"Print",QKeySequence::Print,"document-print",PRINTMENUNAME,PRINTMENUITEM);
//export/import
	if(this->gotPDFCrop==0)
		menuItemSink=this->makeMenuItemClass(FILEMENU,"Export To PDF",0,"stock_export",EXPORTTOPDFMENUNAME,EXPORTTOPDFMENUITEM);
	if(this->gotPDFToText==0)
		menuItemSink=this->makeMenuItemClass(FILEMENU,"Import From PDF",0,"stock_export",IMPORTFROMPDFMENUNAME,IMPORTFROMPDFMENUITEM);

	this->fileMenu->addSeparator();

//close
	this->closeMenuItem=this->makeMenuItemClass(FILEMENU,"Close",QKeySequence::Close,"window-close",CLOSEMENUNAME,CLOSEMENUITEM);

//close-all
	this->closeAllMenuItem=this->makeMenuItemClass(FILEMENU,"Close All",0,"window-close",CLOSEALLMENUNAME,CLOSEALLMENUITEM);

	this->fileMenu->addSeparator();

//reload file
	this->revertMenuItem=this->makeMenuItemClass(FILEMENU,"Revert",0,"document-revert",REVERTMENUNAME,REVERTMENUITEM);

	this->fileMenu->addSeparator();

//quit
	menuItemSink=this->makeMenuItemClass(FILEMENU,"Quit",QKeySequence::Quit,"application-exit",QUITMENUNAME,QUITMENUITEM);

//edit menu
	this->editMenu=new QMenu("&Edit");
	this->menuBar->addMenu(this->editMenu);

//undo
	this->undoMenuItem=this->makeMenuItemClass(EDITMENU,"Undo",QKeySequence::Undo,"edit-undo",UNDOMENUNAME,UNDOMENUITEM);

//redo
	this->redoMenuItem=this->makeMenuItemClass(EDITMENU,"Redo",QKeySequence::Redo,"edit-redo",REDOMENUNAME,REDOMENUITEM);

//undoall
	this->undoAllMenuItem=this->makeMenuItemClass(EDITMENU,"Undo All",0,"edit-undo",UNDOALLMENUNAME,UNDOALLMENUITEM);

//redoall
	this->redoAllMenuItem=this->makeMenuItemClass(EDITMENU,"Redo All",0,"edit-redo",REDOALLMENUNAME,REDOALLMENUITEM);

	this->editMenu->addSeparator();

//cut
	this->cutMenuItem=this->makeMenuItemClass(EDITMENU,"Cut",QKeySequence::Cut,"edit-cut",CUTMENUNAME,CUTMENUITEM);
//copy
	this->copyMenuItem=this->makeMenuItemClass(EDITMENU,"Copy",QKeySequence::Copy,"edit-copy",COPYMENUNAME,COPYMENUITEM);
//paste
	this->pasteMenuItem=this->makeMenuItemClass(EDITMENU,"Paste",QKeySequence::Paste,"edit-paste",PASTEMENUNAME,PASTEMENUITEM);
// vertical paste
	this->verticalPasteMenuItem=this->makeMenuItemClass(EDITMENU,"Vertical Paste",0,"edit-paste",VERTICALPASTEMENUNAME,VERTICALPASTEMENUITEM);
//delete
	this->deleteMenuItem=this->makeMenuItemClass(EDITMENU,"Delete",QKeySequence::Delete,"edit-delete",DELETEMENUNAME,DELETEMENUITEM);
//selectall
	this->editMenu->addSeparator();
	this->selectAllMenuItem=this->makeMenuItemClass(EDITMENU,"Select All",QKeySequence::SelectAll,"edit-select-all",SELECTALLMENUNAME,SELECTALLMENUITEM);

	this->editMenu->addSeparator();

//find
	this->findMenuItem=this->makeMenuItemClass(EDITMENU,"Find",QKeySequence::Find,"edit-find",FINDMENUNAME,FINDMENUITEM);
//find next
	this->findNextMenuItem=this->makeMenuItemClass(EDITMENU,"Find Next",QKeySequence("Ctrl+G"),"go-next",FINDNEXTMENUNAME,FINDNEXTMENUITEM);

	this->editMenu->addSeparator();

//sort tabs
	this->sortTabsMenuItem=this->makeMenuItemClass(EDITMENU,"Sort Tabs",0,"view-sort-ascending",SORTTABSMENUNAME,SORTTABSMENUITEM);
//show all tabs
	this->showAllTabsMenuItem=this->makeMenuItemClass(EDITMENU,"Show All Tabs",0,"list-add",SHOWALLTABSMENUNAME,SHOWALLTABSMENUITEM);
//select tab
	this->selectTabMenu=new QMenu("Select Tab");
	this->editMenu->addMenu(this->selectTabMenu);

	this->editMenu->addSeparator();

//prefs
	menuItemSink=this->makeMenuItemClass(EDITMENU,"Preferences",0,"preferences-desktop",PREFSMENUNAME,PREFSMENUITEM);
//plugs
	menuItemSink=this->makeMenuItemClass(EDITMENU,"Plugin Prefs",0,"preferences-system",PLUGPREFSMENUNAME,PLUGPREFSMENUITEM);

//view menu
	this->viewMenu=new QMenu("&View");
	this->menuBar->addMenu(this->viewMenu);

	if(this->verySafeFlag==false)
		{
			QMenu		*thememenu;
			thememenu=new QMenu("Theme");
			this->viewMenu->addMenu(thememenu);
//local
			{
				QDir				languagesDir(QString("%1/themes/").arg(this->homeDataFolder));
				QDirIterator		it(languagesDir.canonicalPath(),QStringList("*.json"), QDir::Files,QDirIterator::Subdirectories);
				while (it.hasNext())
					{
						QString s=it.next();
						QAction *menuitem=new QAction(QFileInfo(s).baseName());
						thememenu->addAction(menuitem);
						QObject::connect(menuitem,&QAction::triggered,[this,s]()
							{
								this->prefStyleNameHold=QFileInfo(s).baseName();
								this->theme->loadTheme(this->prefStyleNameHold);
								this->resetAllFilePrefs();
							});
					}
			}
//global
			{
				QDir				languagesDir(QString("%1/themes/").arg(DATADIR));
				QDirIterator		it(languagesDir.canonicalPath(),QStringList("*.json"), QDir::Files,QDirIterator::Subdirectories);
				while (it.hasNext())
					{
						QString s=it.next();
						QAction *menuitem=new QAction(QFileInfo(s).baseName());
						thememenu->addAction(menuitem);
						QObject::connect(menuitem,&QAction::triggered,[this,s]()
							{
								this->prefStyleNameHold=QFileInfo(s).baseName();
								this->theme->loadTheme(this->prefStyleNameHold);
								this->resetAllFilePrefs();
							});
					}
			}
		}

//show docs
	menuItemSink=this->makeMenuItemClass(VIEWMENU,"Show Documentation",0,NULL,SHOWDOCSMENUNAME,DOCSMENUITEM);

//toggle toolbar bar
	if(this->toolbarVisible)
		toggleToolBarMenuItem=this->makeMenuItemClass(VIEWMENU,"Hide Tool Bar",0,NULL,SHOWTOOLBARMENUNAME,TOGGLETOOLBARMENUITEM);
	else
		toggleToolBarMenuItem=this->makeMenuItemClass(VIEWMENU,"Show Tool Bar",0,NULL,SHOWTOOLBARMENUNAME,TOGGLETOOLBARMENUITEM);
//tooloutput
	if(this->toolWindowVisible)
		this->toggleToolWindowMenuItem=this->makeMenuItemClass(VIEWMENU,"Hide Tool Output",0,NULL,SHOWTOOLOUTMENUNAME,TOGGLETOOLWINDOWMENUITEM);
	else
		this->toggleToolWindowMenuItem=this->makeMenuItemClass(VIEWMENU,"Show Tool Output",0,NULL,SHOWTOOLOUTMENUNAME,TOGGLETOOLWINDOWMENUITEM);

//toggle statusbar
	if(this->statusBarVisible)
		this->toggleStatusBarMenuItem=this->makeMenuItemClass(VIEWMENU,"Hide Status Bar",0,NULL,SHOWSTATUSMENUNAME,TOGGLESTATUSBARMENUITEM);
	else
		this->toggleStatusBarMenuItem=this->makeMenuItemClass(VIEWMENU,"Show Status Bar",0,NULL,SHOWTOOLOUTMENUNAME,TOGGLESTATUSBARMENUITEM);

#ifdef _BUILDDOCVIEWER_
//toggle docviewer
	this->toggleDocViewMenuItem=this->makeMenuItemClass(VIEWMENU,"Show Docviewer",0,NULL,SHOWDOCVIEWERMENUNAME,TOGGLEDOCVIEWMENUITEM);
//docviewer to front
	menuItemSink=this->makeMenuItemClass(VIEWMENU,"Raise Docviewer",0,NULL,RAISEDOCVIEWERMENUNAME,RAISEDOCVIEWMENUITEM);
#endif

	this->viewMenu->addSeparator();

//toggle line numbers
	this->lineNumbersVisible=this->prefsShowLineNumbers;
	this->toggleLineNumbersMenuItem=this->makeMenuItemClass(VIEWMENU,"Show Line Numbers",0,NULL,"NOTNEEDED",TOGGLELINENUMBERSMENUITEM);
	this->toggleLineNumbersMenuItem->setCheckable(true);
	this->toggleLineNumbersMenuItem->setChecked(this->prefsShowLineNumbers);
//toggle wrap line
	this->wrapLine=this->prefsLineWrap;
	this->toggleWrapLineMenuItem=this->makeMenuItemClass(VIEWMENU,"Wrap Lines",0,NULL,"NOTNEEDED",TOGGLELINEWRAPMENUITEM);
	this->toggleWrapLineMenuItem->setCheckable(true);
	this->toggleWrapLineMenuItem->setChecked(this->prefsLineWrap);
//toggle hilit current line
	this->hiliteCurrentLine=this->prefsHighLightline;
	this->toggleHiliteCurrentLineMenuItem=this->makeMenuItemClass(VIEWMENU,"Highlight Current Line",0,NULL,"NOTNEEDED",TOGGLEHILITELINEMENUITEM);
	this->toggleHiliteCurrentLineMenuItem->setCheckable(true);
	this->toggleHiliteCurrentLineMenuItem->setChecked(this->hiliteCurrentLine);
//show hiliting
	this->showHighLighting=this->prefsSyntaxHilighting;
	this->toggleSyntaxHilightingMenuItem=this->makeMenuItemClass(VIEWMENU,"Syntax Highlighting",0,NULL,"NOTNEEDED",TOGGLESYNTAXHILITINGMENUITEM);
	this->toggleSyntaxHilightingMenuItem->setCheckable(true);
	this->toggleSyntaxHilightingMenuItem->setChecked(this->showHighLighting);
//toggle whitespace
	this->toggleWhiteSpaceMenuItem=this->makeMenuItemClass(VIEWMENU,"Show Whitespace",0,NULL,"NOTNEEDED",TOGGLEWHITESPACEMENUITEM);
	this->toggleWhiteSpaceMenuItem->setCheckable(true);
	this->toggleWhiteSpaceMenuItem->setChecked(false);
//QCompleter
//toggle completions
	this->showCompletions=this->prefsAutoShowCompletions;
	this->toggleCompletionsMenuItem=this->makeMenuItemClass(VIEWMENU,"Show Completions",0,NULL,"NOTNEEDED",TOGGLECOMPLETIONSMENUITEM);
	this->toggleCompletionsMenuItem->setCheckable(true);
	this->toggleCompletionsMenuItem->setChecked(this->showCompletions);

//navigation menu
	this->navMenu=new QMenu("&Navigation");
	this->menuBar->addMenu(this->navMenu);

//goto define
	goToDefineMenuItem=this->makeMenuItemClass(NAVMENU,"Go To Definition",0,"dialog-question",GOTODEFMENUNAME,GOTODEFINEMENUITEM);
	goToDefineMenuSingleItem=this->makeMenuItemClass(NOMENU,"Go To Definition In This Page",0,"stock_info",GOTODEFMENUSINGLENAME,GOTODEFINEMENUSINGLEITEM);
	
//open include
	menuItemSink=this->makeMenuItemClass(NAVMENU,"Open Include File",QKeySequence::fromString("Ctrl+I"),"document-open",OPENINCLUDEMENUNAME,OPENINCLUDEMENUITEM);
//goto line number
	goToLineDialogMenuItem=this->makeMenuItemClass(NAVMENU,"Go To Line",0,"go-down",GOTOLINEMENUNAME,GOTOLINEMENUITEM);
//find define
	menuItemSink=this->makeMenuItemClass(NAVMENU,"Search For Define",0,"edit-find",SEARCHFORDEFMENUNAME,SEARCHFORDEFINEMENUITEM);
//goto doxy docs
	if(gotDoxygen==0)
		menuItemSink=this->makeMenuItemClass(NAVMENU,"Find In Documentation",0,"edit-find",SEARCHDOXYMENUNAME,SEARCHDOXYDOCS);
//go back
	this->goBackMenu=this->makeMenuItemClass(NAVMENU,"Back",0,"go-previous","NOTNEEDED",GOBACKMENUITEM);
//go forward
	this->goFowardMenu=this->makeMenuItemClass(NAVMENU,"Forward",0,"go-next","NOTNEEDED",GOFORWARDMENUITEM);

	this->navMenu->addSeparator();

//function menu
	this->funcMenu=new QMenu("Fun&ctions");
	this->menuBar->addMenu(this->funcMenu);

//newbookmarks
	this->bookMarkMenu=new QMenu("&Bookmarks");
	this->menuBar->addMenu(this->bookMarkMenu);
	this->rebuildBookMarkMenu();

//external tools	
	this->toolsMenu=new QMenu("&Tools");
	this->menuBar->addMenu(this->toolsMenu);
	this->buildTools();

//plugin menu
	this->pluginMenu=new QMenu("&Plugins");
	this->menuBar->addMenu(this->pluginMenu);

//help
	this->helpMenu=new QMenu("&Help");
	this->menuBar->addMenu(this->helpMenu);
//
//about
	menuItemSink=this->makeMenuItemClass(HELPMENU,"About",0,"help-about",ABOUTMENUNAME,ABOUTMENUITEM);
//aboutqt
	menuItemSink=this->makeMenuItemClass(HELPMENU,"About QT",0,"help-about",ABOUTMENUNAME,ABOUTQTMENUITEM);

//help
	menuItemSink=this->makeMenuItemClass(HELPMENU,"Help",0,"help-contents",HELPMENUNAME,HELPMENUITEM);

	this->setUpToolBar();
	this->mainWindow->setMenuBar(qobject_cast<QMenuBar*>(this->menuBar));
	this->mainWindow->addToolBar(this->toolBar);
	this->mainWindow->setCentralWidget(this->mainNotebook);

	this->statusText=new QLabel;	
	this->statusText->setText("Line 0\tCol 0");
 	this->statusBar=this->mainWindow->statusBar();
 	this->statusBar->addWidget(this->statusText);
}

void KKEditClass::showLineEntry(void)
{
	bool	ok;

	QString	text=QInputDialog::getText(this->mainWindow,"Go To Line","Enter Line Number",QLineEdit::Normal,"0",&ok);

	if ((ok==true) && (!text.isEmpty()))
		this->gotoLine(text.toUInt());
}


void KKEditClass::buildTools(void)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsequence-point"
	QVBoxLayout		*mainvbox=new QVBoxLayout();
	QHBoxLayout		*hbox=new QHBoxLayout;
	QLabel			*widgetlabel;
	QGridLayout		*grid;
	QWidget			*tab;
	QCheckBox		*check;
	QRadioButton		*radio;
	QLineEdit		*edit;
	QPushButton		*button;
	int				posx=0;
	int				posy=0;

	this->toolsWindow=new QDialog(this->mainWindow);
	this->toolsWindow->setWindowTitle("External Tools");

	mainvbox->setContentsMargins(0,0,0,16);
	hbox->setContentsMargins(0,0,0,0);

	tab=new QWidget();
	grid=new QGridLayout;
	grid->setColumnStretch(1,1);

	this->toolSelect=new QComboBox;
	this->toolSelect->setObjectName(TOOLCOMBOBOX);
	QObject::connect(this->toolSelect,SIGNAL(activated(int)),this,SLOT(setToolsData(int)));
	mainvbox->addWidget(this->toolSelect);
	this->rebuildToolsMenu();

//name
	widgetlabel=new QLabel("Tool Name:");
	grid->addWidget(widgetlabel,posy,posx,Qt::AlignVCenter);
	edit=new QLineEdit("New Tool");
	edit->setObjectName(TOOLNAME);
	grid->addWidget(edit,posy++,posx+1,Qt::AlignVCenter);

//command
	widgetlabel=new QLabel("Command:");
	grid->addWidget(widgetlabel,posy,posx,Qt::AlignVCenter);
	edit=new QLineEdit("");
	edit->setObjectName(TOOLCOMMAND);
	grid->addWidget(edit,posy++,1,Qt::AlignVCenter);
//shortcut
	widgetlabel=new QLabel("Shortcut:");
	grid->addWidget(widgetlabel,posy,posx,Qt::AlignVCenter);
	edit=new QLineEdit("");
	edit->setObjectName(TOOLKEY);
	grid->addWidget(edit,posy++,posx+1,Qt::AlignVCenter);
//comment
	widgetlabel=new QLabel("Comment:");
	grid->addWidget(widgetlabel,posy,posx,Qt::AlignVCenter);
	edit=new QLineEdit("");
	edit->setObjectName(TOOLCOMMENT);
	grid->addWidget(edit,posy++,posx+1,Qt::AlignVCenter);

//info
	QString	info="PLACEHOLDERS:\n\
%l - List of open files. Passed to command as $KKEDIT_FILE_LIST, files are space separated\n\
%n - Number of open files. PASSED to command as $KKEDIT_FILECOUNT\n\
%f - Filepath of the current document. Passed to command as $KKEDIT_CURRENTFILE\n\
%d - Directory of the current document or $HOME. Passed to command as $KKEDIT_CURRENTDIR\n\
%t - Currently selected text. Passed to command as $KKEDIT_SELECTION\n\
%m - Mime type of file. Passed to command as $KKEDIT_MIMETYPE\n\
%s - Currnt session name. Passed to command as $KKEDIT_CURRENT_SESSION, may be empty\n\
%h - Tempory file for displaying html in doc viewer. Passed to command as $KKEDIT_HTMLFILE\n\
%i - The location of the globally installed tools. Passed to command as $KKEDIT_DATADIR\n\
%p - Progress bar control file. Passed to command as $KKEDIT_BAR_CONTROL\n";

	widgetlabel=new QLabel(info);
	grid->addWidget(widgetlabel,posy,posx,posy++,-1,Qt::AlignVCenter);

	posy+=7;
//checkboxes
//run as root
	check=new QCheckBox("Run Tool As Root");
	check->setObjectName(TOOLRUNASROOT);
	grid->addWidget(check,posy++,posx,Qt::AlignVCenter);
//run in term
	check=new QCheckBox("Run Tool In Terminal");
	check->setObjectName(TOOLRUNINTERM);
	QObject::connect(check,SIGNAL(stateChanged(int)),this,SLOT(setToolsData(int)));
	grid->addWidget(check,posy++,posx,Qt::AlignVCenter);
//show html doc
	check=new QCheckBox("Show HTML Doc");
	check->setObjectName(TOOLSHOWDOC);
	QObject::connect(check,SIGNAL(stateChanged(int)),this,SLOT(setToolsData(int)));
	grid->addWidget(check,posy++,posx,Qt::AlignVCenter);
//run sync
	check=new QCheckBox("Run Tool Synchronously");
	check->setChecked(true);
	QObject::connect(check,SIGNAL(stateChanged(int)),this,SLOT(setToolsData(int)));	
	check->setObjectName(TOOLRUNSYNC);
	grid->addWidget(check,posy++,posx,Qt::AlignVCenter);

//use pole
	check=new QCheckBox("Use Progress Bar");
	check->setObjectName(TOOLUSEPOLE);
	grid->addWidget(check,posy++,posx,Qt::AlignVCenter);

//show in popup
	check=new QCheckBox("Show Tool In Pop-Up Menu");
	check->setObjectName(TOOLSHOWINPOPUP);
	grid->addWidget(check,posy++,posx,Qt::AlignVCenter);
//always show in popup
	check=new QCheckBox("Always Show Tool In Pop-Up Menu");
	check->setObjectName(TOOLALWAYSINPOPUP);
	grid->addWidget(check,posy++,posx,Qt::AlignVCenter);
//clear tool out
	check=new QCheckBox("Clear Tool Output First");
	check->setObjectName(TOOLCLEAROP);
	grid->addWidget(check,posy++,posx,Qt::AlignVCenter);

//radios
//ignore out
	radio=new QRadioButton("Ignore Output");
	radio->setChecked(true);
	radio->setObjectName(TOOLIGNOREOUT);
	grid->addWidget(radio,posy++,posx,Qt::AlignVCenter);
//paste out
	radio=new QRadioButton("Paste Output");
	radio->setObjectName(TOOLPASTEOUT);
	grid->addWidget(radio,posy++,posx,Qt::AlignVCenter);
//replace all
	radio=new QRadioButton("Replace All Contents");
	radio->setObjectName(TOOLREPLACEALL);
	grid->addWidget(radio,posy++,posx,Qt::AlignVCenter);
//view out
	radio=new QRadioButton("View Output");
	radio->setObjectName(TOOLVIEWOUT);
	grid->addWidget(radio,posy++,posx,Qt::AlignVCenter);

	tab->setLayout(grid);
	mainvbox->addWidget(tab);

	hbox=new QHBoxLayout;
	hbox->addStretch(1);
	button=new QPushButton("Save Tool");
	button->setObjectName(QString("%1").arg(TOOLSSAVE));
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(doOddButtons()));
	button->setEnabled(false);
	hbox->addWidget(button);

	hbox->addStretch(1);
	button=new QPushButton("Save Tool As");
	button->setObjectName(QString("%1").arg(TOOLSSAVEAS));
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(doOddButtons()));
	hbox->addWidget(button);

	hbox->addStretch(1);
	button=new QPushButton("Delete Tool");
	button->setObjectName(QString("%1").arg(TOOLSDELETE));
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(doOddButtons()));
	hbox->addWidget(button);

	hbox->addStretch(1);
	button=new QPushButton("Edit Tool");
	button->setObjectName(QString("%1").arg(TOOLSEDIT));
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(doOddButtons()));
	hbox->addWidget(button);

	hbox->addStretch(1);
	button=new QPushButton("Dismiss");
	button->setObjectName(QString("%1").arg(TOOLSCANCEL));
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(doOddButtons()));
	hbox->addWidget(button);
	hbox->addStretch(1);
	mainvbox->addLayout(hbox);

	this->toolsWindow->setLayout(mainvbox);

	this->toolsWindow->setWindowModality(Qt::WindowModal);
#pragma GCC diagnostic pop
}

void KKEditClass::sortTabs(void)
{
	QTabBar	*bar=this->mainNotebook->tabBar();
	bool	flag=true;
	DocumentClass	*doc1;
	DocumentClass	*doc2;

	while(flag==true)
		{
			flag=false;
			for(int j=0;j<bar->count()-1;j++)
				{
					doc1=this->getDocumentForTab(j);
					doc2=this->getDocumentForTab(j+1);
					if(doc1->getFileName().compare(doc2->getFileName())>0)
						{
							flag=true;
							bar->moveTab(j+1,j);
						}
				}
		}
	rebuildTabsMenu();
}

void KKEditClass::rebuildTabsMenu(void)
{
	DocumentClass	*doc;
	MenuItemClass	*menuitem;
	QTabBar			*bar=this->mainNotebook->tabBar();

	if(this->sessionBusy==true)
		return;

	this->selectTabMenu->clear();

	for(int j=0;j<bar->count();j++)
		{
			doc=this->getDocumentForTab(j);
			menuitem=new MenuItemClass(doc->getFileName());
			menuitem->setMenuID(j);
			this->selectTabMenu->addAction(menuitem);
			QObject::connect(menuitem,&QAction::triggered,[this,j]()
				{
					Qt::KeyboardModifiers key=QGuiApplication::keyboardModifiers();
					int cur=this->mainNotebook->currentIndex();
					if((key==Qt::ShiftModifier) && (cur!=j))
						{
							int adj=1;
							if(j<cur)
								adj=0;
							this->mainNotebook->tabBar()->moveTab(j,cur+adj);
							this->setTabVisibilty(cur+adj,true);
						}
					else
						{
							this->setTabVisibilty(j,true);
						}
				});
		}
}

void KKEditClass::buildSpellCheckerGUI(void)
{
#ifdef _ASPELL_
	QVBoxLayout*	vlayout=new QVBoxLayout;
	QWidget*		hbox;
	QHBoxLayout*	hlayout;
	QPushButton		*button;
	QIcon			icon;

	this->spellCheckGUI=new QDialog(this->mainWindow);
	this->spellCheckGUI->setWindowTitle("Spell Check Word");
	vlayout->setContentsMargins(4,0,4,0);

//find
	this->infoLabel=new QLabel("Change word to:");
	this->infoLabel->setAlignment(Qt::AlignCenter);
	vlayout->addWidget(this->infoLabel);

	this->wordDropBox=new QComboBox;
	this->wordDropBox->setEditable(true);
	//reinterpret_cast<QComboBox*>(this->findDropBox)->setCompleter(0);
	vlayout->addWidget(this->wordDropBox);

//switches 3rd row
	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,4,0,4);
	hbox=new QWidget;
	hbox->setLayout(hlayout);

	button=new QPushButton("Apply");
	button->setObjectName(QString("%1").arg(APPLYWORDBUTTON));
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(doOddButtons()));
	hlayout->addWidget(button);

	button=new QPushButton("Ignore");
	button->setObjectName(QString("%1").arg(IGNOREWORDBUTTON));
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(doOddButtons()));
	hlayout->addWidget(button);

	button=new QPushButton("Add");
	button->setObjectName(QString("%1").arg(ADDWORDBUTTON));
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(doOddButtons()));
	hlayout->addWidget(button);

	button=new QPushButton("Cancel");
	button->setObjectName(QString("%1").arg(CANCELSPELLCHECK));
	QObject::connect(button,SIGNAL(clicked()),this,SLOT(doOddButtons()));
	hlayout->addWidget(button);

	vlayout->addWidget(hbox);

	this->spellCheckGUI->setLayout(vlayout);
#endif
}

bool KKEditClass::openFileDialog(void)
{
	chooserDialogClass	chooser(chooserDialogType::loadDialog);
	int					ci=this->mainNotebook->currentIndex()+1;

	chooser.setMultipleSelect(true);
	chooser.gFind.LFSTK_sortByTypeAndName();
	chooser.gFind.LFSTK_setIgnoreBroken(true);
	chooser.setShowImagesInList(false);
	chooser.addFileTypes(".cpp;.c;.h;.hpp;.m;.mm;.py;.go;.java;.js;.rb;.sh;.rs;.tcl;.pl");
	chooser.addFileTypes(".html;.xml;.css;.php;.pro;.in;.am;.m4;.md;.ac;.json;.class;.sql");
	chooser.addFileTypes("All Files");

	chooser.dialogWindow.exec();
	if(chooser.valid==false)
		return(false);
	if(chooser.multiFileList.count()>0)
		{
			for(int j=0;j<chooser.multiFileList.count();j++)
				this->openFile(chooser.multiFileList.at(j).toUtf8().constData(),0,true);
		}
	this->openFromDialog=false;
	this->moveTabToRight(ci);

	return(true);
}

void KKEditClass::resetKeyCombo(void)
{
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->clear();
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Hide tab - %1").arg(this->defaultShortCutsList.at(HIDETABSHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Delete current line - %1").arg(this->defaultShortCutsList.at(DELETELINESHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Delete to EOL - %1").arg(this->defaultShortCutsList.at(DELETETOEOLSHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Delete to SOL - %1").arg(this->defaultShortCutsList.at(DELETETOSOLSHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Select word under cursor - %1").arg(this->defaultShortCutsList.at(SELECTWORDSHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Delete word under cursor - %1").arg(this->defaultShortCutsList.at(DELETEWORDSHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Duplicate line - %1").arg(this->defaultShortCutsList.at(DUPLICATELINESHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Select current line - %1").arg(this->defaultShortCutsList.at(SELECTLINESHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Move line up - %1").arg(this->defaultShortCutsList.at(MOVELINEUPSHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Move line down - %1").arg(this->defaultShortCutsList.at(MOVELINEDOWNSHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Move Selection up - %1").arg(this->defaultShortCutsList.at(MOVESELECTIONUPSHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Move Selection down - %1").arg(this->defaultShortCutsList.at(MOVESELECTIONDOWNSHORTCUT)));
	qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->addItem(QString("Force show completion - %1").arg(this->defaultShortCutsList.at(FORCESHOWCOMPLETE)));
}

void KKEditClass::buildGetKeyShortCut(int index)
{
	bool	ok;

	QString	text=QInputDialog::getText(this->mainWindow,"Keyboard Shortcut","Enter Key Combo",QLineEdit::Normal,this->defaultShortCutsList.at(index),&ok);
	if ((ok==true) && (!text.isEmpty()))
		{
			text=QString(LFSTK_UtilityClass::LFSTK_strReplaceAllChar(text.toStdString()," ","",true).c_str());
			this->defaultShortCutsList[index]=text;
			this->resetKeyCombo();
			qobject_cast<QComboBox*>(prefsOtherWidgets[SHORTCUTSCOMBO])->setCurrentIndex(index);
			this->setAppShortcuts();
		}
}

void KKEditClass::buildDocViewer(void)
{
#ifdef _BUILDDOCVIEWER_
	QVBoxLayout	*docvlayout=new QVBoxLayout;
	QHBoxLayout	*dochlayout=new QHBoxLayout;
	QWidget		*widget;
	QLineEdit	*findbox;
	QPushButton	*button;
	QRect		r;
	QAction		*dump;

	this->docView=new QMainWindow(mainWindow);

	r=this->prefs.value("app/viewergeometry",QVariant(QRect(100,100,800,600))).value<QRect>();
	this->docView->setGeometry(r);
	docvlayout->setContentsMargins(0,0,0,0);
	widget=new QWidget;
	widget->setLayout(docvlayout);
	this->docView->setCentralWidget(widget);

	this->webEngView=new QWebEngineView(widget);

	this->openInNewTab=this->webEngView->pageAction(QWebEnginePage::OpenLinkInNewTab);
	this->openInNewTab->setText("Show In Source File");

//QWebEngineSettings *st=this->webEngViewsettings();
//st->setAttribute();

//don't want these menu items.
	dump=this->webEngView->pageAction(QWebEnginePage::OpenLinkInNewWindow);
	dump->setVisible(false);
	dump=this->webEngView->pageAction(QWebEnginePage::SavePage);
	dump->setVisible(false);
	dump=this->webEngView->pageAction(QWebEnginePage::DownloadLinkToDisk);
	dump->setVisible(false);

	QObject::connect(this->openInNewTab,&QAction::triggered,[this]()
		{

#ifdef _USEQT6_
			const auto & contextMenuData= this->webEngView->lastContextMenuRequest();
#else
			const auto & contextMenuData = this->webEngView->page()->contextMenuData();
#endif

#ifdef _USEQT6_
			if(contextMenuData->linkUrl().isValid())
				{
					//qDebug()<<contextMenuData->linkUrl();
					this->currentURL=contextMenuData->linkUrl().toString();
					bool ret=this->docViewLinkTrap(contextMenuData->linkUrl());//TODO//needs improving

#else
			if(contextMenuData.isValid() && contextMenuData.linkUrl().isValid())
				{
					//qDebug()<<contextMenuData.linkUrl();
					this->currentURL=contextMenuData.linkUrl().toString();
					bool ret=this->docViewLinkTrap(contextMenuData.linkUrl());//TODO//needs improving
#endif
					if(ret==true)
						{
							this->mainWindow->show();
							this->mainWindow->activateWindow();
							this->mainWindow->show();
							this->mainWindow->raise();
						}
				}
		});

	QObject::connect(this->webEngView,&QWebEngineView::urlChanged,[this](const QUrl url)
		{
			this->currentURL=url.toString();
		});

	this->webEngView->setUrl(QUrl("file://" DATADIR "/help/index.html"));

	docvlayout->addWidget(this->webEngView);

	button=new QPushButton(QIcon::fromTheme("go-previous"),"Back");
	QObject::connect(button,&QPushButton::clicked,[=]() {this->webEngView->page()->triggerAction(QWebEnginePage::Back);});

	dochlayout->addSpacing(6);
	dochlayout->addWidget(button);
	dochlayout->addStretch(1);

	findbox=new QLineEdit;
	QObject::connect(findbox,&QLineEdit::returnPressed,[this,findbox]()
		{
			this->htmlURI=QString("https://duckduckgo.com/?q=%1&ia=web").arg(findbox->text());
			this->showWebPage("Results for: " + findbox->text(),this->htmlURI);
		});

	button=new QPushButton(QIcon::fromTheme("edit-find"),"Find");
	QObject::connect(button,&QPushButton::clicked,[this,findbox]()
		{
			this->htmlURI=QString("https://duckduckgo.com/?q=%1&ia=web").arg(findbox->text());
			this->showWebPage("Results for: " + findbox->text(),this->htmlURI);
		});

	dochlayout->addWidget(button);
	dochlayout->addWidget(findbox);

	button=new QPushButton(QIcon::fromTheme("go-down"),"Down");
	QObject::connect(button,&QPushButton::clicked,[this,findbox]()
		{
			this->webEngView->page()->findText(findbox->text());
		});
	dochlayout->addWidget(button);

	button=new QPushButton(QIcon::fromTheme("go-up"),"Up");
	QObject::connect(button,&QPushButton::clicked,[this,findbox]()
		{
			this->webEngView->page()->findText(findbox->text(),QWebEnginePage::FindBackward);
		});
	dochlayout->addWidget(button);

	dochlayout->addStretch(1);

	button=new QPushButton(QIcon::fromTheme("go-next"),"Forward");
	dochlayout->addWidget(button);
	QObject::connect(button,&QPushButton::clicked,[=]() {this->webEngView->page()->triggerAction(QWebEnginePage::Forward);});

	dochlayout->addSpacing(6);
	docvlayout->addLayout(dochlayout);
	docvlayout->addSpacing(6);

	this->docView->hide();
#endif
}

int KKEditClass::yesNoDialog(QString txt,QString info)
{
	QMessageBox	msgBox;

	msgBox.setIcon(QMessageBox::Warning);
	msgBox.setText(txt);
	msgBox.setInformativeText(info);
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	return(msgBox.exec());
}

void KKEditClass::buildToolOutputWindow(void)
{
	QVBoxLayout	*docvlayout=new QVBoxLayout;
	QHBoxLayout	*dochlayout=new QHBoxLayout;
	QWidget		*widget;
	QPushButton	*button;
	QRect		r;

	this->toolOutputWindow=new QMainWindow(mainWindow);

	r=this->prefs.value("app/toolsopgeometry",QVariant(QRect(100,100,800,600))).value<QRect>();
	this->toolOutputWindow->setGeometry(r);

	docvlayout->setContentsMargins(0,0,0,0);
	widget=new QWidget;
	widget->setLayout(docvlayout);
	this->toolOutputWindow->setCentralWidget(widget);

	this->toolsOPText=new QPlainTextEdit(this->mainWindow);
	docvlayout->addWidget(this->toolsOPText);

	button=new QPushButton(QIcon::fromTheme("edit-copy"),"Copy To Clipboard");
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
		if(this->toolsOPText->textCursor().hasSelection()==true)
			this->toolsOPText->copy();
		else
			{
				QTextCursor savetc=this->toolsOPText->textCursor();
				this->toolsOPText->selectAll();
				this->toolsOPText->copy();
				this->toolsOPText->setTextCursor(savetc);
			}
		});
	dochlayout->addStretch(1);
	dochlayout->addWidget(button);

	button=new QPushButton(QIcon::fromTheme("window-close"),"Close");
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			this->toolOutputWindow->hide();
		});
	dochlayout->addStretch(1);
	dochlayout->addWidget(button);

	dochlayout->addStretch(1);

	docvlayout->addLayout(dochlayout);
	docvlayout->addSpacing(6);
	this->toolOutputWindow->hide();
}

void KKEditClass::buildPlugPrefs(void)
{
	QVBoxLayout				*docvlayoutlocal=new QVBoxLayout;
	QVBoxLayout				*docvlayoutsystem=new QVBoxLayout;
	QVBoxLayout				*docvlayout=new QVBoxLayout;
	QHBoxLayout				*dochlayout;
	QPushButton				*btn;
	QCheckBox				*cb;
	QVector<QPushButton*>	settingsbutton;
	QVector<QPushButton*>	aboutbutton;
	pluginStruct				p1;
	pluginStruct				p2;
	bool						flag=false;
	int						cnt=0;
	int						maxcnt=this->plugins.count()-2;	
		
	this->pluginPrefsWindow=new QDialog(mainWindow);
	this->pluginPrefsWindow->setWindowTitle("Plugin Prefs");

	do
		{
			flag=false;
			if(cnt>maxcnt)
				{
					flag=false;
					continue;
				}
			else
				flag=true;

			p1=this->plugins.value(cnt);
			p2=this->plugins.value(cnt+1);
			if(p1.plugName.compare(p2.plugName)>0)
				{
					flag=true;
					this->plugins[cnt+1]=p1;
					this->plugins[cnt]=p2;
					cnt=0;
					continue;
				}
			cnt++;
		}
	while(flag==true);
	
	for(int j=0;j<this->plugins.count();j++)
		{
			dochlayout=new QHBoxLayout;
			cb=new QCheckBox(this->plugins[j].plugName);
			if(this->plugins[j].broken==true)
				{
					cb->setEnabled(false);
				}
			else
				{
					cb->setChecked(this->plugins[j].loaded);
					QObject::connect(cb,&QCheckBox::clicked,[this,j](bool checked)
						{
							this->plugins[j].statusChanged=true;
						});

				}
			dochlayout->addWidget(cb);
			dochlayout->addStretch(128);

			btn=new QPushButton("Settings");
			settingsbutton.push_back(btn);
			dochlayout->addWidget(btn);
			if(((this->plugins[j].wants & DOSETTINGS)!=DOSETTINGS) || (this->plugins[j].loaded==false))
				btn->setEnabled(false);
			QObject::connect(btn,&QPushButton::clicked,[this,j]()
				{
					this->plugins[j].instance->plugSettings();
				});
			
			btn=new QPushButton("About");
			aboutbutton.push_back(btn);
			if(((this->plugins[j].wants & DOABOUT)!=DOABOUT) || (this->plugins[j].loaded==false))
				btn->setEnabled(false);
			QObject::connect(btn,&QPushButton::clicked,[this,j]()
				{
					this->plugins[j].instance->plugAbout();
				});
			dochlayout->addWidget(btn);
			if(this->plugins[j].local==false)
				docvlayoutsystem->addLayout(dochlayout);
			else
				docvlayoutlocal->addLayout(dochlayout);
		}

	dochlayout=new QHBoxLayout;

	btn=new QPushButton("Apply");
	QObject::connect(btn,&QPushButton::clicked,[this,settingsbutton,aboutbutton]()
		{
			for(int j=0;j<this->plugins.count();j++)
				{
					if(this->plugins[j].statusChanged==true)
						{
							if(this->plugins[j].loaded==true)
								{
									this->unloadPlug(&this->plugins[j]);
									if(this->safeFlag==false)
										this->disabledPlugins<<this->plugins[j].plugPath;
								}
							else
								{
									if(this->safeFlag==false)
										this->disabledPlugins.replaceInStrings(this->plugins[j].plugPath,"");
									this->loadPlug(&this->plugins[j],true);
								}
							aboutbutton.at(j)->setEnabled((bool)(this->plugins[j].wants & DOABOUT) && (this->plugins[j].loaded));
							settingsbutton.at(j)->setEnabled((bool)(this->plugins[j].wants & DOSETTINGS) && (this->plugins[j].loaded));
						}
					this->setToolbarSensitive();
				}
			this->writeExitData();
		});
	dochlayout->addWidget(btn);

	btn=new QPushButton("Reload");
	QObject::connect(btn,&QPushButton::clicked,[this,settingsbutton,aboutbutton]()
		{
			for(int j=0;j<this->plugins.count();j++)
				{
					bool hold=this->plugins[j].loaded;
					if(hold==true)
						{
							this->unloadPlug(&this->plugins[j]);
							this->loadPlug(&this->plugins[j],true);
						}
					aboutbutton.at(j)->setEnabled((bool)(this->plugins[j].wants & DOABOUT) && (this->plugins[j].loaded));
					settingsbutton.at(j)->setEnabled((bool)(this->plugins[j].wants & DOSETTINGS) && (this->plugins[j].loaded));
				}
		});
	dochlayout->addWidget(btn);

	btn=new QPushButton("Close");
	QObject::connect(btn,&QPushButton::clicked,[this]()
		{
			this->pluginPrefsWindow->hide();
		});
	dochlayout->addWidget(btn);
	QLabel	*widgetlabel;

	widgetlabel=new QLabel;
	widgetlabel->setText("Local Plugins");

	QHBoxLayout *hbox=new QHBoxLayout;
    hbox->addStretch(1);
	hbox->addWidget(widgetlabel);
	hbox->addStretch(1);
	docvlayout->addLayout(hbox,0);
	docvlayout->addLayout(docvlayoutlocal);
	widgetlabel=new QLabel;
	widgetlabel->setFrameStyle(QFrame::Sunken | QFrame::HLine);
	docvlayout->addWidget(widgetlabel);
	widgetlabel=new QLabel;
	widgetlabel->setText("Global Plugins");
	hbox=new QHBoxLayout;
    hbox->addStretch(1);
	hbox->addWidget(widgetlabel);
	hbox->addStretch(1);
	docvlayout->addLayout(hbox,0);

	docvlayout->addLayout(docvlayoutsystem);

	widgetlabel=new QLabel;
	widgetlabel->setFrameStyle(QFrame::Sunken | QFrame::HLine);
	docvlayout->addWidget(widgetlabel);

	docvlayout->addLayout(dochlayout);
	this->pluginPrefsWindow->setLayout(docvlayout);
}

void KKEditClass::rebuildFunctionMenu(int tab)
{
	DocumentClass			*doc=NULL;
	MenuItemClass			*menuitem;
	int						linenumber;
	QString					label="";
	QString					entrytype="";
	QHash<QString,QMenu*>	menus;
	tagClass					tc(this);

	if(this->sessionBusy==true)
		return;

	this->funcMenu->clear();
	this->funcMenu->setEnabled(false);

	if(tab==-1)
		doc=this->getDocumentForTab(-1);
	else
		doc=qobject_cast<DocumentClass*>(this->mainNotebook->widget(tab));
	if(doc==0)
		return;
	if(doc==NULL)
		return;

	tc.getTagList(QStringList()<<doc->getFilePath());
	if(tc.tagList.count()==0)
		{
			this->funcMenu->setEnabled(false);
		}
	else
		{
			for(int j=0;j<tc.tagList.count();j++)
				{
					linenumber=tc.tagList.at(j).lineNumber;
					label=tc.tagList.at(j).tagDefine;
					entrytype=tc.tagList.at(j).tagType;

					if(entrytype.isEmpty()==false)
						{
							if(entrytype.contains("enumerator",Qt::CaseInsensitive)==false)
								{
									if(this->prefsFunctionMenuLayout==4)
										{
											entrytype=entrytype.left(1).toUpper()+entrytype.mid(1) +"s";
											if(label.length()<2)
												{
													entrytype="Anonymous Functions";
													label=QString("%1 Line %2").arg(doc->getFileName()).arg(linenumber);
												}
											menuitem=new MenuItemClass(this->truncateWithElipses(label,this->prefsMaxMenuChars));
										}
									else
										{
											if(label.length()<2)
												menuitem=new MenuItemClass(QString("ANONYMOUS %1 Line %2").arg(doc->getFileName()).arg(linenumber));
											else
												menuitem=new MenuItemClass(this->truncateWithElipses(entrytype.toUpper() + " " +label,this->prefsMaxMenuChars));
										}

									menuitem->setMenuID(linenumber);
									menuitem->mainKKEditClass=this;
									QObject::connect(menuitem,SIGNAL(triggered()),menuitem,SLOT(menuClickedGotoLine()));			
									if(this->prefsFunctionMenuLayout==4)
										{
											if(menus.contains(entrytype)==false)
												{
													menus[entrytype]=new QMenu(entrytype);
													this->funcMenu->addMenu(menus.value(entrytype));
												}
											menus.value(entrytype)->addAction(menuitem);
										}
									else
										{
											this->funcMenu->addAction(menuitem);
										}
								}
						}
				}
			this->funcMenu->setEnabled(true);
		}
}



