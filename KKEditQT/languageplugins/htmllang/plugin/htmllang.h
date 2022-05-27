/*
 *
 * ©K. D. Hedger. Wed 16 Mar 11:12:24 GMT 2022 keithdhedger@gmail.com

 * This file (htmllang.h) is part of htmllang.

 * htmllang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * htmllang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with htmllang.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HTMLLANG_H
#define HTMLLANG_H

#include "../../../src/SyntaxHighlitePluginInterface.h"

class htmllang : public QObject, SyntaxHighlitePluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.SyntaxHighlitePluginInterface" FILE "htmllang.json")
	Q_INTERFACES(SyntaxHighlitePluginInterface)

	public:
		void						initPlug(QString pathtoplug) override;
		void						unloadPlug(void) override;
//theme
		void						setTheme(QHash<int,themeStruct> newtheme) override;
//new rules format
		void						setLanguageRules(QVector<highLightingRule> *rules) override;
//multi line rules
		void						setMultLineRules(QVector<highLightingRule> *rules) override;
//custom code for edge cases etc
		void						runCustomRule(QString text,highLightingRule *hr) override {};

	private:
		QString					plugPath;
		QHash<int,themeStruct>	theme;
};

#endif


