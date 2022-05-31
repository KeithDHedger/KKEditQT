/*
 *
 * ©K. D. Hedger. Mon 30 May 12:49:43 BST 2022 keithdhedger@gmail.com

 * This file (QT_tabColoursClass.h) is part of KKEditQT.

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

#ifndef _QT_TABCOLOURSCLASS_
#define _QT_TABCOLOURSCLASS_

#include "KKEditClass.h"

class TabColours : public QTabBar
{
	Q_OBJECT

	public:
					TabColours(QTabWidget *parent);
					~TabColours();
	private:
		QTabWidget	*tabwidget;

	protected:
		void			paintEvent(QPaintEvent *event);
};

#endif
