/*
 *
 * ©K. D. Hedger. Fri  4 Oct 11:58:01 BST 2024 keithdhedger@gmail.com

 * This file (tagClass.h) is part of KKEditQT.

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

#ifndef _TAGCLASS_
#define _TAGCLASS_

#include "KKEditClass.h"

struct tagsStruct
	{
		QString tagName;
		QString tagType;
		QString lineNumberString;
		QString tagDefine;
		QString tagFilepath;
		int		lineNumber;
	};

class tagClass
{
	public:
		tagClass(KKEditClass *mainKKEditClass);
		~tagClass();

	QVector<tagsStruct>	tagList;

	void					getTagList(QStringList filepaths,int sorttype=-1);

	private:
		KKEditClass		*mainKKEditClass;
};
#endif
