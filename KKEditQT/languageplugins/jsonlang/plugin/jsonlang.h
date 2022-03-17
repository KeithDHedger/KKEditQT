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
		void					initPlug(QString pathtoplug) override;
		void					unloadPlug(void) override;
//theme
		virtual void			setTheme(QHash<int,themeStruct> newtheme) override;
//new rules format
		void					setLanguageRules(QVector<highLightingRule> *rules) override;
//odd single formats
		void					setMultLineFormatStart(highLightingRule *hr) override;
		void					setMultLineFormatStop(highLightingRule *hr) override;

	private:
		QString					plugPath;
		QHash<int,themeStruct>	theme;
};

#endif
