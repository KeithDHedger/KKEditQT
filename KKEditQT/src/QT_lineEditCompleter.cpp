/*
 *
 * ©K. D. Hedger. Fri  7 Aug 20:44:30 BST 2026 keithdhedger@gmail.com

 * This file (QT_lineEditCompleter.cpp) is part of KKEditQT.

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

#include "QT_lineEditCompleter.h"

QT_lineEditCompleterClass::~QT_lineEditCompleterClass()
{
}

void QT_lineEditCompleterClass::setRootFolder(QString path)
{
	this->rootFold=path;
	this->holdFold=this->rootFold;
}

QT_lineEditCompleterClass::QT_lineEditCompleterClass(const QString &contents,QWidget *parent)
{
	this->setText(contents);
	this->setParent(parent);
	this->setClearButtonEnabled(true);

	QObject::connect(this,&QLineEdit::textEdited,[this](const QString &text)
		{
			switch(this->completionType)
				{
					case FOLDERCOMPLETE:
							if(this->text().startsWith('/'))
								this->rootFold="/";
							else
								this->rootFold=this->holdFold;

							this->folderModel->setStringList(this->completeForPrefix(this->text()));
						break;
					default:
						break;
				}

			this->completer->complete();
			this->completer->popup()->scrollToTop();
		});
}

void QT_lineEditCompleterClass::setCompleteType(int type)
{
	this->completionType=type;
}

QStringList QT_lineEditCompleterClass::completeForPrefix(QString typed)
{
	QString			prefix=typed;
	QDir				homeDir(this->rootFold);
	QDir				searchDir;
	QString			part="";
	QString			filterPart="";
	QStringList		results={};
	QString			relPath="";
	QString			name="";
	QFileInfoList	entries;
	int				slash=0;

	if(!homeDir.exists())
		return(QStringList());

	slash=prefix.lastIndexOf('/');
	searchDir=homeDir;
	part=prefix;

	if(slash>=0)
		{
			QString relDir=prefix.left(slash);
			part=prefix.mid(slash+1);
			searchDir=QDir(homeDir.filePath(relDir));
		}

	if(!searchDir.exists())
		return(QStringList());

	filterPart=part;
	entries=searchDir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries,QDir::Name);

	for(const QFileInfo &fi : entries)
		{
			name=fi.fileName();
			if(name.startsWith(filterPart,Qt::CaseInsensitive))
				{
					relPath=prefix;
					if(slash>=0)
						relPath=prefix.left(slash+1)+name;
					else
						relPath=name;

					if(fi.isDir())
						relPath+='/';

					results<<relPath;
				}
		}
	return results;
}

void QT_lineEditCompleterClass::setStrings(QStringList sl)
{
	this->stringModel->setStringList(sl);
}

void QT_lineEditCompleterClass::setUpCompleter(QStringList sl)
{
	switch(this->completionType)
		{
			case STRINGCOMPLETE:
				this->stringModel=new QStringListModel(this);
				this->stringModel->setStringList(sl);
				this->completer=new QCompleter(this->stringModel,this);
				this->setCompleter(completer);
				break;
			case FOLDERCOMPLETE:
				this->folderModel=new QStringListModel(this);
				this->completer=new QCompleter(this->folderModel,this);
				this->setCompleter(completer);
				break;
		}

	this->completer->setCompletionMode(QCompleter::PopupCompletion);
	this->completer->setCaseSensitivity(Qt::CaseInsensitive);

	shortcutESC=new QShortcut(QKeySequence("Esc"),this);
	shortcutESC->setContext(Qt::WidgetShortcut);
	QObject::connect(shortcutESC,&QShortcut::activated,[&]()
		{
			this->setText(this->holdText);
			this->completer->popup()->hide();
		});

	shortcutTAB=new QShortcut(QKeySequence("Tab"),this);
	shortcutTAB->setContext(Qt::WidgetShortcut);
	QObject::connect(shortcutTAB,&QShortcut::activated,[this]()
		{
			switch(this->completionType)
				{
					case STRINGCOMPLETE:
						this->completer->setCompletionPrefix(this->text());
						this->completer->complete();
						this->completer->popup()->scrollToTop();
						break;
					case FOLDERCOMPLETE:
						if(this->text().startsWith('/'))
							{
								this->rootFold="/";
								this->folderModel->setStringList(completeForPrefix(this->text()));
							}
						else
							{
								this->completer->setCompletionPrefix("");
								this->folderModel->setStringList(completeForPrefix(this->text()));
							}
						this->completer->complete();
						this->completer->popup()->scrollToTop();
						break;
				}
		});
}

void QT_lineEditCompleterClass::focusInEvent(QFocusEvent *e)
{
	this->holdText=this->text();
	QLineEdit::focusInEvent(e);
}