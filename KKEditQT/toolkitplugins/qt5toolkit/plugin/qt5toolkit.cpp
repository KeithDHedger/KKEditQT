/*
 *
 * ©K. D. Hedger. Wed 16 Mar 14:08:07 GMT 2022 keithdhedger@gmail.com

 * This file (qt5toolkit.cpp) is part of qt5toolkit.

 * qt5toolkit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * qt5toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with qt5toolkit.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qt5toolkit.h"

void qt5toolkit::initPlug(QString pathtoplug)
{
	this->plugPath=pathtoplug;
}

void qt5toolkit::unloadPlug(void)
{
}

void qt5toolkit::setToolkitRules(QVector<highLightingRule> *rules)
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

void qt5toolkit::setTheme(QMap<QString,partsStruct> newtheme)
{
	this->theme=newtheme;
}
