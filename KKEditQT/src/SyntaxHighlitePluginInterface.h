/*
 *
 * ©K. D. Hedger. Tue  8 Mar 16:36:35 GMT 2022 keithdhedger@gmail.com

 * This file (SyntaxHighlitePluginInterface.h) is part of KKEditQT.

 * KKEditQT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * KKEditQT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with KKEditQT.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SYNTAXHIGHLITEPLUGININTERFACE_H
#define SYNTAXHIGHLITEPLUGININTERFACE_H

#include <QtCore>
#include <QTextFormat>

struct highLightingRule
{
	QRegularExpression	pattern;
	QTextCharFormat		format;
};

struct themeStruct
{
	QColor				colour;
	int					weight;
	bool				italic;
};

enum {FUNCTIONTHEME=0,CLASSTHEME,TYPETHEME,COMMENTTHEME,QUOTESTHEME,INCLUDETHEME,NUMBERTHEME,KEYWORDTHEME,CUSTOMTHEME,THEMEDONE};

class SyntaxHighlitePluginInterface
{
	public:
		virtual			~SyntaxHighlitePluginInterface()=default;

		virtual void	initPlug(QString pathtoplug)=0;
		virtual void	unloadPlug(void)=0;
//theme
		virtual void	setTheme(QHash<int,themeStruct>	newtheme)=0;
//set individual rule
		virtual void	setFunctionRule(QVector<highLightingRule> *rules)=0;
		virtual void	setClassRule(QVector<highLightingRule> *rules)=0;
		virtual void	setTypeRule(QVector<highLightingRule> *rules)=0;
		virtual void	setSingleLineCommentRule(QVector<highLightingRule> *rules)=0;
		virtual void	setDoubleQuotesRule(QVector<highLightingRule> *rules)=0;
		virtual void	setIncludesRule(QVector<highLightingRule> *rules)=0;
		virtual void	setNumberRule(QVector<highLightingRule> *rules)=0;
		virtual void	setKeywordRule(QVector<highLightingRule> *rules)=0;
		virtual void	setCustomRule(QVector<highLightingRule> *rules)=0;

//odd single formats
		virtual void	setMultLineFormatStart(highLightingRule *hr)=0;
		virtual void	setMultLineFormatStop(highLightingRule *hr)=0;
};

QT_BEGIN_NAMESPACE

#define SyntaxHighlitePluginInterface_iid "org.KKEditQT.SyntaxHighlitePluginInterface"

Q_DECLARE_INTERFACE(SyntaxHighlitePluginInterface, SyntaxHighlitePluginInterface_iid)
QT_END_NAMESPACE

#endif
