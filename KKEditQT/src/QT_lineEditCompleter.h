/*
 *
 * ©K. D. Hedger. Fri 21 Aug 16:05:19 BST 2026 keithdhedger@gmail.com

 * This file (QT_lineEditCompleter.h) is part of KKEditQT.

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

#ifndef _QT_LINEEDITCOMPLETER_
#define _QT_LINEEDITCOMPLETER_

enum {STRINGCOMPLETE=0,FOLDERCOMPLETE};

class QT_lineEditCompleterClass: public QLineEdit
{
	public:
		QT_lineEditCompleterClass(const QString &contents,QWidget *parent=nullptr);
		~QT_lineEditCompleterClass();

		bool				useInternaleSC=true;

		void				setCompleteType(int type);
		void				setUpCompleter(QStringList sl=QStringList());
		void				setRootFolder(QString path);
		void				setStrings(QStringList sl);
		void				doActivateKey(void);
		void				doCancelKey(void);

	protected:
		void				focusInEvent(QFocusEvent *e);

	private:
		QCompleter		*completer=NULL;
		QShortcut		*shortcutESC=NULL;
		QShortcut		*shortcutTAB=NULL;
		QStringListModel	*folderModel=NULL;
		QStringListModel	*stringModel=NULL;
		QString			holdFold;
		QString			rootFold="/";
		QString			holdText="";
		QStringList		completeForPrefix(QString typed);
		int				completionType=STRINGCOMPLETE;

};

#endif
