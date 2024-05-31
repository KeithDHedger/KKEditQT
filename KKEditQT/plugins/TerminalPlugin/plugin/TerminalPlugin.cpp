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
	QAction		*act;
	QStringList	themenames=QTermWidget::availableColorSchemes();
	int			what=1;
	QString		dwss="QDockWidget::title {background: grey;padding-left: 0px;padding-top: 0px;padding-bottom: 0px;}\nQDockWidget {font-size: 10pt;}";

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

	this->plugPrefs=new QSettings("KDHedger","TerminalPlugin");
	this->cbnum=this->plugPrefs->value("themenumber").toInt();

	themenames.sort();

	this->TerminalPluginMenu=new QMenu("TerminalPlugin");
	this->TerminalPluginMenu->setIcon(QIcon(QString("%1/TerminalPlugin.png").arg(QFileInfo(pathtoplug).absolutePath())));
	this->mainKKEditClass->pluginMenu->addMenu(TerminalPluginMenu);

	this->console=new QTermWidget(0,NULL);
	this->console->setScrollBarPosition(QTermWidget::ScrollBarRight);
	this->console->setWorkingDirectory(QString("/tmp"));
	this->console->setColorScheme(themenames.at(this->cbnum));
	this->openOnStart=this->plugPrefs->value("openonstart").toBool();

	this->dw=new QDockWidget(this->mainKKEditClass->mainWindow);
	this->dw->setStyleSheet(dwss);
	this->dw->setVisible(this->openOnStart);
    dw->setWidget(console);
	act=dw->toggleViewAction();
	act->setText("Toggle Terminal");
	//dw->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);//TODO in lfswm2//
	dw->setFeatures(QDockWidget::DockWidgetClosable);
	what=1;
	QObject::connect(act,&QAction::triggered,[this,what]() { this->doMenuItem(what); });
	this->TerminalPluginMenu->addAction(act);
 
	act=new QAction("CD to Doc Folder ...");
	this->TerminalPluginMenu->addAction(act);
	what=2;
	QObject::connect(act,&QAction::triggered,[this,what]() { this->doMenuItem(what); });

	this->mainKKEditClass->mainWindow->addDockWidget(Qt::BottomDockWidgetArea,dw);
	this->console->startShellProgram ();
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
			this->console->setColorScheme(themebox.currentText());
			this->cbnum=themebox.currentIndex();
			this->plugPrefs->setValue("themenumber",this->cbnum);
			this->plugPrefs->setValue("openonstart",this->openOnStart);
		}
}

unsigned int TerminalPluginPlug::plugWants(void)
{
	return(DOABOUT|DOSETTINGS|DOSWITCHPAGE);
}

void TerminalPluginPlug::plugRun(plugData *data)
{
	qDebug()<<"plugRun called";
	qDebug()<<"Name: "<<data->plugName;
	qDebug()<<"Version: "<<data->plugVersion;
	qDebug()<<"What to do: "<<data->what;
	qDebug()<<data->userStrData1;
	qDebug()<<data->userStrData2;
	qDebug()<<data->userStrData3;
	this->filePath=data->userStrData1;
	this->folderPath=data->userStrData3;
	if(data->what==DOSWITCHPAGE)
		this->dw->setWindowTitle(this->filePath);
}

void TerminalPluginPlug::doMenuItem(int what)
{
	switch(what)
		{
			case 1:
				qDebug()<<"Toggle term action ...";
				qDebug()<<this->dw->isVisible();//TODO//
				break;
			case 2:
				this->console->changeDir(this->folderPath);
				break;
		}
}

