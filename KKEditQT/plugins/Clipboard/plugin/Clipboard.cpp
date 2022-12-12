/*
 *
 * ©K. D. Hedger. Sat 26 Mar 15:12:06 GMT 2022 keithdhedger@gmail.com

 * This file (Clipboard.cpp) is part of Clipboard.

 * Clipboard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * Clipboard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Clipboard.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Clipboard.h"

QString ClipboardPlug::truncateWithElipses(const QString str)
{
	QString newlabel;
	QString tempstr=str.trimmed();

	if(tempstr.length()>MAXCLIPMENULEN)
		newlabel=tempstr.left((MAXCLIPMENULEN-3)/2)+"..."+tempstr.right((MAXCLIPMENULEN-3)/2);
	else
		newlabel=tempstr;

	return(newlabel);
}

void ClipboardPlug::initPlug(KKEditClass *kk,QString pathtoplug)
{
	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

	this->cliboardMenu=new QMenu("Clipboard");
	this->cliboardMenu->setIcon(QIcon(QString("%1/ClipboardViewer.png").arg(QFileInfo(pathtoplug).absolutePath())));
	this->mainKKEditClass->pluginMenu->addMenu(cliboardMenu);

	for(int j=0;j<MAXCLIPS;j++)
		{
			this->clips[j]=new QAction(QString("Clip-%1").arg(j));
			this->clips[j]->setObjectName(QString("Clip-%1").arg(j));
			this->cliboardMenu->addAction(this->clips[j]);
			QObject::connect(this->clips[j],&QAction::triggered,[this,j]()
				{
					this->currentClip=j;
					this->clipboard->setText(this->clips[j]->objectName());
				});			
		}

	this->clipboard=this->mainKKEditClass->application->clipboard();
	this->clipCon=QObject::connect(this->clipboard,&QClipboard::dataChanged,[this]()
		{
			if(this->clipboard->text().compare(this->clips[this->currentClip]->objectName())!=0)
				{
					this->currentClip++;
					if(this->currentClip==MAXCLIPS)
						this->currentClip=0;
					this->clips[this->currentClip]->setText(this->truncateWithElipses(this->clipboard->text()));
					this->clips[this->currentClip]->setObjectName(this->clipboard->text());
				}
		});			
}

void ClipboardPlug::unloadPlug(void)
{
	QObject::disconnect(this->clipCon);
	delete this->cliboardMenu;
}

void ClipboardPlug::plugAbout(void)
{
	QMessageBox msgBox;

	QString txt="KKEditQT Clipboard Plugin\n\n©K.D.Hedger 2022\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";
	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("Plugin About");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.exec();
}

void ClipboardPlug::plugSettings(void)
{
	//TODO//
}

unsigned int ClipboardPlug::plugWants(void)
{
	return(DOABOUT);
}
