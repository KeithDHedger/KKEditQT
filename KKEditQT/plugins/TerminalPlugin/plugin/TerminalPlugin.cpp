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

void TerminalPluginPlug::initPlug(KKEditClass *kk,QString pathtoplug)
{
	QStringList	themenames=QTermWidget::availableColorSchemes();
	int			what=1;
	QString		dwss="QDockWidget::title {background: grey;padding-left: 0px;padding-top: 0px;padding-bottom: 0px;}\nQDockWidget {font-size: 10pt;}";
	QSettings	plugprefs("KDHedger","TerminalPlugin");

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

	this->cbnum=plugprefs.value("themenumber").toInt();

	themenames.sort();

	this->TerminalPluginMenu=new QMenu("TerminalPlugin");
	this->TerminalPluginMenu->setIcon(QIcon(QString("%1/TerminalPlugin.png").arg(QFileInfo(pathtoplug).absolutePath())));
	this->mainKKEditClass->pluginMenu->addMenu(TerminalPluginMenu);

	this->console=new QTermWidget(0,NULL);
	this->console->setScrollBarPosition(QTermWidget::ScrollBarRight);
	//this->console->setWorkingDirectory(QString("~"));
	this->console->setColorScheme(themenames.at(this->cbnum));
	this->openOnStart=plugprefs.value("openonstart").toBool();
	this->saveCurrentVis=plugprefs.value("savevis").toBool();

	this->dw=new QDockWidget(this->mainKKEditClass->mainWindow);
	this->dw->setStyleSheet(dwss);
	this->dw->setFloating(false);
	if((this->openOnStart==true) && (this->saveCurrentVis==true))
		{
			this->dw->setVisible(plugprefs.value("currentstate").toBool());
			this->dw->setFloating(plugprefs.value("floating").toBool());
		}
	else
		this->dw->setVisible(this->openOnStart);

	this->dw->setWidget(console);
	what=1;
	QObject::connect(this->dw,&QDockWidget::visibilityChanged,[this,what](bool) { this->doMenuItem(what); });

	this->toggleViewAct=dw->toggleViewAction();
	this->toggleViewAct->setText("Toggle Terminal");
	what=1;
	QObject::connect(this->toggleViewAct,&QAction::triggered,[this,what]() { this->doMenuItem(what); });
	this->TerminalPluginMenu->addAction(this->toggleViewAct);
 
	this->cdToAct=new QAction("CD to Doc Folder ...");
	this->TerminalPluginMenu->addAction(this->cdToAct);
	what=2;
	QObject::connect(this->cdToAct,&QAction::triggered,[this,what]() { this->doMenuItem(what); });

	this->mainKKEditClass->mainWindow->addDockWidget(Qt::BottomDockWidgetArea,dw);
	this->console->startShellProgram ();
	if(this->dw->isFloating()==true)
		{
			if((plugprefs.value("geom").toRect().width()==0) || (plugprefs.value("geom").toRect().height()==0))
				this->dw->setFloating(false);
			else
				this->dw->setGeometry(plugprefs.value("geom").toRect());
		}
}

void TerminalPluginPlug::unloadPlug(void)
{
	delete this->TerminalPluginMenu;
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
	QObject::connect(openonstart,&QCheckBox::stateChanged,[this](bool what) { this->openOnStart=what; });
	vlayout->addWidget(openonstart);

	savevis=new QCheckBox("Save Current State");
	savevis->setChecked(this->saveCurrentVis);
	QObject::connect(savevis,&QCheckBox::stateChanged,[this](bool what) { this->saveCurrentVis=what; });
	vlayout->addWidget(savevis);

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

	this->filePath=data->userStrData1;
	this->folderPath=data->userStrData3;

	if(data->what==DOSHUTDOWN)
		{	
 			plugprefs.setValue("floating",this->dw->isFloating());
 			plugprefs.setValue("geom",this->dw->geometry());
 			plugprefs.setValue("currentstate",this->dw->isVisible());
		}
		
	if(data->what==DOSWITCHPAGE)
		{
			this->dw->setWindowTitle(this->filePath);
			this->toggleViewAct->setText("Toggle Terminal");
		}
	if(data->what==DOCONTEXTMENU)
		data->menu->addAction(this->cdToAct);
}

void TerminalPluginPlug::doMenuItem(int what)
{
	QSettings plugprefs("KDHedger","TerminalPlugin");
	switch(what)
		{
			case 1:
				plugprefs.setValue("currentstate",this->dw->isVisible());
				break;
			case 2:
				this->console->changeDir(this->folderPath);
				this->toggleViewAct->setText("Toggle Terminal");
				break;
		}
}

