/*
 *
 * ©K. D. Hedger. Sat 26 Mar 15:11:45 GMT 2022 keithdhedger@gmail.com

 * This file (Favourites.h) is part of Favourites.

 * Favourites is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * Favourites is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Favourites.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FAVOURITESPLUG_H
#define FAVOURITESPLUG_H

#include "globalincludes.h"

#define MAXFAVMENULEN 48
#define MAXFAVS 32

class FavouritesPlug : public QObject, kkEditQTPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.kkEditQTPluginInterface" FILE "Favourites.json")
	Q_INTERFACES(kkEditQTPluginInterface)

	public:
		void						initPlug(KKEditClass *kk,QString pathtoplug) override;
		void						unloadPlug(void) override;
		void						plugAbout(void) override;
		void						plugSettings(void) override;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
		void						plugRun(plugData *data) override {};
#pragma GCC diagnostic pop
		unsigned int				plugWants(void) override;

	private:
		QString					truncateWithElipses(const QString str);
		void						doAction(QString data);
		KKEditClass				*mainKKEditClass=NULL;
		QString					plugPath="";
		QMenu					*favouritesMenu=NULL;
		QHash<int,QAction*>		favourites;
		QMetaObject::Connection	favCon;
};

#endif


