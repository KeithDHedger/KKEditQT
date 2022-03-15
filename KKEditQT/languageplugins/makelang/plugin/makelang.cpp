/*
 *
 * ©K. D. Hedger. Sat  5 Mar 16:00:46 GMT 2022 keithdhedger@gmail.com

 * This file (kkeditqtPluginExample.cpp) is part of kkeditqtPluginExample.

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

#include "makelang.h"

void cpplang::initPlug(QString pathtoplug)
{
	themeStruct	blank={Qt::black,QFont::Normal,false};

	this->plugPath=pathtoplug;
	if(this->theme.count()==0)
		for(int j=FUNCTIONTHEME;j<THEMEDONE;j++)
			this->theme[j]=blank;
}

void cpplang::unloadPlug(void)
{
}

//new format
void cpplang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//functions
	hr.format.setFontItalic(this->theme[FUNCTIONTHEME].italic);
	hr.format.setFontWeight(this->theme[FUNCTIONTHEME].weight);
	hr.format.setForeground(this->theme[FUNCTIONTHEME].colour);
	hr.pattern=QRegularExpression("^[[:space:]]*[A-Za-z0-9_]+(?=[[:space:]]*=?[[:space:]]*[{\\(])");
	rules->append(hr);

//targets
	hr.format.setForeground(this->theme[LANGUAGEEXTRAS].colour);
	hr.format.setFontWeight(this->theme[LANGUAGEEXTRAS].weight);
	hr.format.setFontItalic(this->theme[LANGUAGEEXTRAS].italic);
	hr.pattern=QRegularExpression("^[[:print:]]*(?=:)");
	rules->append(hr);

//keywords
	hr.format.setForeground(this->theme[KEYWORDTHEME].colour);
	hr.format.setFontWeight(this->theme[KEYWORDTHEME].weight);
	hr.format.setFontItalic(this->theme[KEYWORDTHEME].italic);
	hr.pattern=QRegularExpression("\\b(addprefix|addsuffix|basename|call|dir|error|filter|filter-out|findstring|firstword|foreach|join|notdir|origin|patsubst|shell|sort|strip|subst|suffix|warning|wildcard|word|words|define|else|endef|endif|if|ifdef|ifeq|ifndef|ifneq|include|override|unexport|case|esac|fi|elif|echo|exit|for|printf|continue|do|done|test|break)\\b");
	rules->append(hr);

//quotes
	hr.format.setForeground(this->theme[QUOTESTHEME].colour);
	hr.format.setFontWeight(this->theme[QUOTESTHEME].weight);
	hr.format.setFontItalic(this->theme[QUOTESTHEME].italic);
	hr.pattern = QRegularExpression("(\".*\")|('.*')");
	rules->append(hr);

//variables
	hr.format.setForeground(this->theme[CUSTOMTHEME].colour);
	hr.format.setFontWeight(this->theme[CUSTOMTHEME].weight);
	hr.format.setFontItalic(this->theme[CUSTOMTHEME].italic);
	hr.pattern=QRegularExpression("([[:word:]_]*)(?=[[:space:]]*=)");
	rules->append(hr);

//numbers
	hr.format.setForeground(this->theme[NUMBERTHEME].colour);
	hr.format.setFontWeight(this->theme[NUMBERTHEME].weight);
	hr.format.setFontItalic(this->theme[NUMBERTHEME].italic);
	hr.pattern=QRegularExpression("([+-]?\\b[[:digit:]]*\\.?[[:digit:]]+([eE][+-]?[[:digit:]]+)?\\b)|([+-]?\\b0x[[:xdigit:]]*\\.?[[:xdigit:]]+\\b)");
	rules->append(hr);

//types
	hr.format.setForeground(this->theme[TYPETHEME].colour);
	hr.format.setFontWeight(this->theme[TYPETHEME].weight);
	hr.format.setFontItalic(this->theme[TYPETHEME].italic);
	hr.pattern=QRegularExpression("\\b(true|false|yes|no)|\\b");
	rules->append(hr);

//angle brackets
	hr.format.setForeground(this->theme[CLASSTHEME].colour);
	hr.format.setFontWeight(this->theme[CLASSTHEME].weight);
	hr.format.setFontItalic(this->theme[CLASSTHEME].italic);
	hr.pattern=QRegularExpression("(\\$\\$?{[[:word:]][[:word:]]*}|\\$\\([[:word:]][[:word:]]*\\))");
	rules->append(hr);

//extras
	hr.format.setForeground(this->theme[CLASSTHEME].colour);
	hr.format.setFontWeight(this->theme[CLASSTHEME].weight);
	hr.format.setFontItalic(this->theme[CLASSTHEME].italic);
	hr.pattern=QRegularExpression("\\$\\$?[[:word:]][[:word:]]*");
	rules->append(hr);

//single line comment
	hr.format.setForeground(this->theme[COMMENTTHEME].colour);
	hr.format.setFontWeight(this->theme[COMMENTTHEME].weight);
	hr.format.setFontItalic(this->theme[COMMENTTHEME].italic);
	hr.pattern=QRegularExpression("#[^\n]*");
	rules->append(hr);
}

//odd single formats set to "" for no multiline comment
void cpplang::setMultLineFormatStart(highLightingRule *hr)
{
	hr->pattern = QRegularExpression("");
}

void cpplang::setMultLineFormatStop(highLightingRule *hr)
{
	hr->pattern = QRegularExpression("");
}

void cpplang::setTheme(QHash<int,themeStruct> newtheme)
{
	this->theme=newtheme;
}

