/*
 *
 * ©K. D. Hedger. Sat 18 Jul 20:12:20 BST 2026 keithdhedger@gmail.com

 * This file (QT_SpellCheck.h) is part of KKEditQT.

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

#ifdef _ASPELL_

#ifndef _QT_SPELLCHECK_
#define _QT_SPELLCHECK_

#include <aspell.h>

class QT_SpellCheckClass
{
	public:
		QT_SpellCheckClass(QMainWindow *window);
		~QT_SpellCheckClass();

#ifdef USEPLAIN
		QPlainTextEdit	*te=NULL;
#else
		QTextEdit		*te=NULL;
#endif
		QString			badwordHiliteColour="#40000000";
		bool				changed=false;

		QStringList		getLanguageCodes(void);
		void				setLanguage(QString lang);
		void				doSpellCheckDoc(void);
		void				doSpellCheckWord(QString word);
		AspellConfig		*getConfig(void);

	private:
		QMainWindow		*mainWindow=NULL;
		QDialog			*spellCheckWord=NULL;
		QLabel			*badWordLabel=NULL;
		QComboBox		*wordListDropbox=NULL;
		QString			badWord;
		QString			goodWord;

		AspellConfig		*aspellConfig=NULL;
		AspellSpeller	*spellChecker=0;

		int				badwordstart=-1;
		int				badwordlen=0;
		bool				blockFlag=false;
		bool				cancelCheck=false;

		bool				checkTheWord(QString word);
		void				buildWordCheckDialog(void);
		void				doChangeWord(void);
};

#endif
#endif