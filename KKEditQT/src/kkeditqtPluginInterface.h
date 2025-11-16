/*
 *
 * ©K. D. Hedger. Sat  5 Mar 16:00:56 GMT 2022 keithdhedger@gmail.com

 * This file (kkeditqtPluginInterface.h) is part of KKEditQT.

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

#ifndef KKEDITQTPLUGININTERFACE_H
#define KKEDITQTPLUGININTERFACE_H

#define DONONE 0
#define DOSAVE 1
#define DOLOAD DOSAVE << 1
#define DOCLOSE DOLOAD << 1
#define DORESSESSION DOCLOSE << 1
#define DOSAVESESSION DORESSESSION << 1
#define DOCONTEXTMENU DOSAVESESSION << 1
#define DOTABPOPUP DOCONTEXTMENU << 1
#define DOSETSENSITVE DOTABPOPUP << 1
#define DOSWITCHPAGE DOSETSENSITVE << 1
#define DONEWDOCUMENT DOSWITCHPAGE << 1
#define DOSHUTDOWN DONEWDOCUMENT << 1
#define DOABOUT DOSHUTDOWN << 1
#define DOSETTINGS DOABOUT << 1

//show side
//hide side
//show hide top

struct plugData;

#include "globalincludes.h"
#include "internet.h"
#include "QT_document.h"
#include "QT_notebook.h"
#include "KKEditClass.h"

struct plugData
{
	QString			plugName;
	QString			plugPath;
	QString			plugVersion;
	QString			tempFolder;
	QMenu			*menu=NULL;
	DocumentClass	*doc=NULL;//can only use parent functions ( QPlainTextEdit ).
	int				tabNumber=-1;
	bool				enabled=true;
	int				userIntData1=-1;
	int				userIntData2=-1;
	int				userIntData3=-1;
	QString			userStrData1;
	QString			userStrData2;
	QString			userStrData3;
	unsigned int		what=DONONE;
#ifdef _DEBUGCODE_
	void				printIt(void)
		{
			qDebug() << "plugName" << plugName;
			qDebug() << "plugPath" << plugPath;
			qDebug() << "plugVersion" << plugVersion;
			qDebug() << "menu" << menu;
			qDebug() << "doc" << doc;
			qDebug() << "tabNumber" << tabNumber;
			qDebug() << "enabled" << enabled;
			qDebug() << "userIntData1" << userIntData1;
			qDebug() << "userIntData2" << userIntData2;
			qDebug() << "userIntData3" << userIntData3;
			qDebug() << "userStrData1" << userStrData1;
			qDebug() << "userStrData2" << userStrData2;
			qDebug() << "userStrData3" << userStrData3;

			switch(what)
				{
					case DONONE:
						qDebug() << "what DONONE";
						break;
					case DOSAVE:
						qDebug() << "what DOSAVE";
						break;
					case DOLOAD:
						qDebug() << "what DOLOAD";
						break;
					case DOCLOSE:
						qDebug() << "what DOCLOSE";
						break;
					case DORESSESSION:
						qDebug() << "what DORESSESSION";
						break;
					case DOSAVESESSION:
						qDebug() << "what DOSAVESESSION";
						break;
					case DOCONTEXTMENU:
						qDebug() << "what DOCONTEXTMENU";
						break;
					case DOTABPOPUP:
						qDebug() << "what DOTABPOPUP";
						break;
					case DOSETSENSITVE:
						qDebug() << "what DOSETSENSITVE";
						break;
					case DOSWITCHPAGE:
						qDebug() << "what DOSWITCHPAGE";
						break;
					case DONEWDOCUMENT:
						qDebug() << "what DONEWDOCUMENT";
						break;
					case DOSHUTDOWN:
						qDebug() << "what DOSHUTDOWN";
						break;
					case DOABOUT:
						qDebug() << "what DOABOUT";
						break;
					case DOSETTINGS:
						qDebug() << "what DOSETTINGS";
						break;
				}
		};
#endif
};

class kkEditQTPluginInterface
{
	public:
		virtual ~kkEditQTPluginInterface()=default;

		virtual void			initPlug(KKEditClass *kk,QString pathtoplug)=0;
		virtual void			unloadPlug(void)=0;
		virtual void			plugAbout(void)=0;
		virtual void			plugSettings(void)=0;
		virtual void			plugRun(plugData *data)=0;
		virtual unsigned int	plugWants(void)=0;
};

#define kkEditQTPluginInterface_iid "org.KKEditQT.kkEditQTPluginInterface"
Q_DECLARE_INTERFACE(kkEditQTPluginInterface, kkEditQTPluginInterface_iid)

#endif


