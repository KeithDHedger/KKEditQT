/*
 *
 * ©K. D. Hedger. Fri 21 Nov 13:02:05 GMT 2025 keithdhedger@gmail.com

 * This file (qttoolkit.cpp) is part of qttoolkit.

 * qttoolkit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * qttoolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with qttoolkit.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "qttoolkit.h"

void qttoolkit::initPlug(QString pathtoplug)
{
	this->plugPath=pathtoplug;
}

void qttoolkit::unloadPlug(void)
{
}

void qttoolkit::setToolkitRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

	hr.format.setForeground(this->theme.value("toolkit").colour);
	hr.format.setFontItalic(this->theme.value("toolkit").italic);
	if(this->theme.value("toolkit").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\bQ[A-Za-z]+|qobject_cast\\b");
	rules->append(hr);
}

void qttoolkit::setTheme(QMap<QString,partsStruct> newtheme)
{
	this->theme=newtheme;
}
