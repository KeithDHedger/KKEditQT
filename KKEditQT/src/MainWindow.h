/*
 *
 * ©K. D. Hedger. Mon 24 Nov 11:14:23 GMT 2025 keithdhedger@gmail.com

 * This file (MainWindow.h) is part of KKEditQT.

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

#ifndef _MAINWINDOW_
#define _MAINWINDOW_

class KKEditClass;

class MainWindowClass: public QMainWindow
{
	public:
		MainWindowClass(KKEditClass *kk);
	private:
		KKEditClass	*mainKKEditClass=NULL;
	    void			closeEvent(QCloseEvent *event) override;
};

#endif
