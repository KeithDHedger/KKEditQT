/*
 *
 * ©K. D. Hedger. Tue 31 May 12:16:10 BST 2022 keithdhedger@gmail.com

 * This file (QT_themeClass.cpp) is part of KKEditQT.

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

#include "QT_themeClass.h"

ThemeClass::ThemeClass(KKEditClass *kk)
{
	this->mainKKEditClass=kk;
	this->loadThemes();
}

ThemeClass::~ThemeClass()
{
}

void ThemeClass::loadTheme(QString themename)
{
	int				cnt;
    QVariantMap		mainmap;
	QVariantMap		map;
 	QVariantList		locallist;
	QJsonDocument	doc;
	QJsonParseError	errorPtr;
	QByteArray		data;

	this->currentTheme="default";

	for(int j=0;j<this->themePath.count();j++)
		{
			if(this->themePath.at(j).baseName()==themename)
				{
					this->currentTheme=this->themePath.at(j).absoluteFilePath();
					break;
				}
		}

	QFile			inFile(this->currentTheme);

    inFile.open(QIODevice::ReadOnly|QIODevice::Text);
    data=inFile.readAll();
    inFile.close();

    doc=QJsonDocument::fromJson(data,&errorPtr);
	if(doc.isNull())
		{
			qWarning() << "Parse failed for " << this->currentTheme;
			return;
		}
	mainmap=doc.object().toVariantMap();
	cnt=0;
	while(this->entryNames[cnt]!=NULL)
		{
			locallist=mainmap[this->entryNames[cnt]].toList();
			map=locallist[0].toMap();
			if(cnt==0)
				{
					int	doccnt=0;
					while(this->docPartNames[doccnt]!=NULL)
						{
							this->themeParts[this->docPartNames[doccnt]].colourString=QString(map[this->docPartNames[doccnt]].toString());
							this->themeParts[this->docPartNames[doccnt]].colour=QColor(this->themeParts[this->docPartNames[doccnt]].colourString);
							doccnt++;
						}
				}
			else
				{
					this->themeParts[this->entryNames[cnt]].colourString=QString(map["colour"].toString());
					this->themeParts[this->entryNames[cnt]].colour=QColor(map["colour"].toString());
					this->themeParts[this->entryNames[cnt]].italic=map["italic"].toBool();
					this->themeParts[this->entryNames[cnt]].bold=map["bold"].toBool();
				}
			cnt++;
		}

#ifdef _DEBUGCODE_
	qDebug()<<"Theme"<<this->currentTheme;
	QMapIterator<QString,partsStruct> i(this->themeParts);
	while(i.hasNext())
		{
			i.next();
			qDebug() << i.key() << ": " << i.value().colourString << i.value().italic<< i.value().bold<< i.value().colour;
		}
#endif
}

void ThemeClass::loadThemes(void)
{
	QString			themepath;
	QDir				dir(QString("%1/themes").arg(this->mainKKEditClass->homeDataFolder));
	QFileInfoList	finfolist=dir.entryInfoList(QStringList("*.json"),QDir::Files);
	for(int j=0;j<finfolist.count();j++)
		this->themePath.append(finfolist.at(j));

	dir.setPath(QString("%1/themes").arg(DATADIR));
	finfolist=dir.entryInfoList(QStringList("*.json"),QDir::Files);
	for(int j=0;j<finfolist.count();j++)
		this->themePath.append(finfolist.at(j));
}

unsigned long ThemeClass::hashFromKey(QString key)
{
	unsigned long hash=0;

	for(int j=0;j<key.length();j++)
		hash=31*hash+key.at(j).toLatin1();

	return(hash);
}
