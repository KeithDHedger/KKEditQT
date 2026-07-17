
#include "credits.h"
#include "KKEditClass.h"

#ifndef _QT_ABOUTBOX_
#define _QT_ABOUTBOX_

class AboutBoxClass : public QObject
{
	public:
		AboutBoxClass(QWidget *window,QString pixpath=NULL);
		~AboutBoxClass();

		QString		licence;
		QString		credits;

		void			runAbout(void);
		void			setHomepage(QString hpaddr,QString hpstr);
		void			setBodyText(QString bodystr);
		void			showAboutQtButton(bool show=false);
		void			showLicenceButton(bool show=false);
		void			showCreditsButton(bool show=false);
		void			showHelp(QString htmlpath);

	private:
		QDialog		*aboutDialog;
		QDialog		*licenceDialog;
		QDialog		*creditsDialog;
		QLabel		*hpLabel;
		QLabel		*bodyLabel;
		QLabel		*appNameLabel;
		QPushButton	*aboutQtButton;
		QPushButton	*licenseButton;
		QPushButton	*creditsButton;

		void			setAppName(void);
		void			showLicence(void);
		void			showCredits(void);
		void			killLicenceBox(void);
		void			killAboutBox(void);
		void			killCreditsBox(void);
};

#endif
