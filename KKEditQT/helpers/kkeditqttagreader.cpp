/*
 *
 * ©K. D. Hedger. Sat 27 Jul 15:57:43 BST 2024 keithdhedger@gmail.com

 * This file (kkeditqttagreader.cpp) is part of KKEditQT.

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

#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <unistd.h>

int main(int argc, char **argv)
{
	QString		grouptag;
	int			innertagcnt;
	QStringList	innertags;
	QString		tagname;
	QString		tagdata;
	int			argcount=1;
	QFile		file(argv[argcount++]);
	QStringList	sl;

	if(file.exists()==false)
		{
			qDebug() << "Cannot read file" << file.errorString();
			exit(0);
		}

	if(!file.open(QFile::ReadOnly | QFile::Text))
		{
			qDebug() << "Cannot open file for reading" << file.errorString();
			exit(0);
		}

	QXmlStreamReader reader(&file);

	grouptag=argv[argcount++];
	innertagcnt=0;
	for(int j=argcount;j<argc;j++)
		{
			innertags<<QString(argv[j]);
			innertagcnt++;
		}

	auto testfortag=[&]()-> bool
		{
			for(int j=0;j<innertagcnt;j++)
				if(innertags.at(j).compare(tagname)==0)
					return(true);
			return(false);
		};

	while(reader.isEndDocument()==false)
		{
			reader.readNext();
			if(reader.isStartElement()==true)
				{
					tagname=reader.name().toString();
					reader.readNext();
					tagdata=reader.text().toString();
					if(testfortag()==true)
						sl<<tagname+"="+tagdata+" ";
				}
			else
				{
					if(reader.name().toString().compare(grouptag)==0)
						{
							printf("%s\n",sl.join("").toStdString().c_str());
							sl.clear();
						}
				}
		}

	file.close();
	return 0;

}