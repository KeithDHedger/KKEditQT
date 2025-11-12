/*
 *
 * ©K. D. Hedger. Tue 31 May 12:16:02 BST 2022 keithdhedger@gmail.com

 * This file (QT_themeClass.h) is part of KKEditQT.

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


#ifndef _QT_THEMECLASS_
#define _QT_THEMECLASS_

#include "PluginCommon.h"

class ThemeClass : public QObject
{
	public:
		ThemeClass(KKEditClass *kk);
		~ThemeClass();

//themeparts
		QMap<QString,partsStruct>	themeParts;
		QVector<QFileInfo>			themePath;

		void							loadTheme(QString themename);

	private:
		KKEditClass					*mainKKEditClass;
		QString						currentTheme;

		unsigned long				hashFromKey(QString key);
		void							loadThemes(void);
		const char					*entryNames[14]={"document","functions","class","types","comments","quotes","includes","numbers","keywords","custom","lanuageextra","variables","toolkit",NULL};
		const char					*docPartNames[9]={"docbgcolour","docfgcolour","findbgcol","findfgcol","linebgcolour","linefgcolour","bookmarkbgcolour","insertcolour",NULL};
};

#endif
