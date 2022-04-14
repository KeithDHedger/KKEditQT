/*
 *
 * ©K. D. Hedger. Wed 16 Mar 14:08:28 GMT 2022 keithdhedger@gmail.com

 * This file (gtk2toolkit.h) is part of gtk2toolkit.

 * gtk2toolkit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * gtk2toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with gtk2toolkit.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GTK2TOOLKIT
#define GTK2TOOLKIT

#include "../../../src/ToolkitHighlitePluginInterface.h"

class gtk2toolkit : public QObject, ToolkitHighlitePluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.ToolkitHighlitePluginInterface" FILE "gtk2toolkit.json")
	Q_INTERFACES(ToolkitHighlitePluginInterface)

	public:
		void						initPlug(QString pathtoplug) override;
		void						unloadPlug(void) override;
//theme
		virtual void				setTheme(QHash<int,themeStruct> newtheme) override;
//new rules format
		void						setToolkitRules(QVector<highLightingRule> *rules) override;

	private:
		QString					plugPath;
		QHash<int,themeStruct>	theme;
};

#endif





