/*
 *
 * ©K. D. Hedger. Sun  9 Jan 14:05:01 GMT 2022 keithdhedger@gmail.com

 * This file (QT_notebook.cpp) is part of KKEditQT.

 * KKEditQT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * KKEditQT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with KKEditQT.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "QT_toolbar.h"

NoteBookClass::~NoteBookClass()
{
}

NoteBookClass::NoteBookClass(KKEditClass *kk,QWidget *parent): QTabWidget(parent)
{
	QIcon	qicon;

	this->mainKKEditClass=kk;
	this->setAcceptDrops(true);
	this->scrollLeft=new QToolButton(this);
	this->scrollRight=new QToolButton(this);
	qicon=QIcon::fromTheme("go-previous");
	this->scrollLeft->setIcon(qicon);
	qicon=QIcon::fromTheme("go-next");
	this->scrollRight->setIcon(qicon);

	this->setCornerWidget(this->scrollLeft,Qt::TopLeftCorner);
	this->setCornerWidget(this->scrollRight,Qt::TopRightCorner);

	this->scrollLeft->setAutoRepeat(true);
	this->scrollRight->setAutoRepeat(true);
	QObject::connect(this->scrollLeft,&QPushButton::clicked,[this]()
		{
			this->scrollTabsLeft();
		});
	QObject::connect(this->scrollRight,&QPushButton::clicked,[this]()
		{
			this->scrollTabsRight();
		});
	this->tabBar()->setExpanding(false);
	this->tabBar()->setElideMode(Qt::ElideNone);

//this->tabBar()->setStyleSheet(QString("QTabBar::scroller{width: 0px;}\nQTabBar::right-corner {height: 64px;bottom: 20px;}"));//TODO//
	this->tabBar()->setStyleSheet(QString("QTabBar::scroller{width: 0px;}"));
}

void NoteBookClass::dragMoveEvent(QDragMoveEvent *event)
{
	int tabIndex=this->tabBar()->tabAt(event->pos());
	this->setCurrentIndex(tabIndex);

	if((event->mimeData()->hasUrls()==true) || (event->mimeData()->hasText()))
		event->accept();
	else
		QTabWidget::dragMoveEvent(event);
}

void NoteBookClass::dragEnterEvent(QDragEnterEvent* event)
{
	if((event->mimeData()->hasUrls()==true) || (event->mimeData()->hasText()))
		event->accept();
	else
		QTabWidget::dragEnterEvent(event);
}

void NoteBookClass::dropEvent(QDropEvent* event)
{
 	if(event->mimeData()->hasUrls())
		{
			if(event->mimeData()->urls().isEmpty()==false)
				{
					for(int j=0;j<event->mimeData()->urls().count();j++)
						this->mainKKEditClass->openFile(event->mimeData()->urls().at(j).toLocalFile());
				}
			event->accept();
			return;
		}

	QTabWidget::dropEvent(event);
}

void NoteBookClass::scrollTabsLeft(void)
{
	int	ctab=this->currentIndex()-1;

	while((ctab>-1) && (this->isTabVisible(ctab)==false))
		ctab--;

	if(ctab==-1)
		return;

	this->setCurrentIndex(ctab);
}

void NoteBookClass::scrollTabsRight(void)
{
	int	ctab=this->currentIndex()+1;

	while((ctab<this->count()) && (this->isTabVisible(ctab)==false))
		ctab++;

	if(ctab==this->count())
		ctab=this->count()-1;
	else
		this->setCurrentIndex(ctab);
}




