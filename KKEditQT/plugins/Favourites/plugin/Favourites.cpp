/*
 *
 * ©K. D. Hedger. Sat 26 Mar 15:12:06 GMT 2022 keithdhedger@gmail.com

 * This file (Favourites.cpp) is part of Favourites.

 * Favourites is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * Favourites is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Favourites.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Favourites.h"

QString FavouritesPlug::truncateWithElipses(const QString str)
{
	QString newlabel;
	QString tempstr=str.trimmed();

	if(tempstr.length()>MAXFAVMENULEN)
		newlabel=tempstr.left((MAXFAVMENULEN-3)/2)+"..."+tempstr.right((MAXFAVMENULEN-3)/2);
	else
		newlabel=tempstr;

	return(newlabel);
}

void FavouritesPlug::initPlug(KKEditClass *kk,QString pathtoplug)
{
	QAction	*adddoc;
	QString	data;

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

	data=QString("tail -n%1 '%2/favs' > /tmp/favs;cp /tmp/favs '%3/favs'").arg(MAXFAVS).arg(this->mainKKEditClass->homeDataFolder).arg(this->mainKKEditClass->homeDataFolder);
	system(data.toStdString().c_str());

	this->favouritesMenu=new QMenu("Favourites");
	this->favouritesMenu->setIcon(QIcon(QString("%1/Favorite.png").arg(QFileInfo(pathtoplug).absolutePath())));
	this->mainKKEditClass->pluginMenu->addMenu(favouritesMenu);

	adddoc=new QAction(QString("Add Current Document"));
	this->favouritesMenu->addAction(adddoc);
	QObject::connect(adddoc,&QAction::triggered,[this]()
		{
			QAction	*addfile;
			QString	data;

			if(this->mainKKEditClass->currentFilepath.isEmpty()==false)
				{
					data=QString("echo '%1' >> '%2/favs'").arg(this->mainKKEditClass->currentFilepath).arg(this->mainKKEditClass->homeDataFolder);
					system(data.toStdString().c_str());

					addfile=new QAction(QString(this->mainKKEditClass->currentFilepath));
					this->favouritesMenu->addAction(addfile);
					QObject::connect(addfile,&QAction::triggered,[this,addfile]()
						{
							this->doAction(addfile->text());
						});
				}
		});

	adddoc=new QAction(QString("Add Current DocView URL"));
	this->favouritesMenu->addAction(adddoc);
	QObject::connect(adddoc,&QAction::triggered,[this]()
		{
			QAction	*addurl;
			QString	data;
			if(this->mainKKEditClass->currentURL.isEmpty()==false)
				{
					data=QString("echo '%1' >> '%2/favs'").arg(this->mainKKEditClass->currentURL).arg(this->mainKKEditClass->homeDataFolder);
					system(data.toStdString().c_str());

					addurl=new QAction(QString(this->mainKKEditClass->currentURL));
					this->favouritesMenu->addAction(addurl);
					QObject::connect(addurl,&QAction::triggered,[this,addurl]()
						{
							this->doAction(addurl->text());
						});
				}
		});			

	this->favouritesMenu->addSeparator();

	QFile file(QString("%1/favs").arg(this->mainKKEditClass->homeDataFolder));
	if(file.open(QIODevice::ReadOnly|QIODevice::Text))
		{
			QTextStream	stream(&file);
			QString		line;
			QAction		*addurl;
			bool			flag=false;
			do
				{
					flag=stream.readLineInto(&line);
					if((flag==true) && (line.isEmpty()==false))
						{
							addurl=new QAction(line);
							this->favouritesMenu->addAction(addurl);
							QObject::connect(addurl,&QAction::triggered,[this,line]()
								{
									this->doAction(line);
								});
						}
				}
			while(flag==true);

			file.close();
		}
}

void FavouritesPlug::unloadPlug(void)
{
	QObject::disconnect(this->favCon);
	delete this->favouritesMenu;
}

void FavouritesPlug::plugAbout(void)
{
	QMessageBox msgBox;

	QString txt="KKEditQT Favourites Plugin\n\n©K.D.Hedger 2022\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";
	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("Plugin About");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.exec();
}

void FavouritesPlug::plugSettings(void)
{
}

unsigned int FavouritesPlug::plugWants(void)
{
	return(DOABOUT);
}

void FavouritesPlug::doAction(QString data)
{
	QString command;

	if((data.startsWith("file://")) || (data.startsWith("http://")) || (data.startsWith("https://")))
		{
			command=QString("kkeditqtmsg -k %1 -c 'openindocview' -d '%2'").arg(this->mainKKEditClass->sessionID).arg(data);
			system(command.toStdString().c_str());
		}
	else
		{
			command=QString("kkeditqtmsg -k %1 -c 'openfile' -d '%2'").arg(this->mainKKEditClass->sessionID).arg(data);
			system(command.toStdString().c_str());
		}
}


