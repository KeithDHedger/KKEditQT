/*
 *
 * ©K. D. Hedger. Fri  4 Oct 11:57:19 BST 2024 keithdhedger@gmail.com

 * This file (tagClass.cpp) is part of KKEditQT.

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

#include "tagClass.h"

tagClass::~tagClass()
{
}

tagClass::tagClass(KKEditClass *mainKKEditClass)
{
	this->mainKKEditClass=mainKKEditClass;
}

void tagClass::getTagList(QStringList filepaths,int sorttype)
{
	QString					sort;
	QString					command;
	QString					results;
	int						sorthow;
	QStringList				retlines;
	QString					tagstring;
	QStringList				lines;
	QString					typestring;
	QString					linestring;
	QString					defstring;
	QString					wholetag;
	QStringList				list;
	QString					rep;
	QRegularExpressionMatch	match;
	QRegularExpression		re(" [0-9]+ ");
	QString					filestring;
	QString					paths;

	if(sorttype==-1)
		sorthow=this->mainKKEditClass->prefsFunctionMenuLayout;
	else
		sorthow=sorttype;
	
	switch(sorthow)
		{
			case 0:
				sort="sort -k 2rb,2rb -k 1b,1b";
				break;
			case 1:
				sort="sort -k 2rb,2rb -k 3n,3n";
				break;
			case 2:
				sort="sort -k 3n";
				break;
			case 4:
				sort="sort -k 2rb,2rb -k 1b,1b";
				break;
			default:
				sort="sort";
				break;
		}

	for(int k=0;k<filepaths.count();k++)
		paths+="'"+filepaths.at(k)+"' ";

	command=QString("ctags -Gx %1|%2|sed 's@ \\+@ @g'").arg(paths).arg(sort);
	results=this->mainKKEditClass->runPipeAndCapture(command);
	lines=results.split("\n",Qt::SkipEmptyParts);

	for(int j=0;j<lines.count();j++)
		{
			for(int k=0;k<filepaths.count();k++)
				{
					if(lines.at(j).contains(filepaths.at(k)))
						{
							filestring=filepaths.at(k);
							break;
						}
				}

			int fln;
			rep=QString(lines.at(j));
			fln=rep.indexOf(re);
			wholetag=rep.left(fln).trimmed();
			list=wholetag.split(" ");
			typestring=list.last();
			list.removeLast();
			tagstring=QString(list.join(" "));

			match=re.match(rep); 
			if (match.hasMatch())
				linestring=match.captured(0).trimmed();

			defstring=rep.right(rep.length()-(rep.indexOf(filestring)+filestring.length())).trimmed();
			tagsStruct ts={tagstring,typestring,linestring,defstring,filestring,linestring.toInt()};
			this->tagList.push_back(ts);
		}
}
