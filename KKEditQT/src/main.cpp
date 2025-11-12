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

#include "globalincludes.h"
#include "KKEditClass.h"
#include "SingleInstanceClass.h"

KKEditClass	*kkedit=NULL;

void signalHandler(int signalNum)
{
	kkedit->handleSignal(signalNum);
}

int main (int argc, char **argv)
{
	int				status;
	QDir				commsDir;
	QApplication		*napp=new QApplication(argc,argv);

	signal(SIGUSR1,signalHandler);
	signal(SIGTERM,signalHandler);
	signal(SIGINT,signalHandler);
	
	napp->setOrganizationName("KDHedger");
	napp->setApplicationName("KKEditQT");
	QIcon::setFallbackThemeName("gnome");

	kkedit=new KKEditClass(napp);
    kkedit->splash=new QSplashScreen(QString(DATADIR)+"/pixmaps/KKEditQT.png",Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint);

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

	kkedit->parser.process(kkedit->application->arguments());

	if(kkedit->parser.isSet("key"))
		kkedit->sessionID=kkedit->parser.value("key").toInt(nullptr,0);

	if(kkedit->parser.isSet("safe"))
		kkedit->safeFlag=true;

	if(kkedit->parser.isSet("very-safe"))
		{
			kkedit->safeFlag=true;
			kkedit->verySafeFlag=true;
		}

	if(kkedit->parser.isSet("multi"))
		{
			srandom(time(NULL));
			kkedit->sessionID=random();
		}

	SingleInstanceClass *siapp=new SingleInstanceClass("KKEditQT",kkedit->sessionID);

	fprintf(stderr,"queueAddr=0x%x shmQueueID=0x%x msgKey=0x%x\n",siapp->queueAddr,siapp->shmQueueID,siapp->key);

	if(kkedit->parser.isSet("multi"))
		{
			kkedit->queueID=siapp->queueID;
			kkedit->sessionID=siapp->key;
			kkedit->forceDefaultGeom=!siapp->isOnX11;

			kkedit->initApp(argc,argv);
			kkedit->runCLICommands(siapp->queueID);
			kill(getpid(),SIGUSR1);
			kkedit->application->setWindowIcon(QIcon(DATADIR "/pixmaps/" PACKAGE ".png"));
			siapp->isMulti=true;
			//fprintf(stderr,"queueAddr=0x%x shmQueueID=0x%x msgKey=0x%x\n",siapp->queueAddr,siapp->shmQueueID,siapp->key);
			status=kkedit->application->exec();
			delete kkedit;
			delete siapp;
			return status;
		}

	if(siapp->running==true)
		{
			msgStruct	message;
			int			msglen;

			kkedit->queueID=siapp->queueID;
			kkedit->sessionID=siapp->key;
			msglen=snprintf(message.mText,MAXMSGSIZE-1,"%s","ACTIVATEAPPMSG");
			message.mType=ACTIVATEAPPMSG;
			msgsnd(siapp->queueID,&message,msglen,0);
			kkedit->runCLICommands(siapp->queueID);
			sem_wait(siapp->semid);
				kill(atoi(siapp->queueAddr),SIGUSR1);
			sem_post(siapp->semid);

			delete kkedit;
			delete siapp;
			return(0);
		}
	else
		{
			kkedit->queueID=siapp->queueID;
			kkedit->sessionID=siapp->key;
		}

	kkedit->splash->show();
	kkedit->queueID=siapp->queueID;
	kkedit->forcedMultInst=kkedit->parser.isSet("multi");
	kkedit->sessionID=siapp->key;;
	kkedit->forceDefaultGeom=!siapp->isOnX11;

	kkedit->initApp(argc,argv);

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
		kkedit->application->setWindowIcon(QIcon(DATADIR "/pixmaps/" PACKAGE ".png"));
	else
		kkedit->application->setWindowIcon(QIcon(DATADIR"/pixmaps/KKEditRoot.png"));

	kkedit->splash->finish(kkedit->mainWindow);

	status=kkedit->application->exec();
	delete kkedit;
	delete siapp;
	return status;
}
