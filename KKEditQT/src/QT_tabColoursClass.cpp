/*
 *
 * ©K. D. Hedger. Mon 30 May 12:50:05 BST 2022 keithdhedger@gmail.com

 * This file (QT_tabColoursClass.cpp) is part of KKEditQT.

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

#include "QT_tabColoursClass.h"

TabColours::TabColours(QTabWidget *parent)
{
	this->tabwidget=parent;
}

TabColours::~TabColours()
{
}

void TabColours::paintEvent(QPaintEvent *event)
{
	DocumentClass	*doc;
	QStylePainter	painter(this);
	QStyleOptionTab	opt;

	QTabBar::paintEvent(event);
	for(int i=0;i<this->count();i++)
		{
			doc=qobject_cast<DocumentClass*>(this->tabwidget->widget(i));
			if(doc->visible==true)
				{
					initStyleOption(&opt,i);

					if(doc->tabColour.isValid())
						opt.palette.setColor(QPalette::Button,doc->tabColour);
					painter.drawControl(QStyle::CE_TabBarTabShape,opt);
					painter.drawControl(QStyle::CE_TabBarTabLabel,opt);
				}
		}
}