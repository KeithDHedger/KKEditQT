/*
 *
 * ©K. D. Hedger. Sun 28 Jul 14:45:34 BST 2024 keithdhedger@gmail.com

 * This file (QT_ProxyStyle.h) is part of KKEditQT.

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

#ifndef _QT_PROXYSTYLE_
#define _QT_PROXYSTYLE_

#include "KKEditClass.h"

class ProxyStyle : public QProxyStyle
{
  public:
    int styleHint(StyleHint hint,const QStyleOption *option=nullptr,const QWidget *widget=nullptr,QStyleHintReturn *returnData=nullptr) const override;
};

#endif
