/*
 *
 * ©K. D. Hedger. Sun 28 Jul 14:45:42 BST 2024 keithdhedger@gmail.com

 * This file (QT_ProxyStyle.cpp) is part of KKEditQT.

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

#include "QT_ProxyStyle.h"

int ProxyStyle::styleHint(StyleHint hint,const QStyleOption *option,const QWidget *widget,QStyleHintReturn *returnData) const
{
	if (hint == QStyle::SH_Menu_Scrollable)
		return(int(true));

	return QProxyStyle::styleHint(hint,option,widget,returnData);
}