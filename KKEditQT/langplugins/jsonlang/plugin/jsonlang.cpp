/*
 *
 * ©K. D. Hedger. Thu 17 Mar 16:26:06 GMT 2022 keithdhedger@gmail.com

 * This file (jsonlang.cpp) is part of jsonlang.

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

#include "jsonlang.h"

void jsonlang::initPlug(QString pathtoplug)
{
	this->plugPath=pathtoplug;
}

void jsonlang::unloadPlug(void)
{
}

//new format
void jsonlang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//quotes
	hr.format.setForeground(this->theme.value("quotes").colour);
	hr.format.setFontItalic(this->theme.value("quotes").italic);
	if(this->theme.value("quotes").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern = QRegularExpression("(\"([^\"\\\\]*(\\\\.[^\"\\\\]*)*)\")|('\\\\.')|('.')");
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
	hr.pattern=QRegularExpression("\\b(true|false)\\b");
	rules->append(hr);
}

void jsonlang::setTheme(QMap<QString,partsStruct> newtheme)
{
	this->theme=newtheme;
}


