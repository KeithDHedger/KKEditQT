/*
 *
 * ©K. D. Hedger. Sat 26 Mar 15:11:45 GMT 2022 keithdhedger@gmail.com

 * This file (PLUGNAME.h) is part of PLUGNAME.

 * PLUGNAME is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * PLUGNAME is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with PLUGNAME.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PLUGNAMEPLUG_H
#define PLUGNAMEPLUG_H

#include "../../../src/kkeditqtPluginInterface.h"

class PLUGNAMEPlug : public QObject, kkEditQTPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.kkEditQTPluginInterface" FILE "PLUGNAME.json")
	Q_INTERFACES(kkEditQTPluginInterface)

	public:
		void						initPlug(KKEditClass *kk,QString pathtoplug) override;
		void						unloadPlug(void) override;
		void						plugAbout(void) override;
		void						plugSettings(void) override;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
		void						plugRun(plugData *data) override;
#pragma GCC diagnostic pop
		unsigned int				plugWants(void) override;

	private:
		KKEditClass				*mainKKEditClass;
		QString					plugPath;
		QMenu					*PLUGNAMEMenu;
		QAction					*PLUGNAMEaction;
		QMetaObject::Connection	PLUGNAMEcon;
};

#endif
