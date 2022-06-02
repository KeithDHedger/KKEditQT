/*
 *
 * ©K. D. Hedger. Wed 16 Mar 11:12:16 GMT 2022 keithdhedger@gmail.com

 * This file (htmllang.cpp) is part of htmllang.

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

#include "htmllang.h"

void htmllang::initPlug(QString pathtoplug)
{
	this->plugPath=pathtoplug;
}

void htmllang::unloadPlug(void)
{
}

//new format
void htmllang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//functions
	hr.format.setForeground(this->theme.value("functions").colour);
	hr.format.setFontItalic(this->theme.value("functions").italic);
	if(this->theme.value("functions").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("^[[:space:]]*function[[:space:]]*[A-Za-z0-9_]+(?=\\()");
	rules->append(hr);

//angle brackets
	hr.format.setForeground(this->theme.value("keywords").colour);
	hr.format.setFontItalic(this->theme.value("keywords").italic);
	if(this->theme.value("keywords").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("(<[/\\!?]?.*[/\\!?]?>)|(<[/\\!?]?[^\\n]*)|([/\\!?]?>)");
	rules->append(hr);

//quotes
	hr.format.setForeground(this->theme.value("quotes").colour);
	hr.format.setFontItalic(this->theme.value("quotes").italic);
	if(this->theme.value("quotes").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern = QRegularExpression("(\".*\")|('.*')");
	rules->append(hr);

////variables
	hr.format.setForeground(this->theme.value("variables").colour);
	hr.format.setFontItalic(this->theme.value("variables").italic);
	if(this->theme.value("variables").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("([[:word:]_]*)(?=[[:space:]]*=)");
	rules->append(hr);

//numbers
	hr.format.setForeground(this->theme.value("numbers").colour);
	hr.format.setFontItalic(this->theme.value("numbers").italic);
	if(this->theme.value("numbers").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression(NUMBERSREGEX);
	rules->append(hr);

//types
	hr.format.setForeground(this->theme.value("types").colour);
	hr.format.setFontItalic(this->theme.value("types").italic);
	if(this->theme.value("types").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\&#?[[:alnum:]]+;|\\bconst\\b");
	rules->append(hr);
}

void htmllang::setMultLineRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

	hr.format.setForeground(this->theme.value("comments").colour);
	hr.format.setFontItalic(this->theme.value("comments").italic);
	if(this->theme.value("comments").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("<!--");
	hr.endPattern=QRegularExpression("-->");
	rules->append(hr);
}

void htmllang::setTheme(QMap<QString,partsStruct> newtheme)
{
	this->theme=newtheme;
}


