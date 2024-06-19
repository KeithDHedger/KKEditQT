/*
 *
 * ©K. D. Hedger. Thu 26 Nov 14:19:04 GMT 2015 keithdhedger@gmail.com

 * This file (QT_AboutBox.cpp) is part of AspellGUI.

 * AspellGUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * AspellGUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with AspellGUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "kkedit-includes.h"
#include "QT_AboutBox.h"
#ifndef moc_QT_AboutBox
#include "moc_QT_AboutBox.cpp"
#define moc_QT_AboutBox
#endif

void AboutBoxClass::setAuthors(QString authors)
{
	this->authors=authors;
}

void AboutBoxClass::runAbout(void)
{
	this->aboutDialog->exec();
}

void AboutBoxClass::setLicence(QString licence)
{
	this->licence=licence;
}

AboutBoxClass::~AboutBoxClass()
{
}

void AboutBoxClass::showCredits(void)
{
	QPushButton	*button;
	QHBoxLayout	*hlayout;
	QWidget		*hbox;
	QVBoxLayout	*vlayout=new QVBoxLayout;
	QTabWidget	*tabWidget=new QTabWidget;

	this->creditsDialog=new QDialog(this->aboutDialog);
	this->creditsDialog->setWindowTitle("Credits");
	hlayout=new QHBoxLayout;
	hbox=new QWidget;
	hbox->setLayout(hlayout);

	QTextBrowser	*text=new QTextBrowser;
	text->setText(this->authors);
	text->setReadOnly(true);
	text->setOpenExternalLinks(true);

	tabWidget->addTab(text,"Written By");
	vlayout->addWidget(tabWidget);

//close
	hlayout->setContentsMargins(0,0,0,0);
	button=new QPushButton("&Close");
	QObject::connect(button,&QPushButton::clicked,[=]() {this->creditsDialog->close();});
	hlayout->addWidget(button);

	button->setIcon(QIcon::fromTheme("window-close"));
	hlayout->insertStretch(0,1);
	vlayout->addWidget(hbox);

	creditsDialog->setLayout((QLayout*)vlayout);
	creditsDialog->setModal(true);
	creditsDialog->resize(500,320);
	creditsDialog->exec();

	delete text;
	delete button;
	delete hlayout;
	delete vlayout;
	delete this->creditsDialog;
}

void AboutBoxClass::killAboutBox(void)
{
	this->aboutDialog->close();
	delete this;
}

void AboutBoxClass::showLicence(void)
{
	QPushButton		*button;
	QHBoxLayout		*hlayout;
	QWidget			*hbox;
	QVBoxLayout		*vlayout=new QVBoxLayout;
	QPlainTextEdit	*text=new QPlainTextEdit;

	this->licenceDialog=new QDialog(this->aboutDialog);
	hlayout=new QHBoxLayout;
	hbox=new QWidget;
	hbox->setLayout(hlayout);

	this->licenceDialog->setWindowTitle("Licence");
	vlayout->setContentsMargins(0,4,4,4);
	text->setPlainText(this->licence);
	text->setReadOnly(true);
	text->setLineWrapMode(QPlainTextEdit::NoWrap);
	vlayout->addWidget(text);

//close
	hlayout->setContentsMargins(0,0,0,0);

	button=new QPushButton("&Close");
	QObject::connect(button,&QPushButton::clicked,[=]() {this->licenceDialog->close();});
	button->setIcon(QIcon::fromTheme("window-close"));
	hlayout->addWidget(button);

	hlayout->insertStretch(0,1);
	vlayout->addWidget(hbox);

	this->licenceDialog->setLayout((QLayout*)vlayout);
	this->licenceDialog->setModal(true);
	this->licenceDialog->resize(500,320);
	this->licenceDialog->exec();

	delete text;
	delete button;
	delete hlayout;
	delete vlayout;
	delete this->licenceDialog;
}

AboutBoxClass::AboutBoxClass(QWidget* window,QString pixpath)
{
	QVBoxLayout	*vlayout=new QVBoxLayout;
	QWidget		*hbox;
	QHBoxLayout	*hlayout;
	QPushButton	*button;
	QPixmap		pic(pixpath);
	QLabel		*label=new QLabel;

	this->aboutDialog=new QDialog(window);
	this->aboutDialog->setWindowTitle("About");
	this->aboutDialog->resize(320,128);
	this->aboutDialog->setModal(true);

	vlayout->setContentsMargins(0,4,0,0);
	hlayout=new QHBoxLayout;
	hbox=new QWidget;
	hbox->setLayout(hlayout);

//icon
	label->setPixmap(pic);
	label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	vlayout->addWidget(label);
//progname
	label=new QLabel;
	label->setText("<font size=\"5\"><b>" PACKAGE_STRING "</b></font>");
	label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	vlayout->addWidget(label);
//text
	label=new QLabel;
	label->setText("KKEditQT Code Text Editor");
	label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	vlayout->addWidget(label);
//copyrite
	label=new QLabel;
	label->setText("<small>" COPYRITE "</small>");
	label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	vlayout->addWidget(label);
//homepage
	label=new QLabel;
	label->setText("<a href=\"" KKEDITQTPAGE "\">" PACKAGE "</a>");
	label->setOpenExternalLinks(true);
	label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	vlayout->addWidget(label);
//credits
	button=new QPushButton("&Credits");
	QObject::connect(button,&QPushButton::clicked,[=]() {this->showCredits();});
	hlayout->addWidget(button);
	button->setIcon(QIcon::fromTheme("help-about"));

//licence
	button=new QPushButton("&Licence");
	QObject::connect(button,&QPushButton::clicked,[=]() {this->showLicence();});
	hlayout->addWidget(button);

//close
	button=new QPushButton("&Close");
	QObject::connect(button,&QPushButton::clicked,[=]() {this->killAboutBox();});
	hlayout->addWidget(button);
	button->setIcon(QIcon::fromTheme("window-close"));

	vlayout->addWidget(hbox);
	this->aboutDialog->setLayout((QLayout*)vlayout);
}
