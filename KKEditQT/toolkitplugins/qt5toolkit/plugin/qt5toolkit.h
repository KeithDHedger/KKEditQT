/*
 *
 * ©K. D. Hedger. Wed 16 Mar 14:08:28 GMT 2022 keithdhedger@gmail.com

 * This file (qt5toolkit.h) is part of qt5toolkit.

 * qt5toolkit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * qt5toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with qt5toolkit.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef QT5TOOLKIT_H
#define QT5TOOLKIT_H

#include "../../../src/ToolkitHighlitePluginInterface.h"

class qt5toolkit : public QObject, ToolkitHighlitePluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.ToolkitHighlitePluginInterface" FILE "qt5toolkit.json")
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




