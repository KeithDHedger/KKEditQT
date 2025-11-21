/*
 *
 * ©K. D. Hedger. Fri 21 Nov 13:02:14 GMT 2025 keithdhedger@gmail.com

 * This file (qttoolkit.h) is part of qttoolkit.

 * qttoolkit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * qttoolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with qttoolkit.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QTTOOLKIT_H
#define QTTOOLKIT_H

#include "../../../src/ToolkitHighlitePluginInterface.h"

class qttoolkit : public QObject, ToolkitHighlitePluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.ToolkitHighlitePluginInterface" FILE "qttoolkit.json")
	Q_INTERFACES(ToolkitHighlitePluginInterface)

	public:
		void							initPlug(QString pathtoplug) override;
		void							unloadPlug(void) override;
//theme
		void							setTheme(QMap<QString,partsStruct> newtheme) override;
//new rules format
		void							setToolkitRules(QVector<highLightingRule> *rules) override;

	private:
		QString						plugPath;
		QMap<QString,partsStruct>	theme;
};

#endif
