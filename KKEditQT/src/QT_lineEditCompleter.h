
#ifndef _QT_LINEEDITCOMPLETER_
#define _QT_LINEEDITCOMPLETER_

enum {STRINGCOMPLETE=0,FOLDERCOMPLETE};

class QT_lineEditCompleterClass: public QLineEdit
{
	public:
		QT_lineEditCompleterClass(const QString &contents,QWidget *parent=nullptr);
		~QT_lineEditCompleterClass();

		void				setCompleteType(int type);
		void				setUpCompleter(QStringList sl=QStringList());
		void				setRootFolder(QString path);
		void				setStrings(QStringList sl);

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
		int				completionType=STRINGCOMPLETE;

		QStringList		completeForPrefix(QString typed);
};

#endif
