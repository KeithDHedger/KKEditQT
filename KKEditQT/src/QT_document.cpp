/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:37:12 GMT 2021 keithdhedger@gmail.com

 * This file (QT_document.cpp) is part of KKEditQT.

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

#include "QT_document.h"

int DocumentClass::getCurrentLineNumber(void)
{
	return(this->textCursor().blockNumber()+1);
}

bool DocumentClass::realShowLineNumbers(void)
{
	return(this->mainKKEditClass->lineNumbersVisible);
}

bool DocumentClass::realHiliteLine(void)
{
	return(this->mainKKEditClass->hiliteCurrentLine);
}

bool DocumentClass::realSyntaxHighlighting(void)
{
	return(this->mainKKEditClass->showHighLighting);
}

void DocumentClass::lineNumberAreaPaintEvent(QPaintEvent *event)
{
	if(this->realShowLineNumbers()==false)
		return;

	QPainter painter(this->lineNumberArea);

	painter.fillRect(event->rect(),this->highlighter->lineNumbersBackground);
	QTextBlock block=this->firstVisibleBlock();
	int blockNumber=block.blockNumber();
	int top=(int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom=top+(int) blockBoundingRect(block).height();

	while(block.isValid() && top<=event->rect().bottom())
		{
			if(block.isVisible() && bottom>=event->rect().top())
				{
					painter.setPen(this->highlighter->lineNumbersForeground);
					foreach(bookMarkStruct value,this->mainKKEditClass->bookMarks)
						{
							if((value.docIndex==this->pageIndex) && (value.line==blockNumber+1))
								{
									painter.fillRect(QRect(0,top,lineNumberArea->width(),fontMetrics().height()),this->highlighter->bookMarkBGColour);
								}
						}

					QString number=QString::number(blockNumber+1);painter.setPen(this->highlighter->lineNumbersForeground);
					painter.drawText(0,top,lineNumberArea->width(),fontMetrics().height(),Qt::AlignRight,number);
				}

			block=block.next();
			top=bottom;
			bottom=top+(int) blockBoundingRect(block).height();
			blockNumber++;
		}
}

int DocumentClass::lineNumberAreaWidth(void)
{
	int digits=1;
	int max=qMax(1,blockCount());
	while (max>=10)
		{
			max/=10;
			digits++;
		}
	int space=3+fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

	return space;
}

void DocumentClass::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	if(this->realShowLineNumbers()==false)
		lineNumberArea->setGeometry(0,0,0,0);

	QRect cr=contentsRect();
	lineNumberArea->setGeometry(QRect(cr.left(),cr.top(),lineNumberAreaWidth(),cr.height()));
}

DocumentClass::~DocumentClass()
{
	this->mainKKEditClass->pages.remove(this->pageIndex);
}

void DocumentClass::updateLineNumberAreaWidth(int newcnt)
{
	if(this->realShowLineNumbers()==true)
		setViewportMargins(lineNumberAreaWidth(),0,0,0);
	else
		setViewportMargins(0,0,0,0);

	if(this->oldBlockCount!=newcnt)
		{
			int			diff=newcnt-this->oldBlockCount;
			QTextCursor	txtc=this->textCursor();
			int			oldline=this->getCurrentLineNumber()-diff;
			bool			dore=false;
			foreach(bookMarkStruct value,this->mainKKEditClass->bookMarks)
				{
					if(value.docIndex==this->pageIndex)
						{
							if((oldline)==(value.line))
								{
									if(this->holdBlockEnd==true)
										continue;
									if(this->holdColoumn==0)
										{
											this->mainKKEditClass->bookMarks[value.bmKey].line+=diff;	
											dore=true;
										}
									continue;
								}

							if((oldline)==(value.line+diff))
								continue;

							if((oldline)<(value.line))
								{
									this->mainKKEditClass->bookMarks[value.bmKey].line+=diff;	
									dore=true;
									continue;
								}
						}
				}
			if(dore==true)
				this->highlightCurrentLine();
		}

	this->oldBlockCount=newcnt;
}

void DocumentClass::modified()
{
	if((this->mainKKEditClass->sessionBusy==true) || (this->dirty==true))
		return;

	this->dirty=true;

	this->mainKKEditClass->mainNotebook->indexOf(this);
	if(this->modifiedOnDisk==false)
		this->state=DIRTYTAB;
	else
		this->state=IGNORECHANGEDONDISKTAB;

	this->setTabColourType(state);
}

void DocumentClass::setStatusBarText(void)
{
	QString text=QString("Line %1\tCol %2\tSessionId 0x%3\t\tFilePath %4").arg(this->getCurrentLineNumber()).arg(this->textCursor().positionInBlock()+1).arg(this->mainKKEditClass->sessionID,0,16).arg(this->filePath);
	this->mainKKEditClass->statusText->setText(text);
	this->mainKKEditClass->setToolbarSensitive();
}

void DocumentClass::highlightCurrentLine()
{
	QTextEdit::ExtraSelection	bmselect;
	bool							holdsb=this->mainKKEditClass->sessionBusy;
	QTextCursor					cursor=this->textCursor();

	this->setStatusBarText();
	this->extraBMSelections.clear();
	foreach(bookMarkStruct value,this->mainKKEditClass->bookMarks)
		{
			if(value.docIndex==this->pageIndex)
				{
					QTextBlock block=this->document()->findBlockByNumber(value.line-1);
					QTextCursor cursor1(block);
					bmselect.format.setBackground(this->bookmarkLineColor);
					bmselect.format.setProperty(QTextFormat::FullWidthSelection, true);
					bmselect.cursor=cursor1;
					bmselect.cursor.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
					bmselect.cursor.movePosition(QTextCursor::NextBlock,QTextCursor::KeepAnchor);
					this->extraBMSelections.append(bmselect);			
				}
		}

	this->selectedLine.format.setBackground(this->prefsHiLiteLineColor);
	this->selectedLine.format.setProperty(QTextFormat::FullWidthSelection, true);
	this->selectedLine.cursor=textCursor();

	this->selectedLine.cursor.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
	this->selectedLine.cursor.movePosition(QTextCursor::NextBlock,QTextCursor::KeepAnchor);

//bracket match
//forward
	this->bracketMatch.clear();
	QString			txt=this->toPlainText();
	QTextCursor		bracketcursor=this->textCursor();
	const QString	openbrackets("({[<");
	const QString	closebrackets(")}]>");
	int				pos;
	int				whatbracket;
	int				stack;
	int				cnt;
	bool			gotone=false;

	if(txt.length()>1)
		{
			pos=bracketcursor.position();
			whatbracket=-1;

			if(pos>=txt.length())
				pos=txt.length()-1;

			for(int j=0;j<openbrackets.length();j++)
				{
					if(txt.at(pos)==openbrackets.at(j))
						whatbracket=j;
				}

			if(whatbracket!=-1)
				{
					gotone=true;
					stack=1;
					cnt=pos;
					do
						{
							cnt++;
							if(cnt==txt.length())
								break;
							if(txt.at(cnt)==openbrackets.at(whatbracket))
								stack++;
							if(txt.at(cnt)==closebrackets.at(whatbracket))
								stack--;
						}
					while(stack!=0);
				}

//backwards
			if(gotone==false)
				{
					pos=bracketcursor.position()-1;
					whatbracket=-1;

					if(pos<0)
						pos=0;

					for(int j=0;j<closebrackets.length();j++)
						{
							if(txt.at(pos)==closebrackets.at(j))
								whatbracket=j;
						}

					if(whatbracket!=-1)
						{
							stack=1;
							cnt=pos;
							do
								{
									cnt--;
									if(cnt==-1)
										break;
									if(txt.at(cnt)==closebrackets.at(whatbracket))
										stack++;
									if(txt.at(cnt)==openbrackets.at(whatbracket))
										stack--;
								}
							while(stack!=0);
						}
				}

			if(stack==0)
				{
					QTextEdit::ExtraSelection	bmselect;
					bracketcursor.setPosition(pos);
					bmselect.format.setBackground(QColor(this->highlighter->documentForeground));
					bmselect.format.setForeground(QColor(this->highlighter->documentBackground));
					bracketcursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
					bmselect.cursor=bracketcursor;
					this->bracketMatch.append(bmselect);

					bracketcursor.setPosition(cnt);
					bracketcursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
					bmselect.cursor=bracketcursor;
					this->bracketMatch.append(bmselect);
				}
		}

	if(this->realHiliteLine()==false)
		{
			this->extraSelections.clear();
			this->setExtraSelections(this->extraSelections);
		}
	else
		this->setXtraSelections();

	this->mainKKEditClass->sessionBusy=holdsb;
	this->mainKKEditClass->setToolbarSensitive();
}

void DocumentClass::setXtraSelections(void)
{
	this->extraSelections.clear();
	this->extraSelections=this->extraSelections+this->hilightSelections;
	this->extraSelections.append(this->selectedLine);
	this->extraSelections.append(this->extraBMSelections);
	this->extraSelections.append(this->bracketMatch);
	this->extraSelections.append(this->findMatch);
	this->setExtraSelections(this->extraSelections);
}

void DocumentClass::addXtraSelections()
{
}

void DocumentClass::clearXtraSelections()
{
	this->extraSelections.clear();
	this->setXtraSelections();
}

void DocumentClass::clearHilites()
{
	this->findMatch.clear();
	this->hilightSelections.clear();
	this->setXtraSelections();
}

void DocumentClass::updateLineNumberArea(const QRect &rect,int dy)
{
	if(this->realShowLineNumbers()==false)
		return;

	if(dy)
		lineNumberArea->scroll(0,dy);
	else
		lineNumberArea->update(0,rect.y(),lineNumberArea->width(),rect.height());

	if(rect.contains(viewport()->rect()))
		updateLineNumberAreaWidth(this->oldBlockCount);
}

void DocumentClass::focusInEvent(QFocusEvent *e)
{
    if (this->mainKKEditClass->completer)
        this->mainKKEditClass->completer->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

const QString DocumentClass::textUnderCursor()
{
    QTextCursor tc=this->textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void DocumentClass::paste(void)
{
QPlainTextEdit::paste();
#if 0
int startline=this->getCurrentLineNumber();
qDebug()<<this->getCurrentLineNumber();
QClipboard *clipboard = QGuiApplication::clipboard();

//qDebug()<<"paste"<<clipboard->text()<<clipboard->text(QClipboard::Selection);

qDebug()<<this->getCurrentLineNumber();
int diff=this->getCurrentLineNumber()-startline;
	foreach(bookMarkStruct value,this->mainKKEditClass->bookMarks)
		{
			if(value.docIndex==this->pageIndex)
				{
					if(this->getCurrentLineNumber()<value.line)
						{
							this->mainKKEditClass->bookMarks[value.bmKey].line+=diff;
						}
				}
		}
#endif
}

void DocumentClass::keyPressEvent(QKeyEvent *event)
{
	bool				isshortcut;
	bool				ctrlorshift;
	bool				hasmodifier;
	QRect			cr;
	QString			completionPrefix;
	const QString	eow("~!@#$%^&*()+{}|:\"<>?,./;'[]\\-= \t\n"); // end of word;
	bool				popupflag=true;

	this->holdColoumn=this->textCursor().positionInBlock();
	this->holdBlockEnd=this->textCursor().atBlockEnd();

	if(this->isReadOnly()==true)
		return;

//fix for vnc/linuxfb tab key
	if(((this->mainKKEditClass->application->platformName().compare("vnc")==0) || (this->mainKKEditClass->application->platformName().compare("linuxfb")==0)) && (event->key()==Qt::Key_Tab))
		{
			event->accept();
			this->insertPlainText("\t");
			return;
		}

	if((event->key()==Qt::Key_Return) && (this->mainKKEditClass->prefsIndent==true))
		{
			int j=0;
			QTextCursor	cursor=this->textCursor();
			QTextBlock	tb=cursor.block();
			QString		data=tb.text();
			int			posinblock=cursor.positionInBlock();
			this->indentPad="";
			while((j<posinblock) && ((data.at(j)=='\t') || (data.at(j)==' ')))
				this->indentPad+=data.at(j++);

			if(this->mainKKEditClass->completer && (this->mainKKEditClass->completer->popup()->isVisible()==false))
				{
					cursor.insertText(QString("\n%1").arg(this->indentPad));
					return;
				}
		}
		
	switch (event->key())
		{
			case Qt::Key_Backspace:
				popupflag=false;
				break;
			case Qt::Key_Tab:
				{
					QTextCursor	cursor=this->textCursor();
					QTextBlock	tb=cursor.block();
					QString		data=tb.text();
					int			posinblock=cursor.positionInBlock();
					if(data.mid(0,posinblock).simplified().isEmpty())
						this->lastCursorPosition=true;
				}				
				break;
			case Qt::Key_Down:
				{
					if(this->lastCursorPosition==true)
						{
							QTextCursor	cursor=this->textCursor();
							cursor.movePosition(QTextCursor::NextBlock);
							this->setTextCursor(cursor);
							return;
						}
				}
				break;
			case Qt::Key_Up:
				{
					if(this->lastCursorPosition==true)
						{
							QTextCursor	cursor=this->textCursor();
							cursor.movePosition(QTextCursor::PreviousBlock);
							this->setTextCursor(cursor);
							return;
						}
				}
				break;

			default:
				this->lastCursorPosition=false;
				break;
		}

	if((event->modifiers() & Qt::ControlModifier)==Qt::ControlModifier)
		popupflag=false;

	if(this->mainKKEditClass->completer && this->mainKKEditClass->completer->popup()->isVisible())
		{
        // The following keys are forwarded by the completer to the widget
			switch (event->key())
				{
					case Qt::Key_Enter:
					case Qt::Key_Return:
					case Qt::Key_Escape:
					case Qt::Key_Backtab:
					case Qt::Key_Tab:
						event->ignore();
						return; // let the completer do default behavior
					default:
						break;
				}
		}

	if(this->mainKKEditClass->showCompletions==false)
		{
			QPlainTextEdit::keyPressEvent(event);
			return;
		}

	isshortcut=((event->modifiers() & Qt::ControlModifier) && event->key()== Qt::Key_E);
	if(!this->mainKKEditClass->completer || !isshortcut) // do not process the shortcut when we have a completer
		QPlainTextEdit::keyPressEvent(event);

	ctrlorshift=event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
	if(!this->mainKKEditClass->completer || (ctrlorshift && event->text().isEmpty()))
		return;

	hasmodifier=(event->modifiers() != Qt::NoModifier) && !ctrlorshift;
	completionPrefix=this->textUnderCursor();

		if(popupflag==false)
			{
      	  		this->mainKKEditClass->completer->popup()->hide();
      	  		return;
     	  	}

    if(!isshortcut && (hasmodifier || event->text().isEmpty()|| completionPrefix.length() < this->mainKKEditClass->autoShowMinChars || eow.contains(event->text().right(1))))//
		{
      	  	this->mainKKEditClass->completer->popup()->hide();
      	  	return;
		}

	if (completionPrefix!=this->mainKKEditClass->completer->completionPrefix())
		{
			this->mainKKEditClass->completer->setCompletionPrefix(completionPrefix);
			this->mainKKEditClass->completer->popup()->setCurrentIndex(this->mainKKEditClass->completer->completionModel()->index(0, 0));
		}

	cr=this->cursorRect();
    cr.setWidth(this->mainKKEditClass->completer->popup()->sizeHintForColumn(0)+this->mainKKEditClass->completer->popup()->verticalScrollBar()->sizeHint().width());
   	if(this->mainKKEditClass->completer->widget()==this)
		this->mainKKEditClass->completer->complete(cr);
}

void DocumentClass::keyReleaseEvent(QKeyEvent *event)
{
	this->mainKKEditClass->setToolbarSensitive();
	QPlainTextEdit::keyReleaseEvent(event);
}

DocumentClass::DocumentClass(KKEditClass *kk,QWidget *parent): QPlainTextEdit(parent)
{
	this->doneHighlightAll=false;
	this->lastFind=NULL;
	this->mainKKEditClass=kk;
	this->setAcceptDrops(true);

	this->highlighter=new Highlighter(this->document(),this,this->mainKKEditClass);
	this->setCenterOnScroll(true);
	lineNumberArea=new LineNumberArea(this);

	connect(this,SIGNAL(blockCountChanged(int)),this,SLOT(updateLineNumberAreaWidth(int)));
	connect(this,SIGNAL(updateRequest(QRect,int)),this,SLOT(updateLineNumberArea(QRect,int)));
	connect(this,SIGNAL(cursorPositionChanged()),this,SLOT(highlightCurrentLine()));
	connect(this,SIGNAL(textChanged()),this,SLOT(modified()));

	connect(this,SIGNAL(undoAvailable(bool)),this,SLOT(setUndo(bool)));
	connect(this,SIGNAL(redoAvailable(bool)),this,SLOT(setRedo(bool)));

	updateLineNumberAreaWidth(this->oldBlockCount);
	highlightCurrentLine();
}

void DocumentClass::setFileName(const QString filename)
{
	this->fileName=filename;
}

const QString DocumentClass::getFileName(void)
{
	return(this->fileName);
}

void DocumentClass::setFilePath(const QString pathname)
{
	this->filePath=pathname;
}

const QString DocumentClass::getFilePath(void)
{
	return(this->filePath);
}

void DocumentClass::setDirPath(const QString dirname)
{
	this->dirPath=dirname;
}

const QString DocumentClass::getDirPath(void)
{
	return(this->dirPath);
}

void DocumentClass::setTabName(QString tabname)
{
	int tabnum=this->mainKKEditClass->mainNotebook->indexOf(this);

	if(this->dirty==false)
		this->mainKKEditClass->mainNotebook->tabBar()->setTabTextColor(tabnum,QColor(this->highlighter->documentForeground));

	if(this->tabName.compare(tabname)==0)
		return;

	this->tabName=tabname;
	
	this->mainKKEditClass->mainNotebook->setTabText(tabnum,tabname);
}

const QString DocumentClass::getTabName(void)
{
	return(this->tabName);
}

void DocumentClass::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu				menu(this);
	QList<QAction*>		menuactions;
	MenuItemClass		*mc;
	plugData				pd;

	menuactions=this->mainKKEditClass->bookMarkMenu->actions();
	menu.addAction(menuactions.at(TOGGLEBOOKMARKMENUITEM-REMOVEALLBOOKMARKSMENUITEM));
	menu.addSeparator();

	menuactions=qobject_cast<QMenu*>(this->mainKKEditClass->editMenu)->actions();
	menu.addAction(menuactions.at(UNDOMENUITEM-UNDOMENUITEM));
	menu.addAction(menuactions.at(REDOMENUITEM-UNDOMENUITEM));
	menu.addSeparator();
	menu.addAction(menuactions.at(CUTMENUITEM-UNDOMENUITEM));
	menu.addAction(menuactions.at(COPYMENUITEM-UNDOMENUITEM));
	menu.addAction(menuactions.at(PASTEMENUITEM-UNDOMENUITEM));
	menu.addAction(menuactions.at(DELETEMENUITEM-UNDOMENUITEM));
	menu.addSeparator();
	menu.addAction(menuactions.at(SELECTALLMENUITEM-UNDOMENUITEM));
	menu.addSeparator();

	menuactions=qobject_cast<QMenu*>(this->mainKKEditClass->navMenu)->actions();

	menu.addAction(menuactions.at(SEARCHGTKDOCS-GOTODEFINEMENUITEM));
	menu.addAction(menuactions.at(SEARCHQT5DOCS-GOTODEFINEMENUITEM));
	menu.addAction(menuactions.at(SEARCHDOXYDOCS-GOTODEFINEMENUITEM));
	menu.addAction(menuactions.at(GOTODEFINEMENUITEM-GOTODEFINEMENUITEM));
	menu.addSeparator();

//tools
	menuactions=qobject_cast<QMenu*>(this->mainKKEditClass->toolsMenu)->actions();
	for(int j=1;j<menuactions.count();j++)
		{
			mc=qobject_cast<MenuItemClass*>(menuactions.at(j));
			if(mc!=NULL)
				{
					if(mc->alwaysInPopup==true)
						menu.addAction(menuactions.at(j));
				}
		}

	menu.addSeparator();

	for(int j=1;j<menuactions.count();j++)
		{
			mc=qobject_cast<MenuItemClass*>(menuactions.at(j));
			if(mc!=NULL)
				{
					if(this->textCursor().hasSelection()==true)
						{
							if(mc->inPopup==true)
								menu.addAction(menuactions.at(j));
						}
				}
		}

	menu.addSeparator();
	
#ifdef _ASPELL_
	menu.addAction(this->mainKKEditClass->spellCheckMenuItem);
#endif
	menu.addSeparator();

//plugins
	pd.doc=this;
	pd.menu=&menu;
	pd.tabNumber=this->mainKKEditClass->mainNotebook->currentIndex();
	pd.what=DOCONTEXTMENU;
	this->mainKKEditClass->runAllPlugs(pd);

//	menu.setStyleSheet("QMenu { menu-scrollable: true ;padding: 0;}");
	menu.exec(event->globalPos());
}

void DocumentClass::setFilePrefs(void)
{
	bool			holddirty=this->dirty;
	QTextOption	opts;

	this->setTabStopDistance(1.0);
	this->dirty=true;
	this->highlighter->syntaxHighlighting=this->realSyntaxHighlighting();
	this->highlighter->setTheme();
	this->setStyleSheet(this->highlighter->docBackgroundCSS);

	this->dirty=holddirty;
	this->updateLineNumberAreaWidth(this->oldBlockCount);

	this->setFont(this->mainKKEditClass->prefsDocumentFont);
	this->prefsHiLiteLineColor=this->mainKKEditClass->prefsHiLiteLineColor;
	this->bookmarkLineColor=this->mainKKEditClass->prefsBookmarkHiLiteColor;
	this->highlightCurrentLine();

	this->setLineWrapMode(static_cast<QPlainTextEdit::LineWrapMode>(this->mainKKEditClass->wrapLine));

	opts.setFlags(opts.flags());
	if(this->mainKKEditClass->wrapLine==true)
		opts.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	else
		opts.setWrapMode(QTextOption::NoWrap);
		
	if(this->mainKKEditClass->showWhiteSpace==true)
		{
			opts.setFlags(QTextOption::ShowLineAndParagraphSeparators | QTextOption::ShowTabsAndSpaces|QTextOption::ShowDocumentTerminator);
		}
	this->document()->setDefaultTextOption(opts);

	QFontMetrics fm(this->mainKKEditClass->prefsDocumentFont);
	this->setTabStopDistance(fm.horizontalAdvance(" ")*this->mainKKEditClass->prefsTabWidth);
	this->fromMe=false;
}

void DocumentClass::setHiliteLanguage(void)
{
	for(int j=0;j<this->highlighter->langPlugins.count();j++)
		{
			if(this->highlighter->langPlugins[j].mimeType.contains(this->mimeType)==true)
				{
					this->highlighter->setLanguage(this->highlighter->langPlugins[j].langName);
					this->highlighter->setTheme();
					return;
				}
		}
	this->highlighter->setLanguage("plaintext");
	this->highlighter->setTheme();
}

void DocumentClass::setUndo(bool avail)
{
	this->gotUndo=avail;
}

void DocumentClass::setRedo(bool avail)
{
	this->gotRedo=avail;
}

void DocumentClass::paintEvent(QPaintEvent* event)
{
	QPlainTextEdit::paintEvent(event);
	if(this->inDrag==true)
		{
			QPainter p(viewport());
			QRect r(this->cursorRect());
			r.setWidth(r.width()+2);
			r.translate(-1,0);
			p.fillRect(r,this->mainKKEditClass->theme->themeParts["insertcolour"].colour);
		}
}

void DocumentClass::dragEnterEvent(QDragEnterEvent* event)
{
	this->inDrag=true;

	if (event->mimeData()->hasUrls())
		{
			event->accept();
		}
    else
    		{
			this->holdCursor=this->textCursor();
			QPlainTextEdit::dragEnterEvent(event);
		}
}

void DocumentClass::dragLeaveEvent(QDragLeaveEvent* event)
{
	this->inDrag=false;
	QPlainTextEdit::dragLeaveEvent(event);
}

void DocumentClass::dragMoveEvent(QDragMoveEvent *event)
{
	QTextCursor	cursor;

	if((event->mimeData()->hasUrls()==true) || (event->mimeData()->hasText()==true))
		{
			event->accept();
			cursor=this->cursorForPosition(event->pos());
			this->setTextCursor(cursor);
		}
	else
		QPlainTextEdit::dragMoveEvent(event);
}

void DocumentClass::dropEvent(QDropEvent* event)
{
	this->inDrag=false;

	if (event->mimeData()->hasUrls())
		{
			const QMimeData	*mime=event->mimeData();
			QList<QUrl>		list=mime->urls();
			if(list.isEmpty()==false)
				{
					bool				retval=false;
					QFile			file(list.at(0).toLocalFile());
					QFileInfo		fileinfo(file);
					const QSignalBlocker	blocker(sender());

					retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
					if(retval==true)
						{
							QString			content=QString::fromUtf8(file.readAll());
							QMimeDatabase	db;
							QTextCursor		cursor;
							cursor=this->cursorForPosition(event->pos());
							cursor.beginEditBlock();
								cursor.insertText(content);
								this->setFilePrefs();
								this->highlighter->rehighlight();
								this->dirty=true;
							cursor.endEditBlock();
							file.close();
						}
					this->state=DIRTYTAB;
					this->setTabColourType(DIRTYTAB);
					this->mainKKEditClass->switchPage(this->mainKKEditClass->mainNotebook->currentIndex());
					this->mainKKEditClass->setToolbarSensitive();
					event->accept();
				}
			return;
		}

	this->setTextCursor(this->holdCursor);
	QPlainTextEdit::dropEvent(event);
}

void DocumentClass::setBMFromLineBar(QMouseEvent *event)
{
	QPoint		pos(event->x(),event->y());
	QTextCursor	cursor=this->cursorForPosition(pos);

	this->mainKKEditClass->handleBMMenu(this,TOGGLEBOOKMARKMENUITEM,cursor);
	this->highlightCurrentLine();
}

void DocumentClass::mouseReleaseEvent(QMouseEvent *event)
{
	QTextCursor thiscursor=this->textCursor();

	this->searchPos=this->textCursor().position();
	this->mainKKEditClass->currentTab=this->mainKKEditClass->mainNotebook->currentIndex();
	QPlainTextEdit::mouseReleaseEvent(event);
}

void DocumentClass::mouseDoubleClickEvent(QMouseEvent *event)
{
	QTextCursor	cursor=this->textCursor();
	int			pos=cursor.position();

	QPlainTextEdit::mouseDoubleClickEvent(event);

	if(pos>=this->toPlainText().length())
		pos=this->toPlainText().length()-1;

	if((this->toPlainText().at(pos)==" ") || (this->toPlainText().at(pos)=="\t"))
		{
			if(cursor.atBlockStart()==false)
				{
					cursor.movePosition(QTextCursor::PreviousWord,QTextCursor::MoveAnchor);
					cursor.movePosition(QTextCursor::EndOfWord,QTextCursor::MoveAnchor);
				}
			cursor.movePosition(QTextCursor::NextWord,QTextCursor::KeepAnchor);
			this->setTextCursor(cursor);
		}
}

void DocumentClass::refreshFromDisk(void)
{
	QTextBlock	block;
	QTextCursor	cursor;
	bool			retval;
	QFile		file(this->getFilePath());
	int			currentline=this->getCurrentLineNumber();

	this->mainKKEditClass->sessionBusy=true;

	retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
	if(retval==true)
		{
			QString	content=QString::fromUtf8(file.readAll());
			cursor=this->textCursor();
			cursor.beginEditBlock();
				cursor.select(QTextCursor::Document);
				cursor.removeSelectedText ();
				cursor.insertText(content);

				this->highlighter->rehighlight();
				this->dirty=false;
				file.close();

				block=this->document()->findBlockByNumber(currentline-1);

				if(block.isValid()==false)
					block=this->document()->lastBlock();
				cursor=this->textCursor();
				cursor.setPosition(block.position());
				this->setTextCursor(cursor);
				this->fromMe=false;
				this->modifiedOnDisk=false;
				this->state=NORMALTAB;
				this->dirty=false;
				this->setTabColourType(NORMALTAB);
			cursor.endEditBlock();
		}

	this->mainKKEditClass->sessionBusy=false;
	this->mainKKEditClass->setToolbarSensitive();
}

bool DocumentClass::findStr(int what)
{
	QString							doctext;
	QRegularExpression				pattern;
	QTextCharFormat					format;
	QRegularExpressionMatchIterator	it;
	QRegularExpressionMatch			match;
	QTextCursor						thiscursor;

	this->mainKKEditClass->setSearchPrefs();

//	if(this->mainKKEditClass->searchBack!=this->changeDirection)
//		{
//			this->changeDirection=this->mainKKEditClass->searchBack;
//			this->findStr(FINDNEXT);
//		}

	doctext=this->toPlainText();

	if(this->mainKKEditClass->useRegex==true)
		pattern.setPattern(this->mainKKEditClass->correctedFind);
	else
		pattern.setPattern(QRegularExpression::escape(this->mainKKEditClass->correctedFind));

	if(this->mainKKEditClass->insensitiveSearch==true)
		pattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

	format.setFontItalic(false);
	format.setFontWeight(0);
	format.setForeground(QColor(this->highlighter->findFGColour));
	format.setBackground(QColor(this->highlighter->findBGColour));

	if((this->mainKKEditClass->replaceAll==true) && (what==FINDREPLACE))
		{
			this->totalMatches=doctext.count(pattern);
			doctext.replace(pattern,this->mainKKEditClass->correctedReplace);
			thiscursor=this->textCursor();
			thiscursor.beginEditBlock();
				thiscursor.select(QTextCursor::Document);
				thiscursor.removeSelectedText();
				thiscursor.insertText(doctext);
			thiscursor.endEditBlock();
			this->mainKKEditClass->statusText->setText(QString("Replaced %1 occurences ...").arg(this->totalMatches));
			return(true);
		}

	if(what==FINDREPLACE)
		{
			QString	selection;
			thiscursor=this->textCursor();
			if(thiscursor.hasSelection()==true)
				{
					QString	holdstr;
					selection=doctext.mid(thiscursor.selectionStart(),thiscursor.selectionEnd()-thiscursor.selectionStart());
					holdstr=selection;
					selection.replace(pattern,this->mainKKEditClass->correctedReplace);
					if(holdstr.compare(selection)!=0)
						{
							thiscursor.beginEditBlock();
								thiscursor.removeSelectedText();
								thiscursor.insertText(selection);
							thiscursor.endEditBlock();
							doctext=this->toPlainText();
						}
					if(this->mainKKEditClass->findAfterReplace==false)
						return(true);
				}
		}

	if(this->mainKKEditClass->searchBack==false)
		{
			it=pattern.globalMatch(&doctext,this->searchPos);
			if(it.hasNext())
				{
					match=it.next();
					thiscursor=this->textCursor();
					thiscursor.setPosition(match.capturedStart());
					thiscursor.setPosition(match.capturedLength()+match.capturedStart(),QTextCursor::KeepAnchor);
					this->setTextCursor(thiscursor);
					this->searchPos=match.capturedStart()+match.capturedLength();
					return(true);
				}
			else
				{
					if(this->mainKKEditClass->findInAllFiles==true)
						{
							this->finishedSearch=true;
							return(false);
						}
					if((this->mainKKEditClass->wrapSearch==true) && (this->mainKKEditClass->findInAllFiles==false))
						{
							this->searchPos=0;
							it=pattern.globalMatch(&doctext,this->searchPos);
							if(it.hasNext())
								{
									match=it.next();
									thiscursor=this->textCursor();
									thiscursor.setPosition(match.capturedStart());
									thiscursor.setPosition(match.capturedLength()+match.capturedStart(),QTextCursor::KeepAnchor);
									this->setTextCursor(thiscursor);
									this->searchPos=match.capturedStart()+match.capturedLength();
									return(true);
								}
						}
					else
						{
							if(this->mainKKEditClass->findInAllFiles==true)
								{
									this->finishedSearch=true;
									return(false);
								}
						}
				}
		}
	else
		{
			int retint=doctext.lastIndexOf(pattern,this->searchPos,&match);
			if(retint!=-1)
				{
					this->searchPos=retint-1;
					this->finishedSearch=false;
				}
			else
				{
					if((this->mainKKEditClass->wrapSearch==true) && (this->mainKKEditClass->findInAllFiles==false))
						{
							this->searchPos=doctext.length()-1;
							this->findStr(FINDNEXT);
						}
					if(this->mainKKEditClass->findInAllFiles==true)
						{
							this->finishedSearch=true;
							return(false);
						}
				}

			if(match.capturedStart()>0)
				{				
					thiscursor=this->textCursor();
					thiscursor.setPosition(match.capturedStart());
					thiscursor.setPosition(match.capturedLength()+match.capturedStart(),QTextCursor::KeepAnchor);
					this->setTextCursor(thiscursor);
					return(true);
				}
		}
	return(false);
}

void DocumentClass::setHighlightAll(void)
{
	QString							doctext;
	QRegularExpression				pattern;
	QTextCharFormat					format;
	QRegularExpressionMatchIterator	it;
	QRegularExpressionMatch			match;

	this->mainKKEditClass->setSearchPrefs();

	doctext=this->toPlainText();

	if(this->mainKKEditClass->useRegex==true)
		pattern.setPattern(this->mainKKEditClass->correctedFind);
	else
		pattern.setPattern(QRegularExpression::escape(this->mainKKEditClass->correctedFind));

	if(this->mainKKEditClass->insensitiveSearch==true)
		pattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

	format.setFontItalic(false);
	format.setFontWeight(0);
	format.setForeground(QColor(this->highlighter->findFGColour));
	format.setBackground(QColor(this->highlighter->findBGColour));

	this->findMatch.clear();
		
	it=pattern.globalMatch(&doctext);
	while(it.hasNext())
		{
			match=it.next();
				{
					QTextEdit::ExtraSelection	findmatch;
					findmatch.cursor=this->textCursor();
					findmatch.cursor.setPosition(match.capturedStart());
					findmatch.cursor.setPosition(match.capturedLength()+match.capturedStart(),QTextCursor::KeepAnchor);
					findmatch.format=format;
					this->findMatch.append(findmatch);
				}
		}
	this->setXtraSelections();
}

void DocumentClass::setTabColourType(int type)
{
	QIcon tabicon;
	switch(type)
		{
			case CHANGEDONDISKTAB:
				tabicon=QIcon(QString("%1/pixmaps/changed.png").arg(DATADIR));
				break;
			case IGNORECHANGEDONDISKTAB:
				tabicon=QIcon(QString("%1/pixmaps/ignore.png").arg(DATADIR));
				break;
			case LOCKEDTAB:
				tabicon=QIcon(QString("%1/pixmaps/locked.png").arg(DATADIR));
				break;
			case DIRTYTAB:
				tabicon=QIcon(QString("%1/pixmaps/dirty.png").arg(DATADIR));
				break;
			default:
				tabicon=QIcon(QString("%1/pixmaps/empty.png").arg(DATADIR));
				break;
		}

	this->mainKKEditClass->mainNotebook->setTabIcon(this->mainKKEditClass->mainNotebook->indexOf(this),tabicon);
	this->mainKKEditClass->mainNotebook->tabBar()->update();
}
