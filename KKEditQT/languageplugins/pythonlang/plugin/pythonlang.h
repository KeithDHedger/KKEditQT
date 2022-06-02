/*
 *
 * ©K. D. Hedger. Wed 16 Mar 14:29:04 GMT 2022 keithdhedger@gmail.com

 * This file (pythonlang.h) is part of pythonlang.

 * pythonlang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * pythonlang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with pythonlang.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PYTHONLANG_H
#define PYTHONLANG_H

#include "../../../src/SyntaxHighlitePluginInterface.h"

class pythonlang : public QObject, SyntaxHighlitePluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.SyntaxHighlitePluginInterface" FILE "pythonlang.json")
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


