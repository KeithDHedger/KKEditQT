/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:37:27 GMT 2021 keithdhedger@gmail.com

 * This file (QT_document.h) is part of KKEditQT.

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

#ifndef _QT_DOCUMENT_
#define _QT_DOCUMENT_

#include "KKEditClass.h"

class LineNumberArea;

enum {NORMALTAB=0,CHANGEDONDISKTAB,IGNORECHANGEDONDISKTAB,LOCKEDTAB,DIRTYTAB};

class DocumentClass : public QPlainTextEdit
{
	public:
		explicit DocumentClass(KKEditClass *kk,QWidget *parent=0);
		~DocumentClass();

		KKEditClass						*mainKKEditClass=NULL;
//highlighting
		KSyntaxHighlighting::SyntaxHighlighter	*highlighter2;

		QList<QTextEdit::ExtraSelection>	extraSelections;
		QList<QTextEdit::ExtraSelection>	extraBMSelections;
		QList<QTextEdit::ExtraSelection>	hilightSelections;
		QList<QTextEdit::ExtraSelection>	bracketMatch;
		QList<QTextEdit::ExtraSelection>	findMatch;
		QList<QTextEdit::ExtraSelection>	verticalSelectMatch;

		QTextEdit::ExtraSelection		selection;
		QTextEdit::ExtraSelection		selectedLine;
		int								oldBlockCount=0;

		void								setTheme(QString name);
		void								setHiliteLanguage(QString name);
		void								lineNumberAreaPaintEvent(QPaintEvent *event);
		int								lineNumberAreaWidth(void);
		void								setXtraSelections(void);
		void								addXtraSelections(void);
		void								clearHilites(void);

		void								setFileName(const QString filename);
		const QString					getFileName(void);
		void								setFilePath(const QString filename);
		const QString					getFilePath(void);
		void								setDirPath(const QString dirname);
		const QString					getDirPath(void);
		void								setTabName(QString tabname);
		const QString					getTabName(void);
		int								getCurrentLineNumber(void);

		void								setFilePrefs(void);
		void								setStatusBarText(void);
		void								setBMFromLineBar(QMouseEvent *event);

		void								refreshFromDisk(void);
		void								setTabColourType(int type);

		void								makeDirty(void);
		void								makeClean(void);

//pageStruct
		QString							fileName;
		QString							filePath;
		QString							dirPath;
		QString							tabName;
		bool								doneHighlightAll;
		char								*lastFind;
		bool								dirty=false;
		bool								realChange=false;
		QString							mimeType="text/plain";
		bool								gotUndo=false;
		bool								gotRedo=false;
		int								pageIndex;
		bool								visible=true;
		bool								fromMe=false;
		bool								modifiedOnDisk=false;
		int								state=NORMALTAB;

//find/replace
		int								searchPos=-1000;
		int								totalMatches=0;
		bool								changeDirection=false;
		bool								finishedSearch=false;
		bool								findStr(int what=FINDNEXT);
		void								setHighlightAll(void);
		const QString					textUnderCursor();

//vertical selection
		QString							verticalText="";

	protected:
	    void								resizeEvent(QResizeEvent *event);
		void								keyPressEvent(QKeyEvent *event);
		void								keyReleaseEvent(QKeyEvent *event);
		void								focusInEvent(QFocusEvent *e);
		void								contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
		void								mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void								mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void								mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void								mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

		void								paintEvent(QPaintEvent* event);
//dand
		void								dragEnterEvent(QDragEnterEvent* event);
		void								dragLeaveEvent(QDragLeaveEvent* event);
		void								dropEvent(QDropEvent* event);
		void								dragMoveEvent(QDragMoveEvent *event);

	private slots:
		void								highlightCurrentLine();
		void								updateLineNumberAreaWidth(int newcnt);
		void								updateLineNumberArea(const QRect &, int);
		void								modified();
		void								setUndo(bool avail);
		void								setRedo(bool avail);

	private:
		bool								inDrag=false;
		QWidget 							*lineNumberArea;
		QString							indentPad;
		void								clearXtraSelections(void);
		bool								realShowLineNumbers(void);
		bool								realHiliteLine(void);
		bool								realSyntaxHighlighting(void);
		QString							bestFontColour(QString colour);
		QString							repoColourToHex(KSyntaxHighlighting::Theme::EditorColorRole);
	
		QTextCursor						holdCursor;
		bool								lastCursorPosition=false;
		int								holdColoumn=0;
		bool								holdBlockEnd=false;
//vertical selection
		bool								doingVSelect=false;
		int								startVCol=0;
		int								startVLine=0;
};

class LineNumberArea : public QWidget
{
	public:
	    LineNumberArea(DocumentClass *editor) : QWidget(editor)
			{
				codeEditor=editor;
			}

		QSize sizeHint() const
			{
				return QSize(codeEditor->lineNumberAreaWidth(),0);
			}

	protected:
		void paintEvent(QPaintEvent *event)
			{
				codeEditor->lineNumberAreaPaintEvent(event);
			}

		void mouseReleaseEvent(QMouseEvent *event)
			{
				this->codeEditor->setBMFromLineBar(event);
			}
	private:
		DocumentClass *codeEditor;
};

#endif



