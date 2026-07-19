/*
 *
 * ©K. D. Hedger. Sun 19 Jul 14:11:59 BST 2026 keithdhedger@gmail.com

 * This file (QT_AboutBox.cpp) is part of KKEditQT.

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

#include "qtincs.h"
#include "QT_AboutBox.h"

void AboutBoxClass::runAbout(void)
{
	this->setAppName();
	this->aboutDialog->exec();
}

AboutBoxClass::~AboutBoxClass()
{
}

void AboutBoxClass::killCreditsBox(void)
{
	this->creditsDialog->close();
}

void AboutBoxClass::showCredits(void)
{
	QPushButton	*button;
	QHBoxLayout	*hlayout;
	QWidget		*hbox;
	QVBoxLayout	*vlayout=new QVBoxLayout;
	QTabWidget	*tabWidget=new QTabWidget;

	this->creditsDialog=new QDialog(this->aboutDialog);
	this->creditsDialog->setWindowTitle(QString("Credits For %1").arg(qApp->applicationDisplayName()));

	hlayout=new QHBoxLayout;
	hbox=new QWidget;
	hbox->setLayout(hlayout);

	QTextBrowser	*text=new QTextBrowser;
	text->setHtml(this->credits);
	text->setReadOnly(true);
	text->setOpenExternalLinks(true);

	tabWidget->addTab(text,"Written By");
	vlayout->addWidget(tabWidget);

//close
	hlayout->setContentsMargins(0,0,0,0);
	button=new QPushButton("&Close");
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			this->killCreditsBox();
		});
	hlayout->addWidget(button);

	button->setIcon(QIcon::fromTheme("window-close"));
	hlayout->insertStretch(0,1);
	vlayout->addWidget(hbox);

	creditsDialog->setLayout(vlayout);
	creditsDialog->setModal(true);
	creditsDialog->resize(500,320);
	creditsDialog->exec();

	delete text;
	delete button;
	delete hlayout;
	delete vlayout;
	delete this->creditsDialog;
}

void AboutBoxClass::killLicenceBox(void)
{
	this->licenceDialog->close();
}

void AboutBoxClass::killAboutBox(void)
{
	this->aboutDialog->close();
}

void AboutBoxClass::showLicence(void)
{
	QPushButton		*button;
	QHBoxLayout		*hlayout;
	QWidget*			hbox;
	QVBoxLayout		*vlayout=new QVBoxLayout;
	QPlainTextEdit	*text=new QPlainTextEdit;

	this->licenceDialog=new QDialog(this->aboutDialog);
	hlayout=new QHBoxLayout;
	hbox=new QWidget;
	hbox->setLayout(hlayout);

	this->licenceDialog->setWindowTitle(QString("Licence For %1").arg(qApp->applicationDisplayName()));

	vlayout->setContentsMargins(0,4,4,4);
	text->setPlainText(this->licence);
	text->setReadOnly(true);
	text->setLineWrapMode(QPlainTextEdit::NoWrap);
	vlayout->addWidget(text);

//close
	hlayout->setContentsMargins(0,0,0,0);
	button=new QPushButton("&Close");
	button->setIcon(QIcon::fromTheme("window-close"));
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			this->killLicenceBox();
		});
	hlayout->addWidget(button);

	hlayout->insertStretch(0,1);
	vlayout->addWidget(hbox);

	this->licenceDialog->setLayout(vlayout);
	this->licenceDialog->setModal(true);
	this->licenceDialog->resize(500,320);
	this->licenceDialog->exec();

	delete text;
	delete button;
	delete hlayout;
	delete vlayout;
	delete this->licenceDialog;
}

void AboutBoxClass::setHomepage(QString hpaddr,QString hpstr)
{
	this->hpLabel->setText(QString("<a href=\"%1\">%2</a>").arg(hpaddr).arg(hpstr));
}

void AboutBoxClass::setBodyText(QString bodystr)
{
	this->bodyLabel->setText(bodystr);
}

void AboutBoxClass::setAppName(void)
{
	QString xtradata="";

	if(qApp->applicationVersion().isEmpty()==false)
		xtradata=QString("<br><font size=\"3\">Version %1</font>").arg(qApp->applicationVersion());
	
	this->appNameLabel->setText(QString("<font size=\"5\"><b>%1</b></font>%2").arg(qApp->applicationDisplayName()).arg(xtradata));
}

AboutBoxClass::AboutBoxClass(QWidget *window,QString pixpath)
{
	QVBoxLayout	*vlayout=new QVBoxLayout;
	QWidget		*hbox;
	QHBoxLayout	*hlayout;
	QPushButton	*button;

	QPixmap		pic(pixpath);
	QLabel*		label=new QLabel;

	this->aboutDialog=new QDialog(window);
	this->aboutDialog->setWindowTitle(QString("About %1").arg(qApp->applicationDisplayName()));
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
	appNameLabel=new QLabel;
	appNameLabel->setText("");
	appNameLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	vlayout->addWidget(appNameLabel);
//text
	this->bodyLabel=new QLabel;
	this->bodyLabel->setText("");
	this->bodyLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	vlayout->addWidget(this->bodyLabel);
//copyrite
	label=new QLabel;
	label->setText("<small>" COPYRITE "</small>");
	label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	vlayout->addWidget(label);
//homepage
	this->hpLabel=new QLabel;
	this->hpLabel->setText("<a href=\"https://keithdhedger.github.io/\">Home Page</a>");
	this->hpLabel->setOpenExternalLinks(true);
	this->hpLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	vlayout->addWidget(this->hpLabel);

//credits
	this->creditsButton=new QPushButton("&Credits");
	this->creditsButton->setIcon(QIcon::fromTheme("text-x-credits"));
	QObject::connect(this->creditsButton,&QPushButton::clicked,[this]()
		{
			this->showCredits();
		});
	hlayout->addWidget(this->creditsButton);

//licence
	this->licenseButton=new QPushButton("&Licence");
	this->licenseButton->setIcon(QIcon::fromTheme("text-x-license"));
	QObject::connect(this->licenseButton,&QPushButton::clicked,[this]()
		{
			this->showLicence();
		});
	hlayout->addWidget(this->licenseButton);

//about qt
	this->aboutQtButton=new QPushButton("&About Qt");
	this->aboutQtButton->setIcon(QIcon::fromTheme("help-about"));
	QObject::connect(this->aboutQtButton,&QPushButton::clicked,[this]()
		{
			qApp->aboutQt();
		});
	hlayout->addWidget(this->aboutQtButton);

//close
	button=new QPushButton("&Close");
	button->setIcon(QIcon::fromTheme("window-close"));
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			this->killAboutBox();
		});
	hlayout->addWidget(button);

	vlayout->addWidget(hbox);
	this->aboutDialog->setLayout(vlayout);
}

void AboutBoxClass::showAboutQtButton(bool show)
{
	this->aboutQtButton->setVisible(show);
}

void AboutBoxClass::showLicenceButton(bool show)
{
	this->licenseButton->setVisible(show);
}

void AboutBoxClass::showCreditsButton(bool show)
{
	this->creditsButton->setVisible(show);
}

void AboutBoxClass::showHelp(QString htmlpath)
{
	QDialog			dialog;
	QVBoxLayout		*layout;
	QTextBrowser		*textBrowser=new QTextBrowser(&dialog);

	textBrowser->setSource(QUrl(htmlpath));
	layout=new QVBoxLayout(&dialog);
    layout->addWidget(textBrowser);
 
	QDialogButtonBox *buttonBox=new QDialogButtonBox(QDialogButtonBox::Close,&dialog);
	QObject::connect(buttonBox,&QDialogButtonBox::rejected,&dialog,&QDialog::reject);
	layout->addWidget( buttonBox);

    dialog.setWindowTitle(QString("Help For %1").arg(qApp->applicationDisplayName()));
    dialog.resize(640,480);
    dialog.exec();
}
