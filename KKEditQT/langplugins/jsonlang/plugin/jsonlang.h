/*
 *
 * ©K. D. Hedger. Thu 17 Mar 16:25:08 GMT 2022 keithdhedger@gmail.com

 * This file (jsonlang.h) is part of jsonlang.

 * jsonlang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * jsonlang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with jsonlang.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JSONLANG_H
#define JSONLANG_H

#include "../../../src/SyntaxHighlitePluginInterface.h"

class jsonlang : public QObject, SyntaxHighlitePluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.SyntaxHighlitePluginInterface" FILE "jsonlang.json")
	Q_INTERFACES(SyntaxHighlitePluginInterface)

	public:
		void							initPlug(QString pathtoplug) override;
		void							unloadPlug(void) override;
//new rules format
		void							setLanguageRules(QVector<highLightingRule> *rules) override;
//theme
		void							setTheme(QMap<QString,partsStruct> newtheme) override;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
//multi line rules
		void							setMultLineRules(QVector<highLightingRule> *rules) override {};
//custom code for edge cases etc
		void							runCustomRule(QString text,highLightingRule *hr) override {};
#pragma GCC diagnostic pop
	
	private:
		QString						plugPath;
		QMap<QString,partsStruct>	theme;
};

#endif


