/*
 *
 * ©K. D. Hedger. Tue 12 Apr 20:50:07 BST 2022 keithdhedger@gmail.com

 * This file (PluginCommon.h) is part of KKEditQT.

 * KKEditQT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * KKEditQT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with KKEditQT.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PLUGINCOMMON_H
#define PLUGINCOMMON_H

struct highLightingRule
{
	QRegularExpression	pattern;
	QRegularExpression	endPattern;
	QTextCharFormat		format;
	bool					customRule=false;
	QString				type;
	int					start;
	int					len;
};

struct themeStruct
{
	QColor				colour;
	int					weight;
	bool					italic;
};

#define NUMBERSREGEX "([+-]?\\b[[:digit:]]*\\.?[[:digit:]]+([eE][+-]?[[:digit:]]+)?\\b)|([+-]?\\b0x[[:xdigit:]]*\\.?[[:xdigit:]]+\\b)"

enum {FUNCTIONTHEME=0,CLASSTHEME,TYPETHEME,COMMENTTHEME,QUOTESTHEME,INCLUDETHEME,NUMBERTHEME,KEYWORDTHEME,CUSTOMTHEME,LANGUAGEEXTRAS,VARIABLETHEME,TOOLKITTHEME,THEMEDONE};

#endif










