/*
 *
 * ©K. D. Hedger. Thu 11 Dec 14:35:30 GMT 2025 keithdhedger@gmail.com

 * This file (QT_prefsWidgets.cpp) is part of Classes.

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

#include "QT_prefsWidgets.h"

prefsWidgetsClass::~prefsWidgetsClass()
{
	delete this->window;
}

prefsWidgetsClass::prefsWidgetsClass(QString prefsname,QWidget *parent)
{
	this->theParent=parent;
	this->thePrefsName=prefsname;
}

void prefsWidgetsClass::startWindow(QString wname)
{
	this->window=new QDialog;
	this->window->setWindowTitle(wname);
	this->windowVLayout=new QVBoxLayout(this->window);
	this->window->setLayout(this->windowVLayout);
}

void prefsWidgetsClass::addWidgetToWindow(QWidget *widget)
{
	this->windowVLayout->addWidget(widget);
}

void prefsWidgetsClass::finishWindow(bool addclear)
{
	QDialogButtonBox	*dbb;
	QDialogButtonBox::StandardButton retbutton;
	QEventLoop		ev;
	QFrame			*line=new QFrame(this->window);
	QDialogButtonBox::StandardButtons addbtns=(QDialogButtonBox::Apply|QDialogButtonBox::Discard|QDialogButtonBox::Reset);

	dbb=new  QDialogButtonBox(addbtns,Qt::Horizontal,this->window);
	if(addclear==true)
		{
			QPushButton *customButton=new QPushButton(QIcon::fromTheme("edit-delete"),"Clear Prefs File",this->window);
			dbb->addButton(customButton,QDialogButtonBox::ActionRole);
		}

	QPushButton* defbtn=dbb->button(QDialogButtonBox::Discard); 
	defbtn->setDefault(true);

	QObject::connect(dbb,&QDialogButtonBox::clicked,[this,dbb,&retbutton,&ev](QAbstractButton *button)
		{
			retbutton=dbb->standardButton(button);
			switch(retbutton)
				{
					case QDialogButtonBox::NoButton:
						if(this->deletePrefs()==true)
							this->window->close();
						break;

					case QDialogButtonBox::Apply:
						this->setAllColourPrefs();
						this->setAllCheckPrefs();
						this->setAllComboPrefs();
						this->setAllFontPrefs();
						this->setAllEditPrefs();
						this->setAllSpinPrefs();
						this->setAllFilePrefs();
						this->window->close();
						break;
					case QDialogButtonBox::Reset:
						{
							QSettings	plugprefs("KDHedger",this->thePrefsName);

							for(int j=0;j<this->widgets.size();j++)
								{
									switch(this->widgets.at(j).what)
										{
											case EDITWIDGET:
											case COLOURWIDGET:
											qobject_cast<QLineEdit*>(this->widgets.at(j).widget)->setText(plugprefs.value(this->normalizeLabel(this->widgets.at(j).name)).toString());
												qobject_cast<QLineEdit*>(this->widgets.at(j).widget)->setCursorPosition(0);
												break;

											case CHECKWIDGET:
												qobject_cast<QCheckBox*>(this->widgets.at(j).widget)->setChecked(plugprefs.value(this->normalizeLabel(this->widgets.at(j).name)).toBool());
												break;

											case COMBOWIDGET:
												{
													QComboBox *combo=qobject_cast<QComboBox*>(this->widgets.at(j).widget);
													combo->setCurrentText(plugprefs.value(this->normalizeLabel(this->widgets.at(j).name)).toString());
													this->combos[this->widgets.at(j).name]=combo->currentText();
												}
												break;

											case FONTWIDGET:
												{
													QLineEdit *editbox=qobject_cast<QLineEdit*>(this->widgets.at(j).widget);
													QFont font;
													editbox->setText(plugprefs.value(this->normalizeLabel(this->widgets.at(j).name)).toString());
													font.fromString(editbox->text());
													editbox->setFont(font.family());
													editbox->setCursorPosition(0);
													this->fonts[this->widgets.at(j).name]=editbox->text();
												}
												break;

											case SPINWIDGET:
												{
													QSpinBox *spin=qobject_cast<QSpinBox*>(this->widgets.at(j).widget);
													this->spins[this->widgets.at(j).name]=plugprefs.value(this->normalizeLabel(this->widgets.at(j).name)).toInt();
													spin->setValue(this->spins[this->widgets.at(j).name]);
												}
												break;
											default:
												break;
										}
								}
						}
						break;
					default:
						this->window->close();
						break;
				}
		});

	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);

	this->addWidgetToWindow(line);
	this->addWidgetToWindow(dbb);

	this->window->exec();
}

bool prefsWidgetsClass::deletePrefs(void)
{
	QSettings	plugprefs("KDHedger",this->thePrefsName);
	bool			retval=false;

	QMessageBox::StandardButton ret=QMessageBox::warning(this->theParent,"Delete Prefs",QString("Are you sure you want to delete prefs for \"%1\"?").arg(this->thePrefsName),(QMessageBox::Yes | QMessageBox::No),QMessageBox::No);

	if(ret==QMessageBox::Yes)
		{
			plugprefs.clear();
			retval=true;
		}
	return(retval);
}

QString prefsWidgetsClass::bestFontColour(QString colour)
{
	QColor	cc(colour);
    int		r=0,g=0,b=0;

	r=cc.red();
	g=cc.green();
	b=cc.blue();

	if((r+r+r+b+g+g+g+g)>>3>128)
		return("black");
	else
		return("white");
}

QString prefsWidgetsClass::normalizeLabel(QString label)
{
	QString retlabel=label;

	retlabel=retlabel.remove(' ');
	retlabel=retlabel.toLower();
	return(retlabel);
}

//colour
QWidget* prefsWidgetsClass::getColourWidget(QString label,QString colour)
{
	QHBoxLayout	*hlayout;
	QWidget		*newwidg=new QWidget(this->theParent);
	QLabel		*lab;
	QPushButton	*button;
	QLineEdit	*editbox;
	QString		bgcolstr;

	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	newwidg->setLayout(hlayout);

	lab=new QLabel(label,newwidg);	
	lab->setFixedWidth(this->labelWidth);
	
	hlayout->addWidget(lab,0,Qt::AlignLeft);
	button=new QPushButton(QIcon::fromTheme("preferences-desktop-theme"),"",newwidg);	
	hlayout->addWidget(button,0);

	editbox=new QLineEdit(colour,newwidg);
	this->widgets.push_back({COLOURWIDGET,editbox,label});

	hlayout->addWidget(editbox,1);	

	this->colours[label]=colour;
	bgcolstr=QString("background: %1;\ncolor: %2;").arg(colour).arg(this->bestFontColour(colour));
	editbox->setStyleSheet(bgcolstr);//ding ding ding
	QObject::connect(editbox,&QLineEdit::textChanged,[this,label,editbox](const QString &text)
		{
			QString bgcolstr;
			this->colours[label]=text;
			bgcolstr=QString("background: %1;\ncolor: %2;").arg(text).arg(this->bestFontColour(text));
			editbox->setStyleSheet(bgcolstr);
		});
	QObject::connect(button,&QPushButton::clicked,[this,label,editbox]()
		{
			QColor colour;
			colour=QColorDialog::getColor(editbox->text(),nullptr,"Select Colour");
			if(colour.isValid()==true)
				{
					QString bgcolstr;
					editbox->setText(colour.name(QColor::HexRgb));
					this->colours[label]=colour.name(QColor::HexRgb);
					bgcolstr=QString("background: %1;\ncolor: %2;").arg(colour.name(QColor::HexRgb)).arg(this->bestFontColour(colour.name(QColor::HexRgb)));
					editbox->setStyleSheet(bgcolstr);
				}
		});
	return(newwidg);
}

QWidget	*prefsWidgetsClass::getPrefsColourWidget(QString label,QString defaultcolour)
{
	QWidget	*newwidg;
	newwidg=this->getPrefsWidget(label,defaultcolour,COLOURWIDGET);
	return(newwidg);
}

QString prefsWidgetsClass::getColourValue(QString label)
{
	return(this->colours[label]);
}

void prefsWidgetsClass::setColourPref(QString label)
{
	this->setPref(this->colours,label);
}

void prefsWidgetsClass::setAllColourPrefs(void)
{
	this->setAllPrefs(this->colours);
}

//check
QWidget* prefsWidgetsClass::getCheckWidget(QString label,bool state)
{
	QCheckBox	*newcheck;

	newcheck=new QCheckBox(label,this->theParent);
	this->widgets.push_back({CHECKWIDGET,newcheck,label});

	newcheck->setChecked(state);
	this->checks[label]=state;
	QObject::connect(newcheck,&QCheckBox::checkStateChanged,[this,label](Qt::CheckState state)
		{
			if(state==Qt::Unchecked)
				this->checks[label]=false;
			else
				this->checks[label]=true;
		});
	return(newcheck);
}

QWidget	*prefsWidgetsClass::getPrefsCheckWidget(QString label,bool defaultstate)
{

	QCheckBox	*newcheck;
	QSettings	plugprefs("KDHedger",this->thePrefsName);
	bool			prefchk;
	prefchk=plugprefs.value(this->normalizeLabel(label),defaultstate).toBool();
	newcheck=(QCheckBox*)this->getCheckWidget(label,prefchk);
	return(newcheck);
}

bool prefsWidgetsClass::getCheckValue(QString label)
{
	return(this->checks[label]);
}

void prefsWidgetsClass::setCheckPref(QString label)
{
	QSettings	plugprefs("KDHedger",this->thePrefsName);
	plugprefs.setValue(this->normalizeLabel(label),this->checks[label]);
}

void prefsWidgetsClass::setAllCheckPrefs(void)
{
	QSettings					plugprefs("KDHedger",this->thePrefsName);
	QHashIterator<QString,bool>	i(this->checks);
	while(i.hasNext())
		{
			i.next();
			plugprefs.setValue(this->normalizeLabel(qPrintable(i.key())),i.value());
		}
}

//combo
QWidget* prefsWidgetsClass::getComboWidget(QString label,QStringList data,QString item)
{
	QComboBox	*newcombo;
	QLabel		*lab;
	QHBoxLayout	*hlayout;
	QWidget		*newwidg=new QWidget(this->theParent);

	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	newwidg->setLayout(hlayout);

	lab=new QLabel(label,newwidg);	
	lab->setFixedWidth(this->labelWidth);
	hlayout->addWidget(lab,0,Qt::AlignLeft);

	newcombo=new QComboBox(newwidg);
	this->widgets.push_back({COMBOWIDGET,newcombo,label});

	newcombo->addItems(data);
	if(item.isEmpty()==false)
		{
			newcombo->setCurrentText(item);
			this->combos[label]=item;
		}
	else
		this->combos[label]=data.at(0);

	hlayout->addWidget(newcombo,10);
	QObject::connect(newcombo,&QComboBox::activated,[this,newcombo,label](int index)
		{
			this->combos[label]=newcombo->itemText(index);
		});
	return(newwidg);
}

QWidget	*prefsWidgetsClass::getPrefsComboWidget(QString label,QStringList data,QString item)
{
	QSettings	plugprefs("KDHedger",this->thePrefsName);
	QWidget		*newwidg;
	QString 		combodata;

	combodata=plugprefs.value(this->normalizeLabel(label),item).toString();
	newwidg=this->getComboWidget(label,data,combodata);
	return(newwidg);
}

QString prefsWidgetsClass::getComboValue(QString label)
{
	return(this->combos[label]);
}

void prefsWidgetsClass::setComboPref(QString label)
{
	this->setPref(this->combos,label);
}

void prefsWidgetsClass::setAllComboPrefs(void)
{
	this->setAllPrefs(this->combos);
}

//font
QWidget* prefsWidgetsClass::getFontWidget(QString label,QString font)
{
	QHBoxLayout	*hlayout;
	QWidget		*newwidg=new QWidget(this->theParent);
	QLabel		*lab;
	QPushButton	*button;
	QLineEdit	*editbox;

	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	newwidg->setLayout(hlayout);

	lab=new QLabel(label,newwidg);
	lab->setFixedWidth(this->labelWidth);	
	hlayout	->addWidget(lab,0,Qt::AlignLeft);

	button=new QPushButton(QIcon::fromTheme("preferences-desktop-font"),"",newwidg);	
	hlayout->addWidget(button,0);

	editbox=new QLineEdit(font,newwidg);
	this->widgets.push_back({FONTWIDGET,editbox,label});
	editbox->setCursorPosition(0);
	hlayout->addWidget(editbox,1);	

	this->fonts[label]=font;
	editbox->setFont(QFont(font).family());
	QObject::connect(button,&QPushButton::clicked,[this,label,editbox]()
		{
			QFont font;
			bool ok;
			font.fromString(editbox->text());
			font=QFontDialog::getFont(&ok,font,nullptr,"Select Font");
			if(ok==true)
				{
					editbox->setFont(font.family());
					editbox->setText(font.toString());
					editbox->setCursorPosition(0);
					this->fonts[label]=font.toString();
				}
		});
	return(newwidg);
}

QWidget	*prefsWidgetsClass::getPrefsFontWidget(QString label,QString defaultfont)
{
	QWidget	*newwidg;
	newwidg=this->getPrefsWidget(label,defaultfont,FONTWIDGET);
	return(newwidg);
}

QString prefsWidgetsClass::getFontValue(QString label)
{
	return(this->fonts[label]);
}

void prefsWidgetsClass::setFontPref(QString label)
{
	this->setPref(this->fonts,label);
}

void prefsWidgetsClass::setAllFontPrefs(void)
{
	this->setAllPrefs(this->fonts);
}

//edits
QWidget* prefsWidgetsClass::getEditWidget(QString label,QString text)
{
	QHBoxLayout	*hlayout;
	QWidget		*newwidg=new QWidget(this->theParent);
	QLabel		*lab;
	QLineEdit	*editbox;

	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	newwidg->setLayout(hlayout);

	lab=new QLabel(label,newwidg);	
	hlayout->addWidget(lab,0,Qt::AlignLeft);
	lab->setFixedWidth(this->labelWidth);

	editbox=new QLineEdit(text,newwidg);
	this->widgets.push_back({EDITWIDGET,editbox,label});
	editbox->setCursorPosition(0);
	hlayout->addWidget(editbox,0);	

	this->edits[label]=text;
	QObject::connect(editbox,&QLineEdit::textChanged,[this,label](const QString &text)
		{
			this->edits[label]=text;
		});
	return(newwidg);
}

QWidget	*prefsWidgetsClass::getPrefsEditWidget(QString label,QString defaulttext)
{
	QWidget		*newwidg;
	newwidg=this->getPrefsWidget(label,defaulttext,EDITWIDGET);
	return(newwidg);
}

QString prefsWidgetsClass::getEditValue(QString label)
{
	return(this->edits[label]);
}

void prefsWidgetsClass::setEditPref(QString label)
{
	this->setPref(this->edits,label);
}

void prefsWidgetsClass::setAllEditPrefs(void)
{
	this->setAllPrefs(this->edits);
}

//spinners
QWidget* prefsWidgetsClass::getSpinWidget(QString label,int min,int max,int val)
{
	QHBoxLayout	*hlayout;
	QWidget		*newwidg=new QWidget(this->theParent);
	QLabel		*lab;
	QSpinBox		*spinbox;

	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	newwidg->setLayout(hlayout);

	lab=new QLabel(label,newwidg);	
	lab->setFixedWidth(this->labelWidth);
	hlayout->addWidget(lab,0,Qt::AlignLeft);

	spinbox=new QSpinBox(newwidg);
	this->widgets.push_back({SPINWIDGET,spinbox,label});
	spinbox->setRange(min,max);
	spinbox->setValue(val);
	hlayout->addWidget(spinbox,1);	

	this->spins[label]=val;
	QObject::connect(spinbox,&QSpinBox::valueChanged,[this,label](int i)
		{
			this->spins[label]=i;
		});
	return(newwidg);
}

QWidget	*prefsWidgetsClass::getPrefsSpinWidget(QString label,int min,int max,int val)
{
	QWidget		*newwidg;
	int			prefspin;
	QSettings	plugprefs("KDHedger",this->thePrefsName);
	prefspin=plugprefs.value(this->normalizeLabel(label),val).toInt();
	newwidg=this->getSpinWidget(label,min,max,prefspin);
	return(newwidg);
}

int prefsWidgetsClass::getSpinValue(QString label)
{
	return(this->spins[label]);
}

void prefsWidgetsClass::setSpinPref(QString label)
{
	QSettings	plugprefs("KDHedger",this->thePrefsName);
	plugprefs.setValue(this->normalizeLabel(label),this->spins[label]);
}

void prefsWidgetsClass::setAllSpinPrefs(void)
{
	QSettings						plugprefs("KDHedger",this->thePrefsName);
	QHashIterator<QString,int>	i(this->spins);
	while(i.hasNext())
		{
			i.next();
			plugprefs.setValue(this->normalizeLabel(qPrintable(i.key())),i.value());
		}
}

//file
QWidget* prefsWidgetsClass::getFileWidget(QString label,QString path,QString openhere)
{
	QHBoxLayout	*hlayout;
	QWidget		*newwidg=new QWidget(this->theParent);
	QLabel		*lab;
	QPushButton	*button;
	QLineEdit	*editbox;
	bool			what=this->getAsFolder;

	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	newwidg->setLayout(hlayout);

	lab=new QLabel(label,newwidg);	
	lab->setFixedWidth(this->labelWidth);
	hlayout->addWidget(lab,0,Qt::AlignLeft);

	button=new QPushButton(QIcon::fromTheme("document-open"),"",newwidg);	
	hlayout	->addWidget(button,0);

	editbox=new QLineEdit(path,newwidg);
	this->widgets.push_back({EDITWIDGET,editbox,label});
	editbox->setCursorPosition(0);
	hlayout->addWidget(editbox,1);	

	this->files[label]=path;
	QObject::connect(editbox,&QLineEdit::textChanged,[this,label](const QString &text)
		{
			this->files[label]=text;
		});

	QObject::connect(button,&QPushButton::clicked,[this,label,editbox,openhere,what]()
		{
			QString filepath="";
			if(what==true)
				filepath=QFileDialog::getExistingDirectory(this->theParent,"Open File",openhere);
			else
				filepath=QFileDialog::getOpenFileName(this->theParent,"Select folder",openhere,"",nullptr,QFileDialog::HideNameFilterDetails);
			if(filepath.isEmpty()==false)
				{
					editbox->setText(filepath);
					this->files[label]=filepath;
				}
		});
	return(newwidg);
}

QWidget	*prefsWidgetsClass::getPrefsFileWidget(QString label,QString path,QString openhere)
{
	QWidget		*newwidg;
	QString		prefpath;
	QSettings	plugprefs("KDHedger",this->thePrefsName);
	prefpath=plugprefs.value(this->normalizeLabel(label),path).toString();
	newwidg=this->getFileWidget(label,prefpath,openhere);
	return(newwidg);
}

QString prefsWidgetsClass::getFileValue(QString label)
{
	return(this->files[label]);
}

void prefsWidgetsClass::setFilePref(QString label)
{
	this->setPref(this->files,label);
}

void prefsWidgetsClass::setAllFilePrefs(void)
{
	this->setAllPrefs(this->files);
}

void prefsWidgetsClass::setAllPrefs(QHash<QString,QString> what)
{
	QSettings						plugprefs("KDHedger",this->thePrefsName);
	QHashIterator<QString,QString>	i(what);
	while(i.hasNext())
		{
			i.next();
			plugprefs.setValue(this->normalizeLabel(qPrintable(i.key())),qPrintable(i.value()));
		}
}

void prefsWidgetsClass::setPref(QHash<QString,QString> what,QString label)
{
	QSettings	plugprefs("KDHedger",this->thePrefsName);
	plugprefs.setValue(this->normalizeLabel(label),what[label]);
}

QWidget	*prefsWidgetsClass::getPrefsWidget(QString label,QString defaulttxt,whatWidget what)
{
	QString		preftxt="";
	QWidget		*newwidg=NULL;
	QSettings	plugprefs("KDHedger",this->thePrefsName);

	preftxt=plugprefs.value(this->normalizeLabel(label),defaulttxt).toString();
	switch(what)
		{
			case EDITWIDGET:
				newwidg=this->getEditWidget(label,preftxt);
				break;
			case COLOURWIDGET:
				newwidg=this->getColourWidget(label,preftxt);
				break;
			case FONTWIDGET:
				newwidg=this->getFontWidget(label,preftxt);
				break;
			default:
				break;
		}

	return(newwidg);
}

void prefsWidgetsClass::setLabelWidth(int sze)
{
	this->labelWidth=sze;
}
