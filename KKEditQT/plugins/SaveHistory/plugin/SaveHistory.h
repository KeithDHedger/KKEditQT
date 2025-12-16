/*
 *
 * ©K. D. Hedger. Sat  5 Mar 17:05:31 GMT 2022 keithdhedger@gmail.com

 * This file (SaveHistory.h) is part of SaveHistory.

 * SaveHistory is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * SaveHistory is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with SaveHistory.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OPENURIPLUG_H
#define OPENURIPLUG_H

#include "../../../src/kkeditqtPluginInterface.h"

class SaveHistory : public QObject, kkEditQTPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.kkEditQTPluginInterface" FILE "SaveHistory.json")
	Q_INTERFACES(kkEditQTPluginInterface)

	public:
		void						initPlug(KKEditClass *kk,QString pathtoplug) override;
		void						unloadPlug(void) override;
		void						plugAbout(void) override;
		void						plugSettings(void) override {};
		void						plugRun(plugData *data) override;
		unsigned int				plugWants(void) override;

	private:
		plugData					*data=NULL;
		QString 					homeDataFolder="";
		QString					docName="";
		QString					docDir="";
		KKEditClass				*mainKKEditClass=NULL;
		QString					plugPath="";
		QAction					*saveHistoryMenuitem=NULL;
		QAction					*openHistoryMenuitem=NULL;
		QMenu					*openHistoryListMenuitem=NULL;
		QMetaObject::Connection	clearMenuCon;
		QMetaObject::Connection	openMenuCon;
		QMetaObject::Connection	listMenuCon;

		void						clearHistory(void);
		void						openHistoryFolder(void);
		void						showHistoryList(void);
};

#endif


