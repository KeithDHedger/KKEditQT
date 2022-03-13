/*
 *
 * ©K. D. Hedger. Sat  5 Mar 16:01:08 GMT 2022 keithdhedger@gmail.com

 * This file (kkeditqtPluginExample.h) is part of kkeditqtPluginExample.

 * kkeditqtPluginExample is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * kkeditqtPluginExample is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with kkeditqtPluginExample.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPLANG_H
#define CPPLANG_H

#include "../../../src/SyntaxHighlitePluginInterface.h"

class cpplang : public QObject, SyntaxHighlitePluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.SyntaxHighlitePluginInterface" FILE "cpplang.json")
	Q_INTERFACES(SyntaxHighlitePluginInterface)

	public:
		void					initPlug(QString pathtoplug) override;
		void					unloadPlug(void) override;
//theme
		virtual void			setTheme(QHash<int,themeStruct> newtheme) override;
//set individual rule
		void					setFunctionRule(QVector<highLightingRule> *rules) override;
		void					setClassRule(QVector<highLightingRule> *rules) override;
		void					setTypeRule(QVector<highLightingRule> *rules) override;
		void					setSingleLineCommentRule(QVector<highLightingRule> *rules) override;
		void					setDoubleQuotesRule(QVector<highLightingRule> *rules) override;
		void					setIncludesRule(QVector<highLightingRule> *rules) override;
		void					setNumberRule(QVector<highLightingRule> *rules) override;
		void					setKeywordRule(QVector<highLightingRule> *rules) override;
		void					setCustomRule(QVector<highLightingRule> *rules) override;
//odd single formats
		void					setMultLineFormatStart(highLightingRule *hr) override;
		void					setMultLineFormatStop(highLightingRule *hr) override;
	private:
		QString					plugPath;
		QHash<int,themeStruct>	theme;
};

#endif
