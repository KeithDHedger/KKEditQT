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

bool					singleOverRide=false;
bool					loadPluginsFlag=true;
KKEditClass				*kkedit;

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
	int					status;
	bool				safeflag=false;
	bool				retval=false;
	QDir				commsDir;
	QApplication		app(argc,argv);

	app.setStyleSheet("QMenu { menu-scrollable: true ;}");
	app.setOrganizationName("KDHedger");
	app.setApplicationName("KKEditQT");

	singleOverRide=false;
	loadPluginsFlag=true;

	if(safeflag==true)//TODO//
		{
			singleOverRide=true;
			loadPluginsFlag=false;
		}

	kkedit=new KKEditClass(&app);
	kkedit->parser.addHelpOption();

	kkedit->parser.addOptions(
		{
			{{"m","multi"},"Force multiple instance."},
			{{"q","quit"},"Quit app."},
			{{"s","restore-session"},"SessionName","Open session by name."}
		});

	kkedit->parser.process(app);

	SingleInstanceClass siapp(&app,kkedit->parser.isSet("multi"));

	if(siapp.getRunning()==true)
		{
			kkedit->runCLICommands(siapp.queueID);
			return(0);
		}

	kkedit->queueID=siapp.queueID;
	kkedit->forcedMultInst=kkedit->parser.isSet("multi");
	kkedit->currentWorkSpace=siapp.workspace;;
	kkedit->sessionID=kkedit->currentWorkSpace+MSGKEY;
	kkedit->forceDefaultGeom=!siapp.isOnX11;

	kkedit->initApp(argc,argv);
//load plugins
//	loadPlugins();
//test plugs
#if 0
	for(int j=0;j<kkedit->plugins.count();j++)
		{
			DEBUGSTR(kkedit->plugins[j].plugName);
			DEBUGSTR(kkedit->plugins[j].plugVersion);
			DEBUGSTR(kkedit->plugins[j].plugPath);
			DEBUGSTR(kkedit->plugins[j].wants);
			DEBUGSTR(kkedit->plugins[j].loaded);
			if(kkedit->plugins[j].plugName.compare("Example Plugin")==0)
				{
					kkedit->plugins[j].instance->unloadPlug();
					kkedit->plugins[j].pluginLoader->unload();
					kkedit->plugins[j].loaded=false;
				}
		}
#endif
	kkedit->runCLICommands(kkedit->queueID);

	kkedit->setToolbarSensitive();

	if(getuid()!=0)
		app.setWindowIcon(QIcon(DATADIR "/pixmaps/" PACKAGE ".png"));
	else
		app.setWindowIcon(QIcon(DATADIR"/pixmaps/KKEditRoot.png"));

	status=app.exec();

	delete kkedit;
	return status;
}


