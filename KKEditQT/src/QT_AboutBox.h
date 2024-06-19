/*
 *
 * ©K. D. Hedger. Thu 26 Nov 14:19:15 GMT 2015 keithdhedger@gmail.com

 * This file (QT_AboutBox.h) is part of AspellGUI.

 * AspellGUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * AspellGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with AspellGUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _QT_ABOUTBOX_
#define _QT_ABOUTBOX_

#include <QtWidgets>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

#include "internet.h"

static const char *defaultauthors="K.D.Hedger ©2013-2022<br><a href=\"mailto:" MYEMAIL "\">Email Me</a><br>" \
				"<a href=\"" GLOBALWEBSITE "\">Homepage</a>" \
				"<br><br>More by the same author<br>" \
				
				"<a href=\"" KKEDITQTPAGE "\">KKEditQT<br>" \
				"<a href=\"" KKEDITPAGE "\">KKEdit<br>" \
				"<a href=\"" XDECSPAGE "\">XDecorations<br>" \
				"<a href=\"" THEMEMANAGER "\">Xfce-Theme-Manager<br>" \
				"<a href=\"" COMPMANAGER "\">Xfce4-Composite-Editor<br>" \
				"<a href=\"" MANPAGEPAGE "\">Manpage Editor<br>" \
				"<a href=\"" GTKSUPAGE "\">GtkSu<br>" \
				"<a href=\"" ASPELLPAGE "\">ASpell<br>" \
				"<a href=\"" CLIPVIEW "\">Clipboard Viewer<br>" \
				"<a href=\"" KKFILEMANAGER "\">KKFileManager<br>" \
				"<a href=\"" KKTERMINALPAGE "\">KKTerminal<br>";

class AboutBoxClass : public QObject
{
    Q_OBJECT
int x=1;
	public:
		AboutBoxClass(QWidget* window,QString pixpath);
		~AboutBoxClass();

		void		runAbout(void);
		void		setLicence(QString);
		void		setAuthors(QString authors);
		void		showLicence(void);
		void		showCredits(void);
		void		killAboutBox(void);

	private:
		QDialog	*aboutDialog;
		QDialog	*licenceDialog;
		QDialog	*creditsDialog;
		QString	licence;
		QString	authors;
};
#pragma GCC diagnostic pop

#endif
