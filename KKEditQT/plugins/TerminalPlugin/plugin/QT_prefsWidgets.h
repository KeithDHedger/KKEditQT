/*
 *
 * ©K. D. Hedger. Thu 11 Dec 14:35:30 GMT 2025 keithdhedger@gmail.com

 * This file (QT_prefsWidgets.h) is part of Classes.

 * Classes is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Classes is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Classes.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _QT_PREFSWIDGETS_
#define _QT_PREFSWIDGETS_

#include "../../../src/qtincs.h"

enum whatWidget{EDITWIDGET,COLOURWIDGET,FONTWIDGET};

class prefsWidgetsClass
{
	public:
		prefsWidgetsClass(QString prefsname,QWidget *parent=nullptr);
		~prefsWidgetsClass();

		bool						getAsFolder=false;
		QDialog					*window;

		void						startWindow(QString wname);
		void						addWidgetToWindow(QWidget *widget);
		void						finishWindow(bool addclear=false);

		bool						deletePrefs(void);
		void						setLabelWidth(int sze);
//colour
		QWidget					*getColourWidget(QString label,QString colour);
		QWidget					*getPrefsColourWidget(QString label,QString defaultcolour);
		QString					getColourValue(QString label);
		void						setColourPref(QString label);
		void						setAllColourPrefs(void);

//check
		QWidget					*getCheckWidget(QString label,bool state);
		QWidget					*getPrefsCheckWidget(QString label,bool defaultstate);
		bool						getCheckValue(QString label);
		void						setCheckPref(QString label);
		void						setAllCheckPrefs(void);

//combo
		QWidget					*getComboWidget(QString label,QStringList data,QString item="");
		QWidget					*getPrefsComboWidget(QString label,QStringList data,QString item="");
		QString					getComboValue(QString label);
		void						setComboPref(QString label);
		void						setAllComboPrefs(void);

//font
		QWidget					*getFontWidget(QString label,QString font);
		QWidget					*getPrefsFontWidget(QString label,QString defaultfont);
		QString					getFontValue(QString label);
		void						setFontPref(QString label);
		void						setAllFontPrefs(void);

//edits
		QWidget					*getEditWidget(QString label,QString text);
		QWidget					*getPrefsEditWidget(QString label,QString defaulttext);
		QString					getEditValue(QString label);
		void						setEditPref(QString label);
		void						setAllEditPrefs(void);

//spinner
		QWidget					*getSpinWidget(QString label,int min,int max,int val);
		QWidget					*getPrefsSpinWidget(QString label,int min,int max,int val);
		int						getSpinValue(QString label);
		void						setSpinPref(QString label);
		void						setAllSpinPrefs(void);

//file
		QWidget					*getFileWidget(QString label,QString path,QString openhere="");
		QWidget					*getPrefsFileWidget(QString label,QString path,QString openhere="");
		QString					getFileValue(QString label);
		void						setFilePref(QString label);
		void						setAllFilePrefs(void);

	private:
		QHash<QString,QString>	colours;
		QHash<QString,QString>	combos;
		QHash<QString,QString>	fonts;
		QHash<QString,QString>	edits;
		QHash<QString,QString>	files;
		QHash<QString,int>		spins;
		QHash<QString,bool>		checks;

		int						labelWidth=100;
		QWidget					*theParent=nullptr;
		QString					thePrefsName;
		QVBoxLayout				*windowVLayout;
		QString					bestFontColour(QString colour);
		void						setPref(QHash<QString,QString> what,QString label);
		void						setAllPrefs(QHash<QString,QString> what);
		QWidget					*getPrefsWidget(QString label,QString defaulttxt,whatWidget what);
};

#endif
