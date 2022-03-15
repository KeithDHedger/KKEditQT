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
#ifdef _USEPLUGINS_
	painter.fillRect(event->rect(),this->highlighter->lineNumbersBackground);
#else
	painter.fillRect(event->rect(),Qt::lightGray);
#endif
	QTextBlock block=this->firstVisibleBlock();
	int blockNumber=block.blockNumber();
	int top=(int) blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom=top+(int) blockBoundingRect(block).height();

	while(block.isValid() && top<=event->rect().bottom())
		{
			if(block.isVisible() && bottom>=event->rect().top())
				{
#ifdef _USEPLUGINS_
					painter.setPen(this->highlighter->lineNumbersForeground);
#else
					painter.setPen(Qt::black);
#endif
					foreach(bookMarkStruct value,this->mainKKEditClass->bookMarks)
						{
							if((value.docIndex==this->pageIndex) && (value.line==blockNumber+1))
								{
#ifdef _USEPLUGINS_
									painter.fillRect(QRect(0,top,lineNumberArea->width(),fontMetrics().height()),this->highlighter->bookMarkBGColour);
#else
									painter.fillRect(QRect(0,top,lineNumberArea->width(),fontMetrics().height()),Qt::darkGray);
#endif
								}
						}
#ifdef _USEPLUGINS_
					QString number=QString::number(blockNumber+1);painter.setPen(this->highlighter->lineNumbersForeground);
#else
					QString number=QString::number(blockNumber+1);painter.setPen(Qt::black);
#endif
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

void DocumentClass::updateLineNumberAreaWidth(void)
{
	if(this->realShowLineNumbers()==true)
		setViewportMargins(lineNumberAreaWidth(),0,0,0);
	else
		setViewportMargins(0,0,0,0);
}

void DocumentClass::modified()
{
	if((this->mainKKEditClass->sessionBusy==true) || (this->dirty==true))
		return;

	this->dirty=true;
	this->setTabName(this->getTabName()+"*");
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
	bool						holdsb=this->mainKKEditClass->sessionBusy;
	QTextCursor	cursor=this->textCursor();

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
		updateLineNumberAreaWidth();
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

void DocumentClass::keyPressEvent(QKeyEvent *event)
{
	bool			isshortcut;
	bool			ctrlorshift;
	bool			hasmodifier;
	QRect			cr;
	QString			completionPrefix;
	const QString	eow("~!@#$%^&*()+{}|:\"<>?,./;'[]\\-="); // end of word;

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
			QTextCursor cursor=this->textCursor();
			QTextBlock tb=cursor.block();
			this->indentPad="";
			QString data=tb.text();
			while((j<data.length()) && ((data.at(j)=='\t') || (data.at(j)==' ')))
				{
						this->indentPad+=data.at(j++);
				}
		}

	if(this->mainKKEditClass->completer && this->mainKKEditClass->completer->popup()->isVisible())
		{
        // The following keys are forwarded by the completer to the widget
			switch (event->key())
				{
					case Qt::Key_Enter:
					case Qt::Key_Return:
					case Qt::Key_Escape:
					case Qt::Key_Tab:
					case Qt::Key_Backtab:
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

	isshortcut=((event->modifiers() & Qt::ControlModifier) && event->key()== Qt::Key_E); // CTRL+E//TODO//
	if (!this->mainKKEditClass->completer || !isshortcut) // do not process the shortcut when we have a completer
		QPlainTextEdit::keyPressEvent(event);

	ctrlorshift=event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!this->mainKKEditClass->completer || (ctrlorshift && event->text().isEmpty()))
        return;

	hasmodifier=(event->modifiers() != Qt::NoModifier) && !ctrlorshift;
	completionPrefix=this->textUnderCursor();

    if(!isshortcut && (hasmodifier || event->text().isEmpty()|| completionPrefix.length() < this->mainKKEditClass->autoShowMinChars || eow.contains(event->text().right(1))))
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
	if((event->key()==Qt::Key_Return)&& (this->mainKKEditClass->prefsIndent==true))
		{
			QTextCursor cursor=this->textCursor();
			this->insertPlainText(this->indentPad);
		}
	//this->dirty=true;
	this->mainKKEditClass->setToolbarSensitive();
	QPlainTextEdit::keyReleaseEvent(event);
}

DocumentClass::DocumentClass(KKEditClass *kk,QWidget *parent): QPlainTextEdit(parent)
{
	this->doneHighlightAll=false;
	this->lastFind=NULL;
	this->mainKKEditClass=kk;
	this->setAcceptDrops(true);

#ifdef _USEPLUGINS_
	this->highlighter=new Highlighter(this->document(),this);
#else
	this->highlighter=new QSourceHighlite::QSourceHighliter(this->document());
#endif
	this->setCenterOnScroll(true);
	lineNumberArea=new LineNumberArea(this);

	connect(this,SIGNAL(blockCountChanged(int)),this,SLOT(updateLineNumberAreaWidth()));
	connect(this,SIGNAL(updateRequest(QRect,int)),this,SLOT(updateLineNumberArea(QRect,int)));
	connect(this,SIGNAL(cursorPositionChanged()),this,SLOT(highlightCurrentLine()));
	connect(this,SIGNAL(textChanged()),this,SLOT(modified()));

	connect(this,SIGNAL(undoAvailable(bool)),this,SLOT(setUndo(bool)));
	connect(this,SIGNAL(redoAvailable(bool)),this,SLOT(setRedo(bool)));

	updateLineNumberAreaWidth();
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
	for(int j=0;j<this->mainKKEditClass->plugins.count();j++)
		{
			if((this->mainKKEditClass->plugins[j].loaded) && ((this->mainKKEditClass->plugins[j].wants & DOCONTEXTMENU)==DOCONTEXTMENU))
				this->mainKKEditClass->plugins[j].instance->plugAddToContextMenu(&menu,this);
		}

	menu.setStyleSheet("QMenu { menu-scrollable: true ;}");
	menu.exec(event->globalPos());
}

void DocumentClass::setFilePrefs(void)
{
	bool						holddirty=this->dirty;
	QTextOption					opts;
	this->setTabStopDistance(1.0);
	this->dirty=true;

#ifdef _USEPLUGINS_
	this->highlighter->setTheme(this->mainKKEditClass->prefStyleName);
	//this->highlighter->setTheme("default");
	this->setStyleSheet(this->highlighter->docbackground);
#endif

	this->dirty=holddirty;
	this->updateLineNumberAreaWidth();
	
	this->setFont(this->mainKKEditClass->prefsDocumentFont);
	this->setLineWrapMode(static_cast<QPlainTextEdit::LineWrapMode>(this->mainKKEditClass->wrapLine));
	this->prefsHiLiteLineColor=this->mainKKEditClass->prefsHiLiteLineColor;
	this->bookmarkLineColor=this->mainKKEditClass->prefsBookmarkHiLiteColor;
	this->highlightCurrentLine();

	if(this->realSyntaxHighlighting()==false)
		opts.setFlags(QTextOption::SuppressColors);

	if(this->mainKKEditClass->showWhiteSpace==true)
		opts.setFlags(opts.flags()|QTextOption::ShowLineAndParagraphSeparators | QTextOption::ShowTabsAndSpaces|QTextOption::ShowDocumentTerminator);

	this->document()->setDefaultTextOption(opts);

	QFontMetrics fm(this->mainKKEditClass->prefsDocumentFont);
	this->setTabStopDistance(fm.horizontalAdvance(" ")*this->mainKKEditClass->prefsTabWidth);
}

/*
Mime type: "text/x-c++src"
Mime type: "application/x-shellscript"
Mime type: "text/x-csrc"
Mime type: "text/x-c++src"
Mime type: "text/x-c++src"
Mime type: "text/x-c++src"
Mime type: "text/plain"
text/x-c++hdr
text/x-chdr
*/
void DocumentClass::setHiliteLanguage(void)
{
bool retval=false;
#ifdef _USEPLUGINS_
	for(int j=0;j<this->highlighter->langPlugins.count();j++)
		{
			if(this->highlighter->langPlugins[j].mimeType.contains(this->mimeType)==true)
				{
					retval=this->highlighter->setLanguage(this->highlighter->langPlugins[j].langName);
					this->highlighter->setTheme(this->mainKKEditClass->prefStyleName);
					return;
				}
		}
	retval=this->highlighter->setLanguage("plaintext");
	this->highlighter->setTheme(this->mainKKEditClass->prefStyleName);
#else
	QSourceHighliter::Themes	theme=(QSourceHighliter::Themes)2;

	if(this->mainKKEditClass->prefsSyntaxHilighting==false)
		{
			theme=(QSourceHighliter::Themes)-1;
			return;
		}
	if(this->mimeType.compare("text/plain",Qt::CaseInsensitive)==0)
		theme=(QSourceHighliter::Themes)-1;
	if(this->mimeType.compare("text/x-c++src",Qt::CaseInsensitive)==0)
		this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeCpp);
	else if(this->mimeType.compare("text/x-c++hdr",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeCpp);
	else if(this->mimeType.compare("text/x-csrc",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeC);
	else if(this->mimeType.compare("text/x-chdr",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeC);
	else if(this->mimeType.compare("application/x-shellscript",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeBash);
	else if(this->mimeType.compare("text/x-python",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodePython);
	else if(this->mimeType.compare("text/x-go",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeGo);
	else if(this->mimeType.compare("text/x-lua",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeLua);
	else if(this->mimeType.compare("application/x-yaml",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeYAML);
	else if(this->mimeType.compare("application/x-php",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodePHP);
	else if(this->mimeType.compare("application/xhtml+xml",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeXML);
	else if(this->mimeType.compare("text/css",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeCSS);
	else if(this->mimeType.compare("application/javascript",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeJs);
	else if(this->mimeType.compare("text/x-makefile",Qt::CaseInsensitive)==0)
			this->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeMake);
	else
		theme=(QSourceHighliter::Themes)-1;

	this->highlighter->setTheme(theme);
#endif
}

void DocumentClass::setUndo(bool avail)
{
	this->gotUndo=avail;
}

void DocumentClass::setRedo(bool avail)
{
	this->gotRedo=avail;
}

void DocumentClass::dragMoveEvent(QDragMoveEvent *event)
{
	if((event->mimeData()->hasUrls()==true))
		event->accept();
	else
		QPlainTextEdit::dragMoveEvent(event);
}

void DocumentClass::dragEnterEvent(QDragEnterEvent* event)
{
	if((event->mimeData()->hasUrls()==true))
		event->accept();
    else
		QPlainTextEdit::dragEnterEvent(event);
}

void DocumentClass::dropEvent(QDropEvent* event)
{ 
	if (event->mimeData()->hasUrls())
		{
			const QMimeData	*mime=event->mimeData();
			QList<QUrl>		list=mime->urls();
			if(list.isEmpty()==false)
				{
					bool			retval=false;
					QFile			file(list.at(0).toLocalFile());
					QFileInfo		fileinfo(file);
					int				tabnum;
					const QSignalBlocker	blocker(sender());

					retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
					if(retval==true)
						{
							QString			content=QString::fromUtf8(file.readAll());
							QMimeDatabase	db;
							QMimeType		type=db.mimeTypeForFile(fileinfo.canonicalFilePath());
							this->mimeType=type.name();
							this->setPlainText(content);
							this->setFilePrefs();
//#ifndef _USEPLUGINS_
//			//doc->highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeCpp);
////			QSourceHighliter::Themes theme=(QSourceHighliter::Themes)-1;
////			if(this->prefsSyntaxHilighting==true)
////				theme=(QSourceHighliter::Themes)2;//TODO//get theme from file
////
////			doc->highlighter->setTheme(theme);
//#endif
							this->highlighter->rehighlight();
							this->dirty=true;
							file.close();
						}
					this->mainKKEditClass->switchPage(tabnum);
					this->mainKKEditClass->rebuildTabsMenu();
					this->mainKKEditClass->setToolbarSensitive();
					event->accept();
				}
			return;
		}

	QPlainTextEdit::dropEvent(event);
}

void DocumentClass::setBMFromLineBar(QMouseEvent *event)
{
	QPoint		pos(event->x(),event->y());
	QTextCursor	cursor=this->cursorForPosition(pos);

	this->mainKKEditClass->handleBMMenu(this,TOGGLEBOOKMARKMENUITEM,cursor);
	this->highlightCurrentLine();
}
