/*
 *
 * ©K. D. Hedger. Sat  5 Mar 16:01:08 GMT 2022 keithdhedger@gmail.com

 * This file (kkeditqtPluginExample.h) is part of kkeditqtPluginExample.

 * kkeditqtPluginExample is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * kkeditqtPluginExample is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with kkeditqtPluginExample.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KKEDITQTEXAMPLEPLUG_H
#define KKEDITQTEXAMPLEPLUG_H

#include "../../../src/kkeditqtPluginInterface.h"

class kkeditQTexamplePlug : public QObject, kkEditQTPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.Example.kkEditQTPluginInterface" FILE "kkeditqtPluginExample.json")
	Q_INTERFACES(kkEditQTPluginInterface)

	public:
		void			initPlug(KKEditClass *kk,QString pathtoplug) override;
		void			unloadPlug(void) override;
		void			plugAbout(void) override;
		void			plugSettings(void) override;
		void			plugRun(plugData *data) override;
		unsigned int	plugWants(void) override;

	private:
		KKEditClass		*mainKKEditClass=NULL;
		QString			plugPath="";
		QAction			*exampleMenuitem=NULL;
};

#endif
