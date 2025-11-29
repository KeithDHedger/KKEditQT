/*
 *
 * ©K. D. Hedger. Wed 12 Nov 11:59:32 GMT 2025 keithdhedger@gmail.com

 * This file (docBrowser.cpp) is part of KKEditQT.

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

#include "docBrowser.h"
#include "QT_menuitem.h"
#include "KKEditClass.h"

docBrowserClass::~docBrowserClass()
{
	QSettings	prefs;
	QRect		rg;
	QRect		rf;

	if(this->winWidget!=NULL)
		{
			rg=this->winWidget->geometry();
			rf=this->winWidget->frameGeometry();
			rf.setHeight(rf.height()-(rf.height()-rg.height()));
			rf.setWidth(rf.width()-(rf.width()-rg.width()));
			prefs.setValue("docClassDocWindow/geometry",rf);
			this->winWidget->close();
			delete this->winWidget;
		}
}

docBrowserClass::docBrowserClass(KKEditClass *kk)
{
	this->mainKKEditClass=kk;
}

void docBrowserClass::setPaths(QUrl path)
{

	QString pth=path.toString();

	if(pth.startsWith("file://"))
		pth=pth.mid(7);
	if(QFileInfo(pth).exists()==true)
		{
			this->basePath=pth;
			this->baseDir=QFileInfo(this->basePath).path();
		}
}

bool docBrowserClass::setToPathOrHTML(QUrl path)
{
	QString	ts=path.toString();
	QString	mime=QMimeDatabase().mimeTypeForFile(ts,QMimeDatabase::MatchDefault).name();
	if(mime.contains("video"))
		{
			te->backward();
			te->forward();
			return(true);
		}
this->mainKKEditClass->currentURL="file://"+ts;

	if(mime.contains("html")==true)
		{
			te->setSource(path);
			this->setPaths(path);
			return(true);
		}

	if(mime.contains("html")==false)
		{
			if(ts.startsWith("file://"))
				ts=ts.mid(7);
			QString	content;
			QFile	extfile(ts);
			bool		retval;

			retval=extfile.open(QIODevice::Text | QIODevice::ReadOnly);
			if(retval==true)
				{
					content=extfile.readAll();
					extfile.close();
					content=content.toHtmlEscaped();
					content="<!DOCTYPE html><html><body><pre>"+content+"</pre></body></html>";
					te->setHtml(content);
					this->noBack=true;
					return(true);
				}
		}

	return(false);
}

void docBrowserClass::openSrcFile(QString path)
{
	QString	str=path;
	QString	anchor;

	str.remove(QRegularExpression("file:\\/\\/"));
	anchor=QRegularExpression(R"RX(.*#(.*))RX").match(str).captured(1);
	str.remove(QRegularExpression(R"RX(#.*$)RX"));

	QString contents=this->mainKKEditClass->runPipeAndCapture(QString("sed -n '/<title>/p' '%1'").arg(str),true).simplified();

	contents=QRegularExpression(R"RX(\b([[:alnum:]\._]*)( (Struct|Class|File) Reference| Source File)?</title>)RX").match(contents).captured(1);
	if(this->mainKKEditClass->goToDefinition(contents)==true)
		return;
	else
		{
			if(this->mainKKEditClass->openFile("../"+contents,anchor.mid(1).toInt())==false)
				{
					DocumentClass *doc=this->mainKKEditClass->getDocumentForTab(-1);
					QString dp=doc->getDirPath();
					this->mainKKEditClass->openFile(dp+"/"+contents,anchor.mid(1).toInt());
				}
		}
}

void docBrowserClass::createNewWindow(QString path1)
{
	QVBoxLayout	*docvlayout=new QVBoxLayout;
	QHBoxLayout	*hlayout;
	QPushButton	*button;
	QSettings	prefs;
	QRect		rg;
	QString		path=path1;

//	if(QFileInfo(path1).exists()==false)
//		{
//			if(this->winWidget!=NULL)
//				{
//					this->winWidget->close();
//					delete this->te;
//					delete this->winWidget;
//					this->te=NULL;
//					this->winWidget=NULL;
//				}
//			return;
//		}

	if(this->winWidget==NULL)
		{
			QTextBrowser	*te=new QTextBrowser;
			this->winWidget=new QWidget;
			hlayout=new QHBoxLayout;

			te->setAcceptRichText(true);
			this->te=te;

			docvlayout->setContentsMargins(MARGINS,MARGINS,MARGINS,MARGINS);
			docvlayout->addWidget(te);

			QDir::setCurrent(QFileInfo(path).path());

			if(path1.contains(QRegularExpression("^.*#.*$")))
				te->setSource(path1);
			else
				this->setToPathOrHTML(path);

			te->setOpenExternalLinks(true);
			te->setContextMenuPolicy(Qt::CustomContextMenu);

			this->setPaths(path1);

			this->homePath=this->basePath;
			this->homeDir=this->baseDir;

			te->setSearchPaths(QStringList()<<this->homeDir);
	
			QObject::connect(te,&QWidget::customContextMenuRequested,[this,te](const QPoint &pos)
				{
					QMenu		*menu=new QMenu(te);
					const QUrl	linkUrl = te->anchorAt(pos);
					QTextCursor	txt=te->textCursor();
					bool			gotsel=false;

					if(txt.hasSelection()==true)
						{
							gotsel=true;
							QAction	*copyText=menu->addAction("Copy Text");
							QObject::connect(copyText, &QAction::triggered, [this,txt]()
								{
									QGuiApplication::clipboard()->setText(txt.selectedText());
								});
						}

			        // Add custom actions
					if(linkUrl.isValid())
						{
							QAction	*copyAction=menu->addAction("Copy Link Address");

							QAction	*openAction=menu->addAction("Open Link In External App");
							QAction	*openInSrcAction=menu->addAction("Open Link In Source File");

							// Connect the actions
							QObject::connect(copyAction, &QAction::triggered, [this,te,linkUrl]()
								{
									QString retval=this->urlTofile(linkUrl);
									QGuiApplication::clipboard()->setText(retval);
								});
 
							QObject::connect(openAction, &QAction::triggered, [this,te,linkUrl]()
								{
									QString retval=this->urlTofile(linkUrl);
									QDesktopServices::openUrl(QUrl(retval));
								});

							QObject::connect(openInSrcAction, &QAction::triggered, [this,te,linkUrl]()
								{
									QString s;
									if(linkUrl.isLocalFile()==true)
										s=QFileInfo(linkUrl.toString()).absoluteFilePath();
									else
										s=QFileInfo(this->baseDir+"/"+linkUrl.toString()).absoluteFilePath();
									this->openSrcFile(s);
								});

							// Add existing actions to the menu
							menu->addSeparator(); // Optional: Separator for better appearance
							menu->exec(te->mapToGlobal(pos));
							return;
						}
					else
						{
							if(gotsel==true)
								menu->exec(te->mapToGlobal(pos));
							return;
						}
				});

			QObject::connect(te, &QTextBrowser::anchorClicked, [this,te](const QUrl &link)
				{
					bool		setsrc=false;
					bool		ret=false;
					QString	ts=this->urlTofile(link);

					ret=this->setToPathOrHTML(QUrl(ts));
					QString addfile;
					if(ts.startsWith("file://"))
						addfile="";
					else
						addfile="file://";
					this->mainKKEditClass->currentURL=addfile+ts;

					if(ret==true)
						return;
					if(link.toString().contains(QRegularExpression(R"RX(.*#.*$)RX")))
						{
							if(link.toString().at(0)=='.')
								{
									QDir::setCurrent(this->baseDir);
								}
							this->basePath=link.toString().replace(QRegularExpression(R"RX((.*)#.*)RX"),"\\1");
							if(this->basePath.startsWith("file://"))
								this->basePath=this->basePath.mid(7);
							this->basePath=QFileInfo(this->basePath).canonicalFilePath();
							this->baseDir=QFileInfo(QFileInfo(this->basePath).path()).canonicalFilePath();
							return;
						}

					if(link.toString().startsWith("file://")==true)
						{
							this->basePath=link.toString().mid(7).replace(QRegularExpression(R"RX((.*)#.*)RX"),"\\1");
							this->baseDir=QFileInfo(this->basePath).path();
						}
					else
						{
							this->basePath=QFileInfo(this->basePath).path()+"/"+link.toString();
							this->baseDir=QFileInfo(this->basePath).path();
							setsrc=true;
						}

					this->basePath=QFileInfo(this->basePath).canonicalFilePath();
					this->baseDir=QFileInfo(this->baseDir).canonicalFilePath();
					if(this->basePath.isEmpty()==true)
						{
							this->basePath=link.toString();
							this->baseDir=QFileInfo(this->basePath).path();
							this->basePath=QFileInfo(this->basePath).canonicalFilePath();
							this->baseDir=QFileInfo(this->baseDir).canonicalFilePath();
							setsrc=true;	
						}

					if(setsrc==true)
						te->setSource(this->basePath);

					if(this->basePath.isEmpty()==true)
						{
							this->basePath=te->source().toString();
							this->baseDir=QFileInfo(this->basePath).path();
							this->basePath=QFileInfo(this->basePath).canonicalFilePath();
							this->baseDir=QFileInfo(this->baseDir).canonicalFilePath();
							setsrc=false;
						}

					if(setsrc==true)
						te->setSource(this->basePath);
				});

//home
			button=new QPushButton("&Home");
			QObject::connect(button,&QPushButton::clicked,[this,te]()
				{
					this->setToPathOrHTML(QUrl(this->homePath));
					this->basePath=this->homePath;
					this->baseDir=this->homeDir;
				});
			hlayout->addWidget(button);
			hlayout->addStretch();

//back
			button=new QPushButton("&Back");//CRAPPY!!!
			QObject::connect(button,&QPushButton::clicked,[this,te]()
				{
					if(te->isBackwardAvailable()==false)
						return;
					if(this->noBack==false)
						te->backward();
					else
						{
							te->reload();
							this->noBack=false;
							return;
						}

					this->noBack=false;
					this->basePath=QFileInfo(te->historyUrl(0).toString()).canonicalFilePath();
					this->baseDir=QFileInfo(QFileInfo(te->historyUrl(0).toString()).path()).canonicalFilePath();
					if(this->basePath.isEmpty()==true)
						{
							this->basePath=te->historyUrl(0).toString().mid(7);
							this->basePath=this->basePath.replace(QRegularExpression(R"RX((.*)#.*)RX"),"\\1");
							this->basePath=QFileInfo(this->basePath).canonicalFilePath();
							this->baseDir=QFileInfo(QFileInfo(this->basePath).path()).canonicalFilePath();
						}
				});
			hlayout->addWidget(button);

//forward
			button=new QPushButton("&Forward");
			QObject::connect(button,&QPushButton::clicked,[this,te]()
				{
					if(te->isForwardAvailable()==false)
						return;
					te->forward();

					this->basePath=QFileInfo(te->historyUrl(0).toString()).canonicalFilePath();
					this->baseDir=QFileInfo(QFileInfo(this->basePath).path()).canonicalFilePath();

					if(this->basePath.isEmpty()==true)
						{
							this->basePath=te->historyUrl(0).toString().mid(7);
							this->basePath=this->basePath.replace(QRegularExpression(R"RX((.*)#.*)RX"),"\\1");
							this->basePath=QFileInfo(this->basePath).canonicalFilePath();
							this->baseDir=QFileInfo(QFileInfo(this->basePath).path()).canonicalFilePath();
						}
					this->setToPathOrHTML(te->source());	
				});
			hlayout->addWidget(button);
			hlayout->addStretch();

//close
			button=new QPushButton("&Hide");
			button->setIcon(QIcon::fromTheme("window-close"));
			QObject::connect(button,&QPushButton::clicked,[this]()
				{
					QSettings	prefs;
					QRect		rg;
					QRect		rf;

					rg=this->winWidget->geometry();
					rf=this->winWidget->frameGeometry();
					rf.setHeight(rf.height()-(rf.height()-rg.height()));
					rf.setWidth(rf.width()-(rf.width()-rg.width()));
					prefs.setValue("docClassDocWindow/geometry",rf);
					this->winWidget->hide();
					this->mainKKEditClass->docviewerVisible=false;
					this->mainKKEditClass->toggleDocViewMenuItem->setText("Show Docviewer");
				});
			hlayout->addWidget(button);

			docvlayout->addLayout(hlayout);

		}
	else
		{
			QDir::setCurrent(QFileInfo(path).path());
			if(path1.contains(QRegularExpression("^.*#.*$")))
				this->te->setSource(path1);
			else
				this->setToPathOrHTML(path);

			QString addfile;
			if(path.startsWith("file://"))
				addfile="";
			else
				addfile="file://";
			this->mainKKEditClass->currentURL=addfile+path;

			this->winWidget->setWindowTitle(this->windowTitle);
			this->te->reload();
			this->winWidget->show();
			this->winWidget->activateWindow();
			this->winWidget->raise();
			return;
		}

	this->winWidget->setWindowTitle(this->windowTitle);
	this->winWidget->setLayout(docvlayout); 
	rg=prefs.value("docClassDocWindow/geometry",QVariant(QRect(50,50,1024,768))).value<QRect>();
	this->winWidget->setGeometry(rg);
	this->winWidget->show();
	this->winWidget->activateWindow();
	this->winWidget->raise();
}

QString docBrowserClass::urlTofile(QUrl path)
{
	QString retval=path.toString();

	if(path.scheme().isEmpty()==false)
		return(path.toString());

	if(path.toString().at(0)=='#')
		{
			retval=QFileInfo(this->basePath).canonicalFilePath()+path.toString();
			return("file://"+retval);
		}

	if(path.toString().contains(QRegularExpression(R"RX(.*#.*$)RX")))
		{
			if(this->basePath.startsWith(this->homePath)==true)
				{
					if(path.toString().startsWith("file://")==true)
						this->basePath=path.toString().mid(7);
					if(QFileInfo(path.toString()).isRelative()==false)
						{
							this->basePath=this->basePath.replace(QRegularExpression(R"RX((.*)#.*)RX"),"\\1");
							this->basePath=QFileInfo(basePath).canonicalFilePath();
							this->baseDir=QFileInfo(QFileInfo(this->basePath).path()).canonicalFilePath();
							return(path.toString());
						}
					else
						{
							return("file://"+this->baseDir+"/"+path.toString());
						}
				}
			else
				{
					if(QFileInfo(path.toString()).exists()==true)
						return(path.toString());
					else
						return("file://"+this->baseDir+"/"+path.toString());
				}
		}

	retval=QFileInfo(path.toString()).canonicalFilePath();
	if(retval.isEmpty()==true)
		retval=QFileInfo(this->baseDir+"/"+path.toString()).canonicalFilePath();

	return("file://"+retval);
}
