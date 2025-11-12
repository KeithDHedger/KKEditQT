/*
 *
 * ©K. D. Hedger. Wed 12 Nov 12:00:12 GMT 2025 keithdhedger@gmail.com

 * This file (docBrowser.h) is part of KKEditQT.

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

#ifndef _DOCBROWSER_
#define _DOCBROWSER_

//#include "globalincludes.h"
#include "qtincs.h"
#define MARGINS 4

class KKEditClass;

class docBrowserClass
{
	public:
		docBrowserClass(KKEditClass *kk);
		~docBrowserClass();

		QString		windowTitle;
		QTextBrowser	*te;
		QWidget		*winWidget=NULL;
		void			createNewWindow(QString path);

	private:
		QString		basePath;
		QString		baseDir;
		QString		homePath;
		QString		homeDir;
		QString		urlTofile(QUrl path);
		KKEditClass	*mainKKEditClass=NULL;

		bool			noBack=false;
		bool			setToPathOrHTML(QUrl path);
		void			setPaths(QUrl path);
;};

#endif
