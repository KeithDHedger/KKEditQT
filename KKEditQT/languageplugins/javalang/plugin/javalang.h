/*
 *
 * ©K. D. Hedger. Wed 16 Mar 16:33:16 GMT 2022 keithdhedger@gmail.com

 * This file (javalang.h) is part of javalang.

 * javalang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * javalang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with javalang.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JAVALANG_H
#define JAVALANG_H

#include "../../../src/SyntaxHighlitePluginInterface.h"

class javalang : public QObject, SyntaxHighlitePluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.SyntaxHighlitePluginInterface" FILE "javalang.json")
	Q_INTERFACES(SyntaxHighlitePluginInterface)

	public:
		void							initPlug(QString pathtoplug) override;
		void							unloadPlug(void) override;
//theme
		void							setTheme(QMap<QString,partsStruct> newtheme) override;
//new rules format
		void							setLanguageRules(QVector<highLightingRule> *rules) override;
//multi line rules
		void							setMultLineRules(QVector<highLightingRule> *rules) override;
//custom code for edge cases etc
		void							runCustomRule(QString text,highLightingRule *hr) override;

	private:
		QString						plugPath;
		QMap<QString,partsStruct>	theme;
};

#endif


