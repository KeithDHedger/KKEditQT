/*
 *
 * ©K. D. Hedger. Sat 18 Jul 20:11:36 BST 2026 keithdhedger@gmail.com

 * This file (QT_SpellCheck.cpp) is part of KKEditQT.

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
#include "QT_SpellCheck.h"

QT_SpellCheckClass::~QT_SpellCheckClass()
{
	delete_aspell_speller(this->spellChecker);
	delete_aspell_config(this->aspellConfig);
}

void QT_SpellCheckClass::setLanguage(QString lang)
{
	AspellCanHaveError	*possible_err;

	aspell_config_replace(this->aspellConfig,"lang",qPrintable(lang));
	possible_err=new_aspell_speller(this->aspellConfig);
	if(this->spellChecker!=NULL)
		delete_aspell_speller(this->spellChecker);
	this->spellChecker=NULL;

	if(aspell_error_number(possible_err)!= 0)
		puts(aspell_error_message(possible_err));
	else
		this->spellChecker=to_aspell_speller(possible_err);
}

QStringList	QT_SpellCheckClass::getLanguageCodes(void)
{
	QStringList					ll;
	AspellDictInfoList			*dlist;
	AspellDictInfoEnumeration	*dels;
	const AspellDictInfo			*entry;

	dlist=get_aspell_dict_info_list(this->aspellConfig);
	dels=aspell_dict_info_list_elements(dlist);

	while((entry=aspell_dict_info_enumeration_next(dels))!=0) 
		ll<<entry->code;
	ll.removeDuplicates();
	delete_aspell_dict_info_enumeration(dels);
	return(ll);
}
	
void QT_SpellCheckClass::doChangeWord(void)
{
	QTextCursor cursor=this->te->textCursor();

	cursor.beginEditBlock();
	this->goodWord=wordListDropbox->currentText();
	cursor.setPosition(this->badwordstart);
	cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,this->badwordlen);
	cursor.removeSelectedText();
	cursor.insertText(this->goodWord);
	this->blockFlag=false;
	cursor.endEditBlock();
	this->changed=true;
}

void QT_SpellCheckClass::doSpellCheckWord(QString word)
{
	this->changed=false;

	if(this->te==NULL || this->te->textCursor().hasSelection()==false)
		return;

	this->spellCheckWord->setWindowTitle("Check Word");
	this->badwordstart=this->te->textCursor().selectionStart();
	this->badwordlen=this->te->textCursor().selectedText().length();
	this->blockFlag=true;
	this->checkTheWord(word);
	this->blockFlag=false;
	this->spellCheckWord->hide();
}

bool QT_SpellCheckClass::checkTheWord(QString word)
{
	int						correct;
	AspellWordList			*suggestions=NULL;
	AspellStringEnumeration	*elements=NULL;
	const char				*suggestedword=NULL;
	int						wordcnt=0;
	bool						retval=false;

	wordListDropbox->clear();
	correct=aspell_speller_check(spellChecker,qPrintable(word),-1);
	if(correct==0)
		{
			this->badWord=word;
			this->cancelCheck=false;
			this->badWordLabel->setText(QString("Change <i><b>%1</b></i> to: ").arg(this->badWord));
			suggestions=(AspellWordList*)aspell_speller_suggest(spellChecker,qPrintable(word),-1);
			if(suggestions==NULL)
				return(false);
			elements=aspell_word_list_elements(suggestions);
			if(elements==NULL)
				return(false);
			while((suggestedword=aspell_string_enumeration_next(elements))!=NULL)
				{
					this->wordListDropbox->addItem(suggestedword);
					wordcnt++;
					suggestedword=NULL;
				}

			if(wordcnt==0)
				return(false);

			retval=true;
			delete_aspell_string_enumeration(elements);
			spellCheckWord->show();
			while(this->blockFlag==true)
				qApp->processEvents();
		}
	return(retval);
}

void QT_SpellCheckClass::doSpellCheckDoc(void)
{
	QList<QTextEdit::ExtraSelection>	extraSelections;
	QTextCharFormat					format;
	QTextEdit::ExtraSelection		selection;
	QTextCursor						mc;
	QTextCursor						cursor;
	QString							xbadword	;
	QString							line;
	QSettings						defaults("KDHedger",PACKAGE_NAME);
	AspellCanHaveError				*ret;
	AspellDocumentChecker			*checker;
	AspellToken						token;
	int								diff;
	unsigned int						goodwordlen;
	int								buffdiff=0;

	this->changed=false;
	/* Set up the document checker */
	if((this->spellChecker==NULL) || (this->te==NULL))
		return;
	ret=new_aspell_document_checker(this->spellChecker);
	if (aspell_error(ret)!=0)
		{
			printf("Error: %s\n",aspell_error_message(ret));
			return;
		}

	if(this->te->toPlainText().isEmpty()==true)
		return;

	this->spellCheckWord->setWindowTitle("Check Text ...");
	line=this->te->toPlainText();
	checker=to_aspell_document_checker(ret);
	  /* First process the line */
	aspell_document_checker_process(checker,line.toLatin1().constData(),-1);
	diff=0;
	  /* Now find the misspellings in the line */
	
	while(token=aspell_document_checker_next_misspelling(checker),token.len!=0)
		{
			if(this->spellCheckWord->isHidden()==true)
				{
					this->spellCheckWord->restoreGeometry(defaults.value("spell/spellgeometry").toByteArray());
					this->spellCheckWord->show();
				}
			line=this->te->toPlainText();
			/* Pay particular attention to how token.offset and diff is used */
			xbadword=line.mid(token.offset+diff,token.len);
			goodWord.clear();
			cursor=this->te->textCursor();
			cursor.setPosition(token.offset-buffdiff,QTextCursor::MoveAnchor);
			te->setTextCursor(cursor);
			te->ensureCursorVisible();
	
			extraSelections.clear();
			format.setBackground(QColor(this->badwordHiliteColour)); // Set highlight color
			selection.format=format;
			selection.cursor=te->textCursor();
			selection.cursor.clearSelection();
			selection.cursor.setPosition(token.offset-buffdiff,QTextCursor::MoveAnchor);
			selection.cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor,xbadword.length());

			extraSelections.append(selection);
			// Apply the highlight
			te->setExtraSelections(extraSelections);

			this->blockFlag=true;
			this->badwordstart=token.offset-buffdiff;
			this->badwordlen=xbadword.length();
			checkTheWord(xbadword);

			if(cancelCheck==true)
				{
					delete_aspell_document_checker(checker);
					extraSelections.clear();
					this->te->setExtraSelections(extraSelections);
					return;
				}

			if(goodWord.isEmpty()==false)
				{
					goodwordlen=goodWord.length();
					diff+=goodwordlen-token.len;
					buffdiff+=badWord.length()-goodWord.length();
				}
		}

	if(this->spellCheckWord->isHidden()==false)
		this->spellCheckWord->hide();

	delete_aspell_document_checker(checker);
	extraSelections.clear();
	this->te->setExtraSelections(extraSelections);
	defaults.setValue("spell/spellgeometry",this->spellCheckWord->saveGeometry());
}

void QT_SpellCheckClass::buildWordCheckDialog(void)
{
	QVBoxLayout	*vlayout=new QVBoxLayout;
	QWidget		*hbox;
	QHBoxLayout	*hlayout;
	QPushButton	*button;
	QSettings	prefs("KDHedger",PACKAGE_NAME);

	this->spellCheckWord=new QDialog(this->mainWindow);

	hlayout=new QHBoxLayout;
	hbox=new QWidget(spellCheckWord);
	hbox->setLayout(hlayout);

	badWordLabel=new QLabel(QString("Change <i><b>%1</b></i> to: ").arg(badWord),spellCheckWord);
	hlayout->addWidget(badWordLabel);
	this->wordListDropbox=new QComboBox(spellCheckWord);
	this->wordListDropbox->setEditable(true);
	hlayout->addWidget(wordListDropbox,2);
	vlayout->addWidget(hbox);

//buttons
	hlayout=new QHBoxLayout;
	hbox=new QWidget(spellCheckWord);
	hbox->setLayout(hlayout);

	button=new QPushButton("&Apply",this->spellCheckWord);
	button->setIcon(QIcon::fromTheme("dialog-ok"));
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			if(this->te!=NULL)
				this->doChangeWord();
		});
	hlayout->addWidget(button);

	button=new QPushButton("&Ignore",this->spellCheckWord);
	button->setIcon(QIcon::fromTheme("list-remove"));
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			aspell_speller_add_to_session(this->spellChecker,qPrintable(this->badWord),-1);
			this->badWord.clear();
			this->blockFlag=false;
		});
	hlayout->addWidget(button);

	button=new QPushButton("A&dd",this->spellCheckWord);
	button->setIcon(QIcon::fromTheme("list-add"));
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			aspell_speller_add_to_personal(this->spellChecker,qPrintable(this->badWord),-1);
			aspell_speller_save_all_word_lists(this->spellChecker);
			this->badWord.clear();
			this->blockFlag=false;
		});
	hlayout->addWidget(button);

	button=new QPushButton("&Close",this->spellCheckWord);
	button->setIcon(QIcon::fromTheme("dialog-cancel"));
	QObject::connect(button,&QPushButton::clicked,[this]()
		{
			QSettings	prefs("KDHedger",PACKAGE_NAME);
			prefs.setValue("spell/spellgeometry",this->spellCheckWord->saveGeometry());

			this->badWord.clear();
			this->goodWord.clear();
			this->cancelCheck=true;
			this->blockFlag=false;
			this->spellCheckWord->hide();
		});
	hlayout->addWidget(button);

	vlayout->addWidget(hbox);
	spellCheckWord->setLayout(vlayout);
	spellCheckWord->setModal(true);
	spellCheckWord->restoreGeometry(prefs.value("spell/spellgeometry").toByteArray());
}

QT_SpellCheckClass::QT_SpellCheckClass(QMainWindow *window)
{
	AspellCanHaveError	*possible_err;
	QString				lang=getenv("LANG");

	this->mainWindow=window;

	this->aspellConfig=new_aspell_config();
	if(lang.contains("."))
		lang.chop(lang.length()-lang.lastIndexOf("."));
	aspell_config_replace(this->aspellConfig,"lang",qPrintable(lang));
	
	possible_err=new_aspell_speller(this->aspellConfig);
	if(aspell_error_number(possible_err)!=0)
		{
			aspell_config_replace(this->aspellConfig,"dict-dir","/lib/aspell");
			possible_err=new_aspell_speller(this->aspellConfig);
		}

	if(aspell_error_number(possible_err)!= 0)
		{
			qDebug()<<aspell_error_message(possible_err);
			qDebug()<<"Install some dictionary's in /lib/aspell ...";
		}
	else
		this->spellChecker=to_aspell_speller(possible_err);

	this->buildWordCheckDialog();
}

AspellConfig *QT_SpellCheckClass::getConfig(void)
{
	return(this->aspellConfig);
}
#endif