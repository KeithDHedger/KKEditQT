/*
 *
 * ©K. D. Hedger. Wed 16 Mar 14:07:53 GMT 2022 keithdhedger@gmail.com

 * This file (jslang.h) is part of jslang.

 * jslang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * jslang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with jslang.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JSLANG_H
#define JSLANG_H

#include "../../../src/SyntaxHighlitePluginInterface.h"

class jslang : public QObject, SyntaxHighlitePluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.SyntaxHighlitePluginInterface" FILE "jslang.json")
	Q_INTERFACES(SyntaxHighlitePluginInterface)

	public:
		void					initPlug(QString pathtoplug) override;
		void					unloadPlug(void) override;
//theme
		virtual void			setTheme(QHash<int,themeStruct> newtheme) override;
//new rules format
		void					setLanguageRules(QVector<highLightingRule> *rules) override;
//odd single formats
		void					setMultLineFormatStart(highLightingRule *hr) override;
		void					setMultLineFormatStop(highLightingRule *hr) override;
//custom code for edge cases etc
		void					runCustomRule(QString text,highLightingRule *hr) override;

	private:
		QString					plugPath;
		QHash<int,themeStruct>	theme;
};

#endif


