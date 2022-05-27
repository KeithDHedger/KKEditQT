/*
 *
 * ©K. D. Hedger. Sun 13 Mar 18:41:15 GMT 2022 keithdhedger@gmail.com

 * This file (shlang.h) is part of shlang.

 * shlang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * shlang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with shlang.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SHLANG_H
#define SHLANG_H

#include "../../../src/SyntaxHighlitePluginInterface.h"

class shlang : public QObject, SyntaxHighlitePluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.SyntaxHighlitePluginInterface" FILE "shlang.json")
	Q_INTERFACES(SyntaxHighlitePluginInterface)

	public:
		void						initPlug(QString pathtoplug) override;
		void						unloadPlug(void) override;
//theme
		void						setTheme(QHash<int,themeStruct> newtheme) override;
//new rules format
		void						setLanguageRules(QVector<highLightingRule> *rules) override;
//custom code for edge cases etc
		void						runCustomRule(QString text,highLightingRule *hr) override;
//multi line rules
		void						setMultLineRules(QVector<highLightingRule> *rules) override;

	private:
		QString					plugPath;
		QHash<int,themeStruct>	theme;
};

#endif


