/*
 *
 * ©K. D. Hedger. Mon 24 Nov 11:14:23 GMT 2025 keithdhedger@gmail.com

 * This file (MainWindow.cpp) is part of KKEditQT.

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

#include "KKEditClass.h"
#include "docBrowser.h"
#include "MainWindow.h"

MainWindowClass::MainWindowClass(KKEditClass *kk)
{
	this->mainKKEditClass=kk;
}

void MainWindowClass::closeEvent(QCloseEvent *event)
{
	if(this->mainKKEditClass->docView!=NULL)
		{
			delete this->mainKKEditClass->docView;
			this->mainKKEditClass->docView=NULL;
		}
	event->accept();
}

