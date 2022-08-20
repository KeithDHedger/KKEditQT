/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:38:16 GMT 2021 keithdhedger@gmail.com

 * This file (main.cpp) is part of KKEditQT.

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

#include "kkedit-includes.h"
#include "SingleInstanceClass.h"

KKEditClass	*kkedit;

//unsigned long hashFromKey(const char *key)
//{
//	unsigned long hash=0;
//
//	for(unsigned i=0;key[i]!=0;i++)
//		hash=31*hash+key[i];
//
//	return(hash);
//}

int main (int argc, char **argv)
{
	int				status;
	QDir				commsDir;
	QApplication		app(argc,argv);
	QPixmap			pixmap(DATADIR "/pixmaps/KKEditQT.png");

	app.setOrganizationName("KDHedger");
	app.setApplicationName("KKEditQT");

	kkedit=new KKEditClass(&app);
    kkedit->splash=new QSplashScreen(pixmap,Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint);
	kkedit->parser.addHelpOption();

	kkedit->parser.addOptions(
		{
			{{"k","key"},"Force key ID.","KeyID"},
			{{"m","multi"},"Force multiple instance."},
			{{"s","safe"},"Safe mode ( no plugins, no plugin enable/disable data written )."},
			{{"v","very-safe"},"Very safe mode ( no plugins, no highlighting/themes, no prefs data written )."},
			{{"q","quit"},"Quit app."},
			{{"r","restore-session"},"Open session by name.","SessionName"}
	});

	kkedit->parser.process(app);
	if(kkedit->parser.isSet("key"))
		kkedit->sessionID=kkedit->parser.value("key").toInt(nullptr,0);

	if(kkedit->parser.isSet("safe"))
		kkedit->safeFlag=true;

	if(kkedit->parser.isSet("very-safe"))
		{
			kkedit->safeFlag=true;
			kkedit->verySafeFlag=true;
		}

	SingleInstanceClass siapp(&app,kkedit->sessionID,kkedit->parser.isSet("multi"));
	if(siapp.getRunning()==true)
		{
			kkedit->runCLICommands(siapp.queueID);
			return(0);
		}

	kkedit->splash->show();
	kkedit->queueID=siapp.queueID;
	kkedit->forcedMultInst=kkedit->parser.isSet("multi");
	kkedit->currentWorkSpace=siapp.workspace;
	kkedit->sessionID=siapp.useKey;
	kkedit->forceDefaultGeom=!siapp.isOnX11;

	kkedit->initApp(argc,argv);
	app.setStyleSheet(kkedit->prefsMenuStyleString);
//QMenu{menu-scrollable: true;padding: 0px;margin: 0px}
//test plugs
#if 0
#ifdef _DEBUGCODE_
	//qSetMessagePattern("[%{type}] %{appname} (%{file}->%{function}->%{line}) - %{message}");
	for(int j=0;j<kkedit->plugins.count();j++)
		kkedit->plugins[j].printIt();
#endif
#endif

	kkedit->runCLICommands(kkedit->queueID);

	kkedit->setToolbarSensitive();

	if(getuid()!=0)
		app.setWindowIcon(QIcon(DATADIR "/pixmaps/" PACKAGE ".png"));
	else
		app.setWindowIcon(QIcon(DATADIR"/pixmaps/KKEditRoot.png"));

	kkedit->splash->finish(kkedit->mainWindow);

	status=app.exec();

	delete kkedit;
	return status;
}


