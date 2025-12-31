/*
 *
 * ©K. D. Hedger. Fri 21 Nov 14:11:18 GMT 2025 keithdhedger@gmail.com

 * This file (QT_AboutBox.h) is part of KKEditQT.

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

#ifndef _QT_ABOUTBOX_
#define _QT_ABOUTBOX_

#include "internet.h"

class AboutBoxClass : public QObject
{
	public:
		AboutBoxClass(QWidget* window,QString pixpath);
		AboutBoxClass(void);
		~AboutBoxClass();

		void		runAbout(void);
		void		setLicence(QString);
		void		setAuthors(QString authors);
		void		showLicence(void);
		void		showCredits(void);
		void		killAboutBox(void);
		void		showHelp(void);

	private:
		QDialog	*aboutDialog;
		QDialog	*licenceDialog;
		QDialog	*creditsDialog;
		QString	licence;
		QString	authors;
};

#endif
