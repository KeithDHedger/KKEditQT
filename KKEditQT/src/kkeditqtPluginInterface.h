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

enum whatIWant {DONONE,DOSAVE,DOLOAD,DOCLOSE,DORESSESSION,DOSAVESESSION,DOCONTEXTMENU,DOTABPOPUP};

class KKEditClass;

#include "kkedit-includes.h"

class kkEditQTPluginInterface
{
	public:
		virtual ~kkEditQTPluginInterface()=default;

		virtual void		initPlug(KKEditClass *kk,QString pathtoplug)=0;
		virtual void		unloadPlug(void)=0;
		virtual void		plugAbout(void)=0;
		virtual void		plugSettings(void)=0;
		virtual void		plugRun(whatIWant)=0;
		virtual void		plugAddToContextMenu(QMenu *menu,DocumentClass *document)=0;
		virtual whatIWant	plugWants(void)=0;
};

QT_BEGIN_NAMESPACE

#define kkEditQTPluginInterface_iid "org.KKEditQT.kkEditQTPluginInterface"

Q_DECLARE_INTERFACE(kkEditQTPluginInterface, kkEditQTPluginInterface_iid)
QT_END_NAMESPACE

#endif
