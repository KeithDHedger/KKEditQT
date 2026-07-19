/*
 *
 * ©K. D. Hedger. Sun 19 Jul 14:12:08 BST 2026 keithdhedger@gmail.com

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

#include "credits.h"
#include "KKEditClass.h"

#ifndef _QT_ABOUTBOX_
#define _QT_ABOUTBOX_

class AboutBoxClass : public QObject
{
	public:
		AboutBoxClass(QWidget *window,QString pixpath=NULL);
		~AboutBoxClass();

		QString		licence;
		QString		credits;

		void			runAbout(void);
		void			setHomepage(QString hpaddr,QString hpstr);
		void			setBodyText(QString bodystr);
		void			showAboutQtButton(bool show=false);
		void			showLicenceButton(bool show=false);
		void			showCreditsButton(bool show=false);
		void			showHelp(QString htmlpath);

	private:
		QDialog		*aboutDialog;
		QDialog		*licenceDialog;
		QDialog		*creditsDialog;
		QLabel		*hpLabel;
		QLabel		*bodyLabel;
		QLabel		*appNameLabel;
		QPushButton	*aboutQtButton;
		QPushButton	*licenseButton;
		QPushButton	*creditsButton;

		void			setAppName(void);
		void			showLicence(void);
		void			showCredits(void);
		void			killLicenceBox(void);
		void			killAboutBox(void);
		void			killCreditsBox(void);
};

#endif
