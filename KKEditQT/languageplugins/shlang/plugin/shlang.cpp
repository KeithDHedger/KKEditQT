/*
 *
 * ©K. D. Hedger. Sun 13 Mar 18:41:54 GMT 2022 keithdhedger@gmail.com

 * This file (shlang.cpp) is part of shlang.

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

#include "shlang.h"

void shlang::initPlug(QString pathtoplug)
{
	themeStruct	blank={Qt::black,QFont::Normal,false};

	this->plugPath=pathtoplug;
	if(this->theme.count()==0)
		for(int j=FUNCTIONTHEME;j<THEMEDONE;j++)
			this->theme[j]=blank;
}

void shlang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//keywords rule
	hr.format.setForeground(this->theme[KEYWORDTHEME].colour);
	hr.format.setFontWeight(this->theme[KEYWORDTHEME].weight);
	hr.format.setFontItalic(this->theme[KEYWORDTHEME].italic);
	hr.pattern=QRegularExpression("\\b(alias|bg|bind|break|builtin|caller|case|command|compgen|complete|continue|declare|dirs|disown|do|done|elif|else|enable|esac|eval|exec|exit|export|false|fc|fg|fi|for|getopts|hash|help|history|if|in|jobs|let|local|logout|popd|printf|pushd|read|readonly|return|select|set|shift|shopt|suspend|test|then|times|trap|true|type|typeset|umask|unalias|unset|until|wait|while|echo)\\b");
	rules->append(hr);

//numbers rule
	hr.format.setForeground(this->theme[NUMBERTHEME].colour);
	hr.format.setFontWeight(this->theme[NUMBERTHEME].weight);
	hr.format.setFontItalic(this->theme[NUMBERTHEME].italic);
	hr.pattern=QRegularExpression(NUMBERSREGEX);
	rules->append(hr);

//quotes rule
	hr.format.setForeground(this->theme[QUOTESTHEME].colour);
	hr.format.setFontWeight(this->theme[QUOTESTHEME].weight);
	hr.format.setFontItalic(this->theme[QUOTESTHEME].italic);
	hr.pattern = QRegularExpression("(\".*\")|('.*')");
	rules->append(hr);

//includes rule
	hr.format.setForeground(this->theme[INCLUDETHEME].colour);
	hr.format.setFontWeight(this->theme[INCLUDETHEME].weight);
	hr.format.setFontItalic(this->theme[INCLUDETHEME].italic);
	hr.pattern=QRegularExpression("^[[:blank:]]*(\\.[^\\n]*|source[^\\n]*)");
	rules->append(hr);

//variables
	hr.format.setForeground(this->theme[VARIABLETHEME].colour);
	hr.format.setFontWeight(this->theme[VARIABLETHEME].weight);
	hr.format.setFontItalic(this->theme[VARIABLETHEME].italic);
	hr.pattern=QRegularExpression("([[:word:]]*)(?==)");
	rules->append(hr);

//angle brackets
	hr.format.setForeground(this->theme[CLASSTHEME].colour);
	hr.format.setFontWeight(this->theme[CLASSTHEME].weight);
	hr.format.setFontItalic(this->theme[CLASSTHEME].italic);
	hr.pattern=QRegularExpression("(\\${[[:print:]]*}|\\$\\([[:print:]]*\\)|\\$[[:word:]_]+)");
	rules->append(hr);

//function dec
	hr.format.setForeground(this->theme[FUNCTIONTHEME].colour);
	hr.format.setFontWeight(this->theme[FUNCTIONTHEME].weight);
	hr.format.setFontItalic(this->theme[FUNCTIONTHEME].italic);
	hr.pattern=QRegularExpression("^(function[[:blank:]]*[[:word:]]*|[[:word:]]*)([[:blank:]]*\\([[:blank:]]*\\))");
	rules->append(hr);

//single line comment
	hr.format.setForeground(this->theme[COMMENTTHEME].colour);
	hr.format.setFontWeight(this->theme[COMMENTTHEME].weight);
	hr.format.setFontItalic(this->theme[COMMENTTHEME].italic);
	hr.pattern=QRegularExpression("#[^\n]*");
	rules->append(hr);
}

void shlang::unloadPlug(void)
{
}

//odd single formats set to "" for no multiline comment
void shlang::setMultLineFormatStart(highLightingRule *hr)
{
	hr->format.setForeground(this->theme[COMMENTTHEME].colour);
	hr->format.setFontWeight(this->theme[COMMENTTHEME].weight);
	hr->format.setFontItalic(this->theme[COMMENTTHEME].italic);
	hr->pattern = QRegularExpression("if false;then");
}

void shlang::setMultLineFormatStop(highLightingRule *hr)
{
	hr->pattern = QRegularExpression("fi");
}

void shlang::setTheme(QHash<int,themeStruct> newtheme)
{
	this->theme=newtheme;
}
