/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:38:31 GMT 2021 keithdhedger@gmail.com

 * This file (KKEditClassCallbacks.cpp) is part of KKEditQT.

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

#include "KKEditClass.h"

void KKEditClass::doSessionsMenuItems(void)
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());
	QFile			file;
	bool				retval;
	DocumentClass	*doc;
	bookMarkStruct	bm;
	QString			sessionname;
	QString			tempstr;
	MenuItemClass	*menuItemSink;
	unsigned int		sessionnumber;
	plugData			pd;

//this is crappy needs redoing
	if(mc==NULL)
		sessionnumber=0;
	else
		sessionnumber=mc->getMenuID();

	if((sessionnumber==0) || (sender()->objectName().compare(SAVESESSIONMENUNAME)==0) || (sender()->objectName().compare(QUITMENUNAME)==0))
		{
			if(sessionnumber!=0)
				{
					if(sender()->objectName().compare(QUITMENUNAME)==0)
						{
							sessionnumber=0;
						}
					else
						{
							if(sessionnumber==CURRENTSESSION)
								{
									if(this->currentSessionNumber==0xdeadbeef)
										return;
									sessionnumber=this->currentSessionNumber;
								}
						}
				}
			sessionname=QString("Session-%1").arg(sessionnumber);
			file.setFileName(QString("%1/Session-%2").arg(this->sessionFolder).arg(sessionnumber));
//get sesion name
			retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
			if(retval==true)
				{
					QTextStream	in(&file);
					tempstr=in.readLine();//sessionname
					file.close();
					if(tempstr.isEmpty()==false)
						{
							sessionname=tempstr;
						}
				}

//TODO//set session name for save
			retval=file.open(QIODevice::Text | QIODevice::WriteOnly);
			if(retval==true)
				{
					int linenumber=10;
					bool	ok;
					if((sessionnumber!=0) && (mc->getMenuID()!=CURRENTSESSION))
						{
							if(sender()->objectName().compare(QUITMENUNAME)!=0)
								{
									QString	text=QInputDialog::getText(this->mainWindow,"Session Name","Enter Session Name",QLineEdit::Normal,sessionname,&ok);
									if ((ok==true) && (!text.isEmpty()))
										{
											sessionname=(text);
											mc->setText(sessionname);
											this->sessionNames[mc->getMenuID()]=sessionname;
										}
								}
						}
					else
						{
							sessionname=this->sessionNames.value(sessionnumber);
						}

					this->currentSessionNumber=sessionnumber;
					this->restoreSessionsMenu->clear();
					this->restoreSessionsMenu->addAction(this->restoreDefaultSessionMenuItem);
					this->restoreSessionsMenu->addSeparator();
					this->restoreSessionMenuItemsList.clear();
					for(int j=1;j<MAXSESSIONS;j++)
						{
							menuItemSink=this->makeMenuItemClass(RESTORESESSIONSMENU,this->sessionNames.value(j),0,NULL,RESTORESESSIONMENUNAME,j);
							this->restoreSessionMenuItemsList.append(menuItemSink);
						}

					QTextStream(&file) << sessionname << Qt::endl;
					QTextStream(&file) << this->mainWindow->geometry().x() << " " << this->mainWindow->geometry().y() << " " <<this->mainWindow->geometry().width() << " " << this->mainWindow->geometry().height() << Qt::endl;
					QTextStream(&file) << "#RESERVED" << Qt::endl;
					QTextStream(&file) << "#RESERVED" << Qt::endl;
					QTextStream(&file) << "#RESERVED" << Qt::endl;

					for(int j=0;j<this->mainNotebook->count();j++)
						{
							doc=this->getDocumentForTab(j);
							linenumber=doc->getCurrentLineNumber();
							QTextStream(&file) << linenumber << " " << doc->visible << " " << doc->getFilePath() << Qt::endl;
							foreach(bookMarkStruct bm, this->bookMarks)
								{
									if(doc->pageIndex==bm.docIndex)
										QTextStream(&file) << bm.line << " " << bm.bmLabel << Qt::endl;
								}
							QTextStream(&file) << "-1 endmarks" << Qt::endl;
						}
					file.close();
//plugins
					pd.userStrData1=sessionname;
					pd.userIntData1=sessionnumber;
					pd.what=DOSAVESESSION;
					this->runAllPlugs(pd);
				}
			return;
		}

	if(sender()->objectName().compare(RESTORESESSIONMENUNAME)==0)
		{
			this->sessionBusy=true;
			sessionnumber=mc->getMenuID();
//autosave session
			if(sessionnumber==CURRENTSESSION)
				sessionnumber=0;

			this->closeAllTabs();
			this->sessionBusy=true;
			file.setFileName(QString("%1/Session-%2").arg(this->sessionFolder).arg(sessionnumber));
			QString retdata=this->runPipeAndCapture(QString("cat '%1'|sed -n \"/^.*endmarks$/p\"|wc -l").arg(file.fileName()));
			retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
			int cntfiles=0;
			if(retval==true)
				{
					this->showBarberPole("Restore Session","Please Wait","",retdata,QString("%1/session").arg(this->tmpFolderName));
					this->splash->finish(this->mainWindow);
					int		linenumber=999;
					int		visible=666;
					int		mainline;
					QTextStream	in(&file);
					QString sessionname;
					QString filename;
					QString bookmark;
					sessionname=in.readLine();//sessionname
					int		x,y,w,h;//geometry
					QString unused;//reserved
					in >> x;
					in >> y;
					in >> w;
					in >> h;
					unused=in.readLine().trimmed();
					unused=in.readLine().trimmed();
					unused=in.readLine().trimmed();
					unused=in.readLine().trimmed();

					while(in.atEnd()==false)
						{
							in >> mainline;
							in >> visible;
							filename=in.readLine().trimmed();
							linenumber=-1;
							this->runNoOutput(QString("echo -e \"Opening %1 ...\n%3\">\"%2/session\" &").arg(filename.trimmed()).arg(this->tmpFolderName).arg(cntfiles++));
							this->openFile(filename,0,false,false);
							do
								{
									in >> linenumber;
									bookmark=in.readLine().trimmed();
									if(linenumber>0)
										{
											this->gotoLine(linenumber);
											QTextCursor tc;
											this->handleBMMenu(this->mainNotebook->currentWidget(),TOGGLEBOOKMARKMENUITEM,tc);
										}
								}
							while(linenumber!=-1);
							this->setTabVisibilty(this->mainNotebook->currentIndex(),visible);
							this->gotoLine(mainline);
						}
					this->runNoOutput(QString("echo -ne \"Finishing ...\n%1\">\"%2/session\"").arg(retdata).arg(this->tmpFolderName));
					sleep(1);
					this->runNoOutput(QString("echo -e quit>\"%1/session\"").arg(this->tmpFolderName));

					this->currentSessionNumber=sessionnumber;
					file.close();
					this->mainWindow->setGeometry(x,y,w,h);
//plugins
					pd.userStrData1=sessionname;
					pd.userIntData1=sessionnumber;
					pd.what=DORESSESSION;
					this->runAllPlugs(pd);
				}

			for(int j=0;j<this->mainNotebook->count();j++)
				{
					doc=this->getDocumentForTab(j);
					doc->document()->clearUndoRedoStacks(QTextDocument::UndoAndRedoStacks);
					doc->dirty=false;
					doc->visible=this->mainNotebook->isTabVisible(j);
				}
		}

	this->sessionBusy=false;
	this->setCompWordList();
	for(int j=this->mainNotebook->count()-1;j>-1;j--)
		{
			if(this->mainNotebook->isTabVisible(j)==true)
				{
					this->switchPage(j);
					break;
				}
		}
	this->rebuildTabsMenu();
	this->setToolbarSensitive();
}

void KKEditClass::doBookmarkMenuItems()
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());
	DocumentClass	*document=this->getDocumentForTab(-1);
	QTextCursor		tc;

	switch(mc->getMenuID())
		{
			case REMOVEALLBOOKMARKSMENUITEM:
				this->rebuildBookMarkMenu();
				for(int j=0;j<this->mainNotebook->count();j++)
					{
						document=this->getDocumentForTab(j);
						document->extraBMSelections.clear();
						document->setXtraSelections();
					}
				break;
			case TOGGLEBOOKMARKMENUITEM:
				this->handleBMMenu(this->mainNotebook->currentWidget(),TOGGLEBOOKMARKMENUITEM,tc);
				break;
			default:
				this->handleBMMenu(this->mainNotebook->currentWidget(),mc->getMenuID(),tc);
				break;
		}
}

void KKEditClass::doToolsMenuItems()
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());
	DocumentClass	*document=this->getDocumentForTab(-1);
	QStringList		sl;
	QString			filelist;

	switch(mc->getMenuID())
		{
			 case MANAGETOOLSMENUITEM:
			 	this->toolsWindow->show();
			 	break;

			 default:
			 	sl=this->verifyTool(mc->getMenuString());
			 	if(sl.isEmpty()==true)
			 		return;

				QHashIterator<int,DocumentClass*> i(this->pages);
				while(i.hasNext())
					{
						i.next();
						filelist+=i.value()->getFilePath()+" ";
					}

				if(sl.at(TOOL_COMMAND).section(TOOLCOMMAND,1,1).trimmed().isEmpty()==false)
					{
						QString	command=sl.at(TOOL_COMMAND).section(TOOLCOMMAND,1,1).trimmed();
						bool		usebar=false;
						bool		showdoc=false;
						bool		rootrun=false;
						bool		interm=false;
						bool		sync=false;
						bool		clearop=false;
						int		viewflags=sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_INSERT_MASK;

						if(document!=NULL)
							{
//								//%l
								setenv("KKEDIT_FILE_LIST",filelist.trimmed().toStdString().c_str(),1);
								command.replace("%l",filelist);
								//%f doc filepath
								setenv("KKEDIT_CURRENTFILE",document->getFilePath().toStdString().c_str(),1);
								command.replace("%f",document->getFilePath());
								//%n
								setenv("KKEDIT_FILECOUNT",QString("%1").arg(this->mainNotebook->count()).toStdString().c_str(),1);
								command.replace("%n",QString("%1").arg(this->mainNotebook->count()));
								//%d doc folder
								setenv("KKEDIT_CURRENTDIR",document->getDirPath().toStdString().c_str(),1);
								command.replace("%d",document->getDirPath());
								//%t selected text
								setenv("KKEDIT_SELECTION",document->textCursor().selectedText().replace(QRegularExpression("\u2029|\\r\\n|\\r"),"\n").toStdString().c_str(),1);
								command.replace("%t",document->textCursor().selectedText());
								//%m
								setenv("KKEDIT_MIMETYPE",document->mimeType.toStdString().c_str(),1);
								command.replace("%m",document->mimeType);
							}

						//%s
						setenv("KKEDIT_CURRENT_SESSION",this->sessionNames.value(this->currentSessionNumber).toStdString().c_str(),1);
						command.replace("%s",filelist);
						//%h html file
						setenv("KKEDIT_HTMLFILE",this->htmlFile.toStdString().c_str(),1);
						command.replace("%h",this->htmlFile);
						//%i
						setenv("KKEDIT_DATADIR",DATADIR,1);
						command.replace("%i",DATADIR);

						if(sl.at(TOOL_USE_BAR).section(TOOLUSEPOLE,1,1).trimmed().toInt()==1)
							usebar=true;

						if(sl.at(TOOL_RUN_AS_ROOT).section(TOOLRUNASROOT,1,1).trimmed().toInt()==1)
							rootrun=true;
	
						if(((sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_PASTE_OP)==TOOL_PASTE_OP) && (document!=NULL))
							{
								//pasteop=true;
//								//ignoreop=false;
	//							//replacedoc=false;//
								//viewop=false;
							}

						if(((sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_REPLACE_OP)==TOOL_REPLACE_OP) && (document!=NULL))
							{
								//replacedoc=true;
							//	pasteop=false;
								//ignoreop=false;
								//viewop=false;
							}

						if((sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_VIEW_OP)==TOOL_VIEW_OP)
							{
								//viewop=true;
								//replacedoc=false;
							//	pasteop=false;
							//	ignoreop=false;
							}

						if(sl.at(TOOL_CLEAR_VIEW).section(TOOLCLEAROP,1,1).trimmed().toInt()==1)
							clearop=true;
//
//						if(sl.at(TOOL_CLEAR_VIEW).section(TOOLCLEAROP,1,1).trimmed().toInt()==1)
//							clearop=true;
//
						if((sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_SHOW_DOC)==TOOL_SHOW_DOC)
							showdoc=true;

						if((sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_ASYNC)==TOOL_ASYNC)
							{
								sync=false;
								clearop=false;
//								pasteop=false;
//								replacedoc=false;
//								ignoreop=false;/
								showdoc=false;
							}
						else
							{
								sync=true;
							}

						if(sl.at(TOOL_IN_TERM).section(TOOLRUNINTERM,1,1).trimmed().toInt()==1)
							{
								interm=true;
								showdoc=false;
								clearop=false;
//								pasteop=false;
//								replacedoc=false;
//								ignoreop=false;
								sync=false;
							}

						//%p progress bar control file
						if(usebar==true)
							{
								setenv("KKEDIT_BAR_CONTROL",QString("%1/progress").arg(this->tmpFolderName).toStdString().c_str(),1);
								command.replace("%p",QString("%1/progress").arg(this->tmpFolderName));
								this->showBarberPole(sl.at(TOOL_NAME).section(TOOLNAME,1,1).trimmed(),"Running tool ..","","100",QString("%1/progress").arg(this->tmpFolderName));
							}

						if(interm==true)
							{
								QString str;

								if(rootrun==false)
									str=QString("%1 'sh -c \"cd %2;%3\"'").arg(this->prefsTerminalCommand).arg(this->toolsFolder).arg(command);
								else
									str=QString("%1 'sudo sh -c \"cd %2;%3\"'").arg(this->prefsTerminalCommand).arg(this->toolsFolder).arg(command);

								this->runNoOutput(str,false,false);
								return;
							}

//qDebug()<<"sync"<<sync;
//qDebug()<<"ignoreop"<<ignoreop;
//qDebug()<<"pasteop"<<pasteop;
//qDebug()<<"replacedoc"<<replacedoc;
//qDebug()<<"clearop"<<clearop;
//qDebug()<<"viewop"<<viewop;
//qDebug()<<"viewflags"<<viewflags;
//qDebug()<<"showdoc"<<showdoc;		
//qDebug()<<"rootrun"<<rootrun;		

						if(sync==true)
							{
								QString str;

								str=QString("cd %1;%3").arg(this->toolsFolder).arg(command);
								switch(viewflags)
									{
										case TOOL_PASTE_OP:
											document->textCursor().beginEditBlock();
												document->textCursor().removeSelectedText();
												if(rootrun==false)
													document->textCursor().insertText(this->runPipeAndCapture(str));
												else
													document->textCursor().insertText(this->runPipeAndCapture(QString("%1 sh -c \"%2\"").arg(this->prefsRootCommand).arg(str)));						
											document->textCursor().endEditBlock();
											break;

										case TOOL_REPLACE_OP:
											document->textCursor().beginEditBlock();
												if(rootrun==false)
													document->setPlainText(this->runPipeAndCapture(str));
												else
													document->setPlainText(this->runPipeAndCapture(QString("%1 sh -c \"%2\"").arg(this->prefsRootCommand).arg(str)));
											document->textCursor().endEditBlock();
											break;

										case TOOL_VIEW_OP:
											if(clearop==false)
												{
													QTextCursor tc=this->toolsOPText->textCursor();
													tc.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
													this->toolsOPText->setTextCursor(tc);
												}
											else
												this->toolsOPText->setPlainText("");

											this->toolOutputWindow->setWindowTitle(sl.at(TOOL_NAME).section(TOOLNAME,1,1).trimmed());
											if(rootrun==false)
												this->runPipeAndCaptureToToolOP(str);
											else
												this->runPipeAndCaptureToToolOP(QString("%1 sh -c \"%2\"").arg(this->prefsRootCommand).arg(str));
											break;

										default:
											this->runNoOutput(command,true,rootrun);
											break;
									}
							}
						else
							{
								this->runNoOutput(command,false,rootrun);
							}

						if(showdoc==true)
							this->showWebPage(sl.at(TOOL_NAME).section(TOOLNAME,1,1).trimmed(),"file://" + this->htmlFile);
					}
			 	break;
		}
}

void KKEditClass::doHelpMenuItems()
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());

	switch(mc->getMenuID())
		{
			 case ABOUTMENUITEM:
			 	{
					QString			content;
					QFile			licencefile(DATADIR "/docs/gpl-3.0.txt");
					bool			retval;
					AboutBoxClass	*about=new AboutBoxClass(this->mainWindow,DATADIR "/pixmaps/" PACKAGE ".png");

					retval=licencefile.open(QIODevice::Text | QIODevice::ReadOnly);
					if(retval==true)
						{
							content=QString::fromUtf8(licencefile.readAll());
							licencefile.close();
							about->setLicence(content);
						}
					about->setAuthors(defaultauthors);
					about->runAbout();
				}
			 	break;
			 case ABOUTQTMENUITEM:
			 	this->application->aboutQt();
			 	break;
			 case HELPMENUITEM:
			 	this->showWebPage("Help For " PACKAGE,"file://" DATADIR "/help/index.html");
			 	break;
		}
}

void KKEditClass::doNavMenuItems()
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());

	switch(mc->getMenuID())
		{
			case GOBACKMENUITEM:
				this->history->goBack();
				break;
			case GOFORWARDMENUITEM:
				this->history->goForward();
				break;
			case GOTODEFINEMENUITEM:
				this->goToDefinition("");
				break;
			case OPENINCLUDEMENUITEM:
				this->findFile();
				break;
			case GOTOLINEMENUITEM:
				this->showLineEntry();
				break;
			case SEARCHFORDEFINEMENUITEM:
				this->functionSearchDialog();
				break;
			case SEARCHGTKDOCS:
				this->searchAPIDocs("",0);
				break;
			case SEARCHQT5DOCS:
				this->searchAPIDocs("",1);
				break;
		}
}

void KKEditClass::doViewMenuItems()
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());

	switch(mc->getMenuID())
		{
			case DOCSMENUITEM:
				this->showDocs();
				break;
			case TOGGLETOOLBARMENUITEM:
				this->toolbarVisible=!this->toolbarVisible;
				if(this->toolbarVisible)
					{
						this->toggleToolBarMenuItem->setText("Hide Tool Bar");
						this->toolBar->show();
					}
				else
					{
						this->toggleToolBarMenuItem->setText("Show Tool Bar");
						this->toolBar->hide();
					}
				break;
			case TOGGLETOOLWINDOWMENUITEM:
				this->toolWindowVisible=!this->toolWindowVisible;
				if(this->toolWindowVisible)
					{
						this->toolOutputWindow->show();
						this->toggleToolWindowMenuItem->setText("Hide Tool Output");
					}
				else
					{
						this->toolOutputWindow->hide();
						this->toggleToolWindowMenuItem->setText("Show Tool Output");
					}
				break;
			case TOGGLESTATUSBARMENUITEM:
				this->statusBarVisible=!this->statusBarVisible;
				if(this->statusBarVisible)
					{
						this->toggleStatusBarMenuItem->setText("Hide Status Bar");
						this->statusBar->show();
					}
				else
					{
						this->toggleStatusBarMenuItem->setText("Show Status Bar");
						this->statusBar->hide();
					}
				break;
			case TOGGLEDOCVIEWMENUITEM:
#ifdef _BUILDDOCVIEWER_
				this->docviewerVisible=!this->docviewerVisible;
				if(this->docviewerVisible)
					{
						this->toggleDocViewMenuItem->setText("Hide Docviewer");
						this->docView->show();
					}
				else
					{
						this->toggleDocViewMenuItem->setText("Show Docviewer");
						this->docView->hide();
					}
#endif
				break;
			case TOGGLELINENUMBERSMENUITEM:
				this->lineNumbersVisible=!this->lineNumbersVisible;
				this->resetAllFilePrefs();
				break;
			case TOGGLELINEWRAPMENUITEM:
				this->wrapLine=!this->wrapLine;
				this->resetAllFilePrefs();
				break;
			case TOGGLEHILITELINEMENUITEM:
				this->hiliteCurrentLine=!this->hiliteCurrentLine;
				this->resetAllFilePrefs();
				break;
			case TOGGLEWHITESPACEMENUITEM:
				this->showWhiteSpace=!this->showWhiteSpace;
				this->resetAllFilePrefs();
				break;
			case TOGGLESYNTAXHILITINGMENUITEM:
				this->showHighLighting=!this->showHighLighting;
				this->resetAllFilePrefs();
				break;
			case TOGGLECOMPLETIONSMENUITEM:
				this->showCompletions=!this->showCompletions;
				break;
		}
}

void KKEditClass::doEditMenuItems()
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());
	DocumentClass	*document=this->getDocumentForTab(-1);

	switch(mc->getMenuID())
		{
			case UNDOMENUITEM:
				if(document!=NULL)
					document->undo();
				break;
			case REDOMENUITEM:
				if(document!=NULL)
					document->redo();
				break;
			case UNDOALLMENUITEM:
				if(document!=NULL)
					while(document->document()->availableUndoSteps()>0)
						document->undo();
				break;
			case REDOALLMENUITEM:
				if(document!=NULL)
					{
						while(document->document()->availableRedoSteps()>0)
							document->redo();
						document->redo();
					}
				break;
			case CUTMENUITEM:
				if(document!=NULL)
					document->cut();
				break;
			case COPYMENUITEM:
				if(document!=NULL)
					document->copy();
				break;
			case PASTEMENUITEM:
				if(document!=NULL)
					document->paste();
				break;
			case DELETEMENUITEM:
				{
					QTextCursor	cursor;
					cursor=document->textCursor();
					cursor.beginEditBlock();

					if(document!=NULL)
						{
							if(cursor.hasSelection()==true)
								cursor.removeSelectedText();
						}
						cursor.endEditBlock();
				}
				break;
			case SELECTALLMENUITEM:
				if(document!=NULL)
					{
						document->selectAll();
						this->setToolbarSensitive();
					}
				break;
			case FINDMENUITEM:
				this->findReplaceDialog->show();
				break;
			case FINDNEXTMENUITEM:
				doFindReplace(FINDNEXT);
				if(this->hightlightAll==true)
					{
						if(document!=NULL)
							document->setHighlightAll();
					}
				break;
			case SORTTABSMENUITEM:
				this->sortTabs();
				break;
			case SHOWALLTABSMENUITEM:
				for(int j=0;j<this->mainNotebook->count();j++)
					{
						this->setTabVisibilty(j,true);
					}
				break;
			case PREFSMENUITEM:
				this->doPrefs();
				break;
			case PLUGPREFSMENUITEM:
				this->buildPlugPrefs();
				this->pluginPrefsWindow->exec();
				this->setToolbarSensitive();
				delete this->pluginPrefsWindow;
				break;
		}
}

void KKEditClass::doFileMenuItems()
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());

	switch(mc->getMenuID())
		{
//file menu
			case NEWMENUITEM:
				this->newFile();
				break;
			case OPENMENUITEM:
				this->openFileDialog();
				break;
			case HEXDUMPMENUITEM:
				this->openAsHexDump();
				break;
			case NEWADMINEDMENUITEM:
				this->newEditor(NEWADMINEDMENUITEM);
				break;
			case NEWEDMENUITEM:
				this->newEditor(NEWEDMENUITEM);
				break;
			case
				MANPAGEEDMENUITEM:
				this->newEditor(MANPAGEEDMENUITEM);
				break;
			case BUILDDOCSMENUITEM:
				this->buildDocs();
				break;
			case SAVEMENUITEM:
				this->saveFile(-1,false);
				this->setToolbarSensitive();
				break;
			case SAVEASMENUITEM:
				this->saveFileAs(-1);
				break;
			case SAVEALLMENUITEM:
				this->saveAllFiles(false);
				break;
			case PRINTMENUITEM:
				this->printDocument();
				break;
			case CLOSEMENUITEM:
				this->closingAllTabs=false;
				this->closeTab(-1);
				break;
			case CLOSEALLMENUITEM:
				this->closeAllTabs();
				break;
			case REVERTMENUITEM:
				this->reloadDocument();
				break;
			case QUITMENUITEM:
				this->shutDownApp();//TODO.//
				break;
		}
}

void KKEditClass::clickMenu(QMenu *menu,QString name)
{
	foreach(QAction *action,menu->actions())
		{
			if(action->menu())
				this->clickMenu(action->menu(),name);
			else
				if(action->text().compare(name)==0)
					action->triggered();
		}
}

void KKEditClass::notDoneYet(QString string)
{
	QTextCursor tc=this->toolsOPText->textCursor();
	tc.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
	this->toolsOPText->setTextCursor(tc);
	this->toolsOPText->insertPlainText(QString("%1\n").arg(string));
	this->toolOutputWindow->setWindowTitle("Information");
	this->toolOutputWindow->show();
	qInfo()<<string;
}

void KKEditClass::doTimer(void)
{
	int			retcode=0;
	msgStruct	buffer;
	
#ifdef _BUILDDOCVIEWER_
	this->setDocMenu();
#endif
	this->toolWindowVisible=this->toolOutputWindow->isVisible();
	if(this->toolWindowVisible==true)
		this->toggleToolWindowMenuItem->setText("Hide Tool Output");
	else
		this->toggleToolWindowMenuItem->setText("Show Tool Output");

	while(retcode!=-1)
		{
			buffer.mText[0]=0;
			retcode=msgrcv(this->queueID,&buffer,MAXMSGSIZE,MSGANY,IPC_NOWAIT);
			buffer.mText[retcode]=0;
			if(retcode!=-1)
				{
					switch(buffer.mType & ALLMSGTYPES)
						{
							case SENDCURRENTURL:
								this->sendMessgage(this->currentURL);
								break;
							case OPENINDOCVIEWMSG:
								this->showWebPage("",buffer.mText);
								break;
							case GOTOLINEMSG:
								this->gotoLine(strtol(buffer.mText,NULL,0));
								break;
							case SEARCHDEFMSG:
								this->goToDefinition(buffer.mText);
								break;
							case SELECTTABMSG:
								{
									long		tabnum=strtol(buffer.mText,NULL,0);
									QTabBar	*bar=this->mainNotebook->tabBar();
									if(1+tabnum>bar->count())
										tabnum=bar->count()-1;
									if(tabnum<0)
										tabnum=0;
									this->setTabVisibilty(tabnum,true);
								}
								break;
							case SELECTTABBYNAMEMSG:
								{
									QTabBar	*bar=this->mainNotebook->tabBar();
									for(int j=0;j<bar->count();j++)
										{
											if(bar->tabText(j).compare(buffer.mText)==0)
												{
													this->setTabVisibilty(j,true);
													break;
												}
										}
								}
								break;
							case SELECTTABBYPATHMSG:
								{
									QTabBar	*bar=this->mainNotebook->tabBar();
									for(int j=0;j<bar->count();j++)
										{
											if(bar->tabToolTip(j).compare(buffer.mText)==0)
												{
													this->setTabVisibilty(j,true);
													break;
												}
										}
								}
								break;
							case BOOKMARKMSG:
								{
									DocumentClass	*doc=this->getDocumentForTab(-1);
									if(doc!=NULL)
										{
											this->handleBMMenu(this->mainNotebook->currentWidget(),TOGGLEBOOKMARKMENUITEM,QTextCursor());
											doc->repaint();
										}
								}
								break;
							case CLOSETABMSG:
								this->closingAllTabs=false;
								this->closeTab(atoi(buffer.mText));
								break;
							case CLOSEALLTABSMSG:
								this->closeAllTabs();
								break;
							case SETUSERMARKMSG:
								this->notDoneYet("SETUSERMARKMSG not yet implemented");
								break;
							case UNSETUSERMARKMASG:
								this->notDoneYet("UNSETUSERMARKMASG not yet implemented");
								break;
							case MOVETOMSG:
								{
									QTabBar			*bar=this->mainNotebook->tabBar();
									DocumentClass	*doc;
									QTextCursor		cursor;
									QTextBlock		blockfrom;
									QString			str=buffer.mText;
									QStringList		list1=str.split(QLatin1Char(':'));

									if(list1.count()<3)
										{
											qWarning()<<"Move To need 3 values, eg 'TABNUM:LINENUM:COLNUM'";
											break;
										}
									int				tabnum=list1.at(0).toInt();
									int				linefrom=list1.at(1).toInt();
									int				colfrom=list1.at(2).toInt();
									int				blockfromstart;

									if(bar->count()==0)
										break;

									if(tabnum>bar->count()-1)
										tabnum=bar->count()-1;
									if(tabnum<0)
										tabnum=0;

									colfrom--;
									if(colfrom<0)
										colfrom=0;

									linefrom--;
									if(linefrom<0)
										linefrom=0;
									this->setTabVisibilty(tabnum,true);
									doc=this->getDocumentForTab(-1);
									if(doc!=NULL)
										{
											cursor=doc->textCursor();

											blockfrom=doc->document()->findBlockByNumber(linefrom);
											blockfromstart=blockfrom.position();
											if((colfrom)>blockfrom.length()-1)
												colfrom=blockfrom.length()-1;

											cursor.setPosition(blockfromstart+colfrom);
											doc->setTextCursor(cursor);
										}
								}
								break;
							case SELECTBETWEENMSG:
								{
									QTabBar			*bar=this->mainNotebook->tabBar();
									DocumentClass	*doc;
									QTextCursor		cursor;
									QTextBlock		blockfrom;
									QTextBlock		blockto;
									QString			str=buffer.mText;
									QStringList		list1=str.split(QLatin1Char(':'));
									if(list1.count()<3)
										{
											qWarning()<<"Move To need 5 values, eg 'TABNUM:LINEFROM:COLFROM:LINETO:COLTO'";
											break;
										}

									int				tabnum=list1.at(0).toInt();
									int				linefrom=list1.at(1).toInt();
									int				lineto=list1.at(3).toInt();
									int				colfrom=list1.at(2).toInt();
									int				colto=list1.at(4).toInt();
									int				blockfromstart;
									int				blocktostart;

									if(bar->count()==0)
										break;

									if(lineto<linefrom)
										break;
									if((lineto==linefrom) && (colto<=colfrom))
										break;
									if(tabnum>bar->count()-1)
										tabnum=bar->count()-1;
									if(tabnum<0)
										tabnum=0;

									colfrom--;
									colto--;
									if(colto<0)
										colto=0;
									if(colfrom<0)
										colfrom=0;

									linefrom--;
									lineto--;
									if(lineto<0)
										lineto=0;
									if(linefrom<0)
										linefrom=0;
									this->setTabVisibilty(tabnum,true);
									doc=this->getDocumentForTab(-1);
									if(doc!=NULL)
										{
//from
											cursor=doc->textCursor();

											blockfrom=doc->document()->findBlockByNumber(linefrom);
											blockfromstart=blockfrom.position();
											if((colfrom)>blockfrom.length()-1)
												colfrom=blockfrom.length();

											cursor.setPosition(blockfromstart+colfrom);
//to
											blockto=doc->document()->findBlockByNumber(lineto);
											blocktostart=blockto.position();
											if((colto)>blockto.length()-1)
												colto=blockto.length();

											cursor.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor,blocktostart-blockfromstart+(colto-colfrom));
											doc->setTextCursor(cursor);
										}
								}
								break;
							case PASTEMSG:
								{
									DocumentClass	*doc=this->getDocumentForTab(-1);
									if(doc!=NULL)
										doc->paste();
								}
								break;
							case COPYMSG:
								{
									DocumentClass	*doc=this->getDocumentForTab(-1);
									if(doc!=NULL)
										doc->copy();
								}
								break;
							case CUTMSG:
								{
									DocumentClass	*doc=this->getDocumentForTab(-1);
									if(doc!=NULL)
										doc->cut();
								}
								break;
							case INSERTTEXTMSG:
								{
									DocumentClass	*doc=this->getDocumentForTab(-1);
									if(doc!=NULL)
										{
											QTextCursor	cursor=doc->textCursor();
											QString		txt=buffer.mText;

											cursor.clearSelection();
											cursor.insertText(txt);
											doc->setTextCursor(cursor);
										}
								}
								break;
							case INSERTNLMSG:
								{
									DocumentClass	*doc=this->getDocumentForTab(-1);
									QTextCursor		cursor;
									if(doc!=NULL)
										{
											cursor=doc->textCursor();
											cursor.clearSelection();
											cursor.insertText("\n");
											doc->setTextCursor(cursor);
										}
								}
								break;
							case INSERTFILEMSG:
								{
									bool				retval=false;
									QFile			file(buffer.mText);
									DocumentClass	*doc=this->getDocumentForTab(-1);
									QTextCursor		cursor;
									QString			content;
									if(doc!=NULL)
										{
											cursor=doc->textCursor();

											retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
											if(retval==true)
												{
													content=QString::fromUtf8(file.readAll());
													cursor.beginEditBlock();
														cursor.insertText(content);
														doc->highlighter->rehighlight();
														doc->dirty=true;
													cursor.endEditBlock();
													doc->state=DIRTYTAB;
													doc->setTabColourType(DIRTYTAB);
													this->setToolbarSensitive();
												}
										}
								}
								break;
							case PRINTFILESMSG://TODO//just print with defaults//TODO//
								emit this->printMenuItem->triggered();
								break;
							case RUNTOOLMSG:
								this->clickMenu(this->toolsMenu,QString(buffer.mText));
								break;
							case ACTIVATEMENUBYLABELEDMSG:
								foreach(QAction *action,this->menuBar->actions())
									{
										if(action->menu())
											{
												this->clickMenu(action->menu(),QString(buffer.mText));
											}
									}
								break;
							case SENDPOSDATAMSG:
								{
									DocumentClass	*doc=this->getDocumentForTab(-1);
									QTextCursor		cursor;
									if(doc!=NULL)
										{
											cursor=doc->textCursor();
											this->sendMessgage(QString("%1:%2:%3").arg(this->mainNotebook->currentIndex()).arg(doc->getCurrentLineNumber()).arg(cursor.positionInBlock()+1));
										}
									else
										this->sendMessgage("");
								}
								break;
							case SENDSELECTEDTEXTMSG:
								{
									DocumentClass	*doc=this->getDocumentForTab(-1);
									QTextCursor		cursor;
									if(doc!=NULL)
										{
											cursor=doc->textCursor();
											this->sendMessgage(cursor.selection().toPlainText());
										}
									else
										this->sendMessgage("");
								}
								break;
							case OPENFILEMSG:
								this->openFile(buffer.mText);
								break;
							case NEWFILEMSG:
								this->newFile();
								break;
							case SAVEFILEMSG:
								this->notDoneYet("SAVEFILEMSG not yet implemented");
								break;
							case QUITAPPMSG:
								this->shutDownApp();
								break;
							case ACTIVATEAPPMSG:
								this->mainWindow->setWindowState((this->mainWindow->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
								this->mainWindow->raise();
								this->application->setActiveWindow(this->mainWindow);
								this->mainWindow->activateWindow();
								break;

							case SAVECURRENTSESSIONMSG:
								emit this->saveCurrentSessionMenuItem->triggered();
								break;
							case RESTORESESSIONMSG:
								for(int j=0;j<this->restoreSessionMenuItemsList.count();j++)
									{
										if(QString(buffer.mText).compare(this->restoreSessionMenuItemsList.at(j)->text())==0)
											emit this->restoreSessionMenuItemsList.at(j)->triggered();
									}
								break;

							case SENDSESSIONNAMEMSG:
								this->sendMessgage(this->sessionNames.value(currentSessionNumber));
								break;
						}

					if((buffer.mType & CONTINUEMSG)==CONTINUEMSG)
						{
							buffer.mText[0]=0;
							buffer.mType=CONTINUEMSG;
							msgsnd(this->queueID,&buffer,0,0);
						}
				}
		}
}

void KKEditClass::doDoubleClickPrefs(QListWidgetItem *item)
{
	const char		*data="NOSXCPURFG9ADLsEQBW";
	int				row=listWidget->currentRow();
	QListWidgetItem	*titem=this->listWidget->takeItem(row);

	for(unsigned int j=0;j<strlen(data);j++)
		{
			if(item->type()==data[j])
				{
					this->tool[j]->setEnabled(true);
					break;
				}
		}
	delete titem;
}

void KKEditClass::setPreferences(void)
{
	this->prefsIndent=qobject_cast<QCheckBox*>(this->prefsWidgets[AUTOINDENT])->checkState();
	this->prefsLineWrap=qobject_cast<QCheckBox*>(this->prefsWidgets[WRAP])->checkState();
	this->prefsShowLineNumbers=qobject_cast<QCheckBox*>(this->prefsWidgets[SHOWNUMS])->checkState();
	this->prefsHighLightline=qobject_cast<QCheckBox*>(this->prefsWidgets[HIGHLIGHT])->checkState();
	this->prefsDepth=qobject_cast<QSpinBox*>(this->prefsIntWidgets[MAXFUNCDEPTH])->value();
	this->prefsTabWidth=qobject_cast<QSpinBox*>(this->prefsIntWidgets[TABWIDTH])->value();
	this->prefsMaxTabChars=qobject_cast<QSpinBox*>(this->prefsIntWidgets[MAXTABCHARS])->value();
	this->maxFRHistory=qobject_cast<QSpinBox*>(this->prefsIntWidgets[MAXHISTORY])->value();
	this->prefsMaxMenuChars=qobject_cast<QSpinBox*>(this->prefsIntWidgets[MENUWIDTH])->value();
	this->prefsFunctionMenuLayout=qobject_cast<QComboBox*>(this->prefsOtherWidgets[FUNCTIONCOMBO])->currentIndex();
	this->prefsSyntaxHilighting=qobject_cast<QCheckBox*>(this->prefsWidgets[SYNTAXHILITE])->checkState();
	this->prefsAutoShowCompletions=qobject_cast<QCheckBox*>(this->prefsWidgets[AUTOSHOW])->checkState();
	this->prefsNoOpenduplicate=qobject_cast<QCheckBox*>(this->prefsWidgets[NODUPLICATE])->checkState();
	this->prefsNoWarnings=qobject_cast<QCheckBox*>(this->prefsWidgets[NOWARN])->checkState();
	this->prefsNagScreen=qobject_cast<QCheckBox*>(this->prefsWidgets[BEKIND])->checkState();
	this->recentFiles->maxFiles=qobject_cast<QSpinBox*>(this->prefsIntWidgets[MAXRECENTS])->value();
	this->autoShowMinChars=qobject_cast<QSpinBox*>(this->prefsIntWidgets[COMPLETIONSIZE])->value();
	this->prefsMsgTimer=qobject_cast<QSpinBox*>(this->prefsIntWidgets[MSGCHECKTIME])->value();
	this->prefStyleName=qobject_cast<QComboBox*>(this->prefsOtherWidgets[THEMECOMBO])->currentText();
	this->prefStyleNameHold=this->prefStyleName;

	this->checkMessages->setInterval(this->prefsMsgTimer);
	this->recentFiles->updateRecents();

	this->onExitSaveSession=qobject_cast<QCheckBox*>(this->prefsWidgets[AUTOSAVE])->checkState();

//menu style
	this->prefsMenuStyleString=qobject_cast<QLineEdit*>(prefsOtherWidgets[PREFSMENUSTYLE])->text();
//print command
	this->prefsPrintCommand=qobject_cast<QLineEdit*>(prefsOtherWidgets[PREFSPRINTCOMMAND])->text();
//term command
	this->prefsTerminalCommand=qobject_cast<QLineEdit*>(prefsOtherWidgets[PREFSTERMCOMMAND])->text();
//root command
	this->prefsRootCommand=qobject_cast<QLineEdit*>(prefsOtherWidgets[PREFSROOTCOMMAND])->text();
//qt doc dir
	this->prefsQtDocDir=qobject_cast<QLineEdit*>(prefsOtherWidgets[PREFSQTDOCDIR])->text();

	this->prefsWindow->hide();

	this->prefsToolBarLayout="";
	for (int j=0;j<this->listWidget->count();j++)
		this->prefsToolBarLayout+=this->listWidget->item(j)->type();

	this->setUpToolBar();
//show line numbers
	this->lineNumbersVisible=this->prefsShowLineNumbers;
	this->toggleLineNumbersMenuItem->setChecked(this->lineNumbersVisible);
//wrap lines
	this->wrapLine=this->prefsLineWrap;
	this->toggleWrapLineMenuItem->setChecked(this->wrapLine);
//hilite current line
	this->hiliteCurrentLine=this->prefsHighLightline;
	this->toggleHiliteCurrentLineMenuItem->setChecked(this->hiliteCurrentLine);
//syntax hiliting
	this->showHighLighting=this->prefsSyntaxHilighting;
	this->toggleSyntaxHilightingMenuItem->setChecked(this->showHighLighting);
//completions
	this->showCompletions=this->prefsAutoShowCompletions;
	this->toggleCompletionsMenuItem->setChecked(this->showCompletions);
	this->theme->loadTheme(this->prefStyleNameHold);
	this->resetAllFilePrefs();
	this->writeExitData();
	this->setAppShortcuts();
	this->application->setStyleSheet(this->prefsMenuStyleString);

	switchPage(this->mainNotebook->currentIndex());
}

void KKEditClass::setBMColour(void)
{
	QColor	tmpcolour;
	tmpcolour=QColorDialog::getColor(QColor(this->prefsBookmarkHiLiteColor),0,"Select Color",QColorDialog::ShowAlphaChannel);
	if(tmpcolour.isValid())
		{
			this->prefsOtherWidgets[BMHIGHLIGHTCOLOUR]->setProperty("palette",QPalette(tmpcolour));
			this->prefsOtherWidgets[BMHIGHLIGHTCOLOUR]->setProperty("autoFillBackground",true);
			this->prefsOtherWidgets[BMHIGHLIGHTCOLOUR]->setProperty("text",tmpcolour.name());
			this->prefsBookmarkHiLiteColor=tmpcolour;
		}
}

void KKEditClass::setLineColour(void)
{
	QColor	tmpcolour;
	tmpcolour=QColorDialog::getColor(QColor(this->prefsHiLiteLineColor),0,"Select Color",QColorDialog::ShowAlphaChannel);
	if(tmpcolour.isValid())
		{
			this->prefsOtherWidgets[CURRENTLINECOLOUR]->setProperty("palette",QPalette(tmpcolour));
			this->prefsOtherWidgets[CURRENTLINECOLOUR]->setProperty("autoFillBackground",true);
			this->prefsOtherWidgets[CURRENTLINECOLOUR]->setProperty("text",tmpcolour.name());
			this->prefsHiLiteLineColor=tmpcolour;
		}
}

void KKEditClass::setFont(void)
{
	bool	ok;
	QFont	tmpfont;

	tmpfont=QFontDialog::getFont(&ok,this->prefsDocumentFont,this->mainWindow);
	if(ok==true)
		{
			this->prefsDocumentFont=tmpfont;
			qobject_cast<QLabel*>(this->prefsOtherWidgets[PREFSCURRENTFONT])->setText(QString("%1 %2").arg(tmpfont.family()).arg(tmpfont.pointSize()));
		}
}

void KKEditClass::addToToolBar(void)
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());

	this->prefsToolBarLayout=(char*)qobject_cast<MenuItemClass*>(mc)->objectName().constData();
	this->populateStore();
}

void KKEditClass::doTabBarContextMenu(void)
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());
	QClipboard		*clipboard=this->application->clipboard();
	DocumentClass	*doc;

	doc=this->getDocumentForTab((mc->getMenuID() & 0xff));
	switch(mc->getMenuID() & 0xfff00)
		{
			case COPYFOLDERPATH:
				clipboard->setText(doc->getDirPath());
				break;
			case COPYFILEPATH:
				clipboard->setText(doc->getFilePath());
				break;
			case COPYFILENAME:
				clipboard->setText(doc->getFileName());
				break;
			case SPELLCHECKDOC:
				this->checkDoc(doc);
				break;
			case OPENFROMHERE:
				this->openFile(QString("%1/%2").arg(doc->getDirPath()).arg(mc->text()));
				break;
			case HIDETAB:
				this->setTabVisibilty(mc->getMenuID() & 0xff,false);
				break;
			case LOCKCONTENTS:
				{
					doc->setReadOnly(!doc->isReadOnly());
					if(doc->isReadOnly()==true)
						doc->setTabColourType(LOCKEDTAB);
					else
						{
							doc->setTabColourType(doc->state);
						}
				}
				break;
		}
	this->setToolbarSensitive();
}

void KKEditClass::doOddMenuItems(void)
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());
#ifdef _ASPELL_
	DocumentClass	*doc=this->getDocumentForTab(-1);
#endif

		switch(mc->getMenuID())
			{
				case SPELLCHECKMENUITEM:
#ifdef _ASPELL_
					if(doc->textCursor().selectedText().isEmpty()==false)
						{
							this->returnWord=false;
							this->setUpSpellGUI(doc->textCursor().selectedText(),doc);
						}
#endif
					break;
			}
}

void KKEditClass::doOddButtons(void)
{
#ifdef _ASPELL_
	DocumentClass	*doc=this->getDocumentForTab(-1);
#endif
	bool			saveas=false;

	switch(sender()->objectName().toInt())
		{
			case TOOLSEDIT:
				{
					QFile	file;
					file.setFileName(QString("%1/%2").arg(this->toolsFolder).arg(this->toolsWindow->findChild<QLineEdit*>(TOOLCOMMAND)->text()));
					if(file.exists())
						this->openFile(file.fileName());
					else
						{
							this->application->clipboard()->setText(this->toolsWindow->findChild<QLineEdit*>(TOOLCOMMAND)->text());
						}
				}
				this->toolsWindow->hide();
				break;
			case TOOLSSAVEAS:
				saveas=true;
			case TOOLSSAVE:
				{
					int			flags=0;
					QFile		file;
					bool			retval;
					QPushButton	*savebutton;
	
					if(saveas==false)
						file.setFileName(this->toolSelect->currentData().toString());
					else
						file.setFileName(QString("%1/%2").arg(this->toolsFolder).arg(this->toolsWindow->findChild<QLineEdit*>(TOOLNAME)->text()));

					retval=file.open(QIODevice::Text | QIODevice::WriteOnly);
					if(retval==true)
						{
							QTextStream(&file) << TOOLALWAYSINPOPUP << "\t" << this->toolsWindow->findChild<QCheckBox*>(TOOLALWAYSINPOPUP)->isChecked() << Qt::endl;
							QTextStream(&file) << TOOLCLEAROP << "\t" << this->toolsWindow->findChild<QCheckBox*>(TOOLCLEAROP)->isChecked() << Qt::endl;
							QTextStream(&file) << TOOLCOMMAND << "\t" << this->toolsWindow->findChild<QLineEdit*>(TOOLCOMMAND)->text() << Qt::endl;
							QTextStream(&file) << TOOLCOMMENT << "\t" << this->toolsWindow->findChild<QLineEdit*>(TOOLCOMMENT)->text() << Qt::endl;

							if(this->toolsWindow->findChild<QRadioButton*>(TOOLPASTEOUT)->isChecked()==true)
								flags=TOOL_PASTE_OP;
							if(this->toolsWindow->findChild<QRadioButton*>(TOOLREPLACEALL)->isChecked()==true)
								flags=TOOL_REPLACE_OP;
							if(this->toolsWindow->findChild<QRadioButton*>(TOOLVIEWOUT)->isChecked()==true)
								flags=TOOL_VIEW_OP;
							if(this->toolsWindow->findChild<QCheckBox*>(TOOLRUNSYNC)->isChecked()==false)
								flags|=TOOL_ASYNC;
							if(this->toolsWindow->findChild<QCheckBox*>(TOOLSHOWDOC)->isChecked()==true)
								flags|=TOOL_SHOW_DOC;

							QTextStream(&file) << TOOLFLAGS << "\t" << flags << Qt::endl;
							QTextStream(&file) << TOOLSHOWINPOPUP << "\t" << this->toolsWindow->findChild<QCheckBox*>(TOOLSHOWINPOPUP)->isChecked() << Qt::endl;
							QTextStream(&file) << TOOLRUNINTERM << "\t" << this->toolsWindow->findChild<QCheckBox*>(TOOLRUNINTERM)->isChecked() << Qt::endl;
							QTextStream(&file) << TOOLNAME << "\t" << this->toolsWindow->findChild<QLineEdit*>(TOOLNAME)->text() << Qt::endl;
							QTextStream(&file) << TOOLRUNASROOT << "\t" << this->toolsWindow->findChild<QCheckBox*>(TOOLRUNASROOT)->isChecked() << Qt::endl;
							QTextStream(&file) << TOOLKEY << "\t" << this->toolsWindow->findChild<QLineEdit*>(TOOLKEY)->text() << Qt::endl;
							QTextStream(&file) << TOOLUSEPOLE << "\t" << this->toolsWindow->findChild<QCheckBox*>(TOOLUSEPOLE)->isChecked() << Qt::endl;

							file.close();

							flags=this->toolSelect->currentIndex();
			 				this->rebuildToolsMenu();
							if(saveas==true)
								flags=this->toolSelect->findText(this->toolsWindow->findChild<QLineEdit*>(TOOLNAME)->text());
			 				this->toolSelect->setCurrentIndex(flags);
							this->runNoOutput(QString("echo quit>\"%1/tools\"").arg(this->tmpFolderName));
							savebutton=this->toolsWindow->findChild<QPushButton*>(QString("%1").arg(TOOLSSAVE));
							savebutton->setEnabled(true);
						}
				}
				break;
			case TOOLSDELETE:
				{
					QFile		file;
					file.setFileName(this->toolSelect->currentData().toString());
					QFileInfo	fileinfo(file);
					if(this->yesNoDialog("Deleting "+fileinfo.fileName(),"This is not undoable, continue?")!=QMessageBox::Yes)
						return;

					file.remove();
					this->rebuildToolsMenu();
					this->runNoOutput(QString("echo quit>\"%1/tools\"").arg(this->tmpFolderName));
				}
				break;
			case TOOLSCANCEL:
				this->toolsWindow->hide();
				break;
#ifdef _ASPELL_
			case APPLYWORDBUTTON:
				this->spellCheckGUI->hide();
				if(this->returnWord==false)
					{
						doc->textCursor().removeSelectedText();
						doc->textCursor().insertText(this->wordDropBox->currentText());
					}
				else
					this->goodWord=this->wordDropBox->currentText();
				this->returnWord=false;
				break;
			case IGNOREWORDBUTTON:
				if(this->returnWord==false)
					aspell_speller_add_to_session(this->spellChecker,doc->textCursor().selectedText().toStdString().c_str(),-1);
				else
					aspell_speller_add_to_session(this->spellChecker,this->badWord.toStdString().c_str(),-1);
				this->spellCheckGUI->hide();
				this->returnWord=false;
				break;
			case ADDWORDBUTTON:
				aspell_speller_add_to_personal(this->spellChecker,this->badWord.toStdString().c_str(),-1);
				aspell_speller_save_all_word_lists(this->spellChecker);
				this->spellCheckGUI->hide();
				break;
			case CANCELSPELLCHECK:
				this->cancelCheck=true;
				this->spellCheckGUI->hide();
				break;
#endif
			case CANCELPREFS:
				this->prefsWindow->hide();
				this->readConfigs();
				this->resetAllFilePrefs();
				this->switchPage(this->mainNotebook->currentIndex());
				break;
			case DOLIVESEARCH:
				this->doLiveSearch(this->liveSearchWidget->text());
				break;
			case DOAPISEARCH:
				this->goToDefinition(this->findDefWidget->text());
				break;
			case DOQT5SEARCH:
				this->searchAPIDocs(this->findQtApiWidget->text(),1);
				break; 
			case DOGTKSEARCH:
				this->searchAPIDocs(this->findGtkApiWidget->text(),0);
				break;
			case DOCVIEWERGOHOME:
				this->showWebPage("",this->htmlURI);
				break;
			default:
				break;
		}
}

void KKEditClass::docViewLinkTrap(const QUrl url)
{
#ifdef _BUILDDOCVIEWER_
	Qt::KeyboardModifiers key=QGuiApplication::keyboardModifiers();
	if(key!=Qt::ControlModifier)
		this->webView->load(url);
	else
		{
			QString	str=url.toString();
			QString	finalstring;
			int		linenum=0;
			int		stringcnt=0;

			str.remove(QRegularExpression("file:\\/\\/"));
			str.remove("_source.html");
			str.remove(QRegularExpression("\\/html"));
			str.remove(QRegularExpression("\\.html$"));
			linenum=QRegularExpression("#l([[:digit:]]*)").match(str).captured(1).toInt();
			str.remove(QRegularExpression("#l[[:digit:]]*"));

			while(stringcnt<str.length())
				{
					if(str.at(stringcnt)=='_')
						{
							stringcnt++;
							if(str.at(stringcnt).isLetter())
								{
									finalstring+=str.at(stringcnt++).toUpper();
									continue;
								}
							switch(str.at(stringcnt).unicode())
								{
									case '_':
										finalstring+="_";
										break;
									case '8':
										finalstring+=".";
									case '\n':
										break;
								}
						}
					else
						{
							finalstring+=str.at(stringcnt);
						}
					stringcnt++;
					}
					
			if(this->openFile(finalstring,linenum)==true)
				{
					return;
				}
			else
				{
					QString datafile=QRegularExpression("file://(.*\\.html)#.*$").match(url.toString()).captured(1);
					QString lnk=QRegularExpression("file://(.*)\\.html#(.*)$").match(url.toString()).captured(2);
					if(datafile.isEmpty()==false)
						{
							str=this->runPipeAndCapture(QString("cat %1|sed -n 's|^.*%2\">\\(.*\\)</a>.*$|\\1|p'|head -n1").arg(datafile).arg(lnk)).remove("\n");
							if(goToDefinition(str)==true)
								return;	
						}

					if(QRegularExpression(".*/(struct)(.*)$").match(str).captured(1).compare("struct")==0)
						{
							if(goToDefinition(QRegularExpression(".*/(struct)(.*)$").match(finalstring).captured(2))==true)
								return;
						}

					if(QRegularExpression(".*/(class)(.*)$").match(str).captured(1).compare("class")==0)
						{
							if(goToDefinition(QRegularExpression(".*/(class)(.*)$").match(finalstring).captured(2))==true)
								return;
						}
				}
		}
#endif
}

void KKEditClass::fileChangedOnDisk(const QString &path)
{
	if(this->fileWatch->files().contains(path)==true)
		{
			foreach(DocumentClass *doc,this->pages)
				{
					if(doc!=NULL)
						{
							if(doc->getFilePath().compare(path)==0)
								{
									if(doc->fromMe==false)
										{
											doc->fromMe=true;
											if(this->prefsNoWarnings==false)
												{
													QMessageBox msgBox;
													int			retval;

													msgBox.setIcon(QMessageBox::Warning);
													msgBox.setText(doc->getFilePath()+" has been modified on disk");
													msgBox.setInformativeText("Do you want to refresh document from disk?");
													msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Ignore);
													msgBox.setDefaultButton(QMessageBox::Apply);
													retval=msgBox.exec();
													if(retval==QMessageBox::Apply)
														{
															doc->refreshFromDisk();
															doc->modifiedOnDisk=false;
															doc->dirty=false;
															doc->fromMe=false;
															doc->state=NORMALTAB;
														}
													else
														{
															doc->modifiedOnDisk=true;
															doc->dirty=true;
															this->setToolbarSensitive();
															doc->state=IGNORECHANGEDONDISKTAB;
														}
												}
											else
												{
													doc->refreshFromDisk();
													doc->fromMe=false;
													doc->modifiedOnDisk=false;
													doc->dirty=false;
													doc->state=CHANGEDONDISKTAB;
												}
										}
									doc->setTabColourType(doc->state);
									doc->fromMe=false;
									continue;
								}
						}
				}
		}
}

void KKEditClass::doAppShortCuts(void)
{
	QShortcut		*sc=qobject_cast<QShortcut*>(sender());
	DocumentClass	*doc=this->getDocumentForTab(-1);
	QString			txt;
	QTextCursor		cursor;
	int				anc;
	//bool				retval=true;

	if(doc==NULL)
		return;

	if(sc->objectName().toInt()==HIDETABSHORTCUT)
		{
			this->setTabVisibilty(this->mainNotebook->currentIndex(),false);
			return;
		}

	cursor=doc->textCursor();
	cursor.beginEditBlock();
	switch(sc->objectName().toInt())
		{
			case DELETELINESHORTCUT:
				cursor.select(QTextCursor::LineUnderCursor);
				cursor.removeSelectedText();
				cursor.deleteChar();
				break;
			case DELETETOEOLSHORTCUT:
				cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::KeepAnchor);
				cursor.removeSelectedText();
				break;
			case DELETETOSOLSHORTCUT:
				cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::KeepAnchor);
				cursor.removeSelectedText();
				break;
			case SELECTWORDSHORTCUT:
				cursor.select(QTextCursor::WordUnderCursor);
				doc->setTextCursor(cursor);
				break;
			case DELETEWORDSHORTCUT:
				if(cursor.hasSelection()==false)
					cursor.select(QTextCursor::WordUnderCursor);
				cursor.removeSelectedText();
				break;
			case DUPLICATELINESHORTCUT:
				anc=cursor.anchor();
				cursor.select(QTextCursor::BlockUnderCursor);
				txt=cursor.selectedText();
				if(txt.isEmpty()==true)
					txt='\n';
				cursor.movePosition(QTextCursor::EndOfLine,QTextCursor::MoveAnchor);
				cursor.insertText(txt);
				cursor.setPosition(anc);
				doc->setTextCursor(cursor);
				emit doc->cursorPositionChanged();
				break;
			case SELECTLINESHORTCUT:
				cursor.select(QTextCursor::LineUnderCursor);
				doc->setTextCursor(cursor);
				break;
			case MOVELINEUPSHORTCUT:
				anc=cursor.positionInBlock();
				doc->moveCursor(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
				cursor.select(QTextCursor::LineUnderCursor);
				txt=cursor.selectedText();
				cursor.removeSelectedText();
				cursor.deleteChar();
				doc->moveCursor(QTextCursor::PreviousBlock,QTextCursor::MoveAnchor);
				cursor=doc->textCursor();
				cursor.insertText(txt+"\n");
				doc->setTextCursor(cursor);
				doc->moveCursor(QTextCursor::Up,QTextCursor::MoveAnchor);
				cursor=doc->textCursor();
				cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
				cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,anc);
				doc->setTextCursor(cursor);
				break;
			case MOVELINEDOWNSHORTCUT:
				anc=cursor.positionInBlock();
				doc->moveCursor(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
				cursor.select(QTextCursor::LineUnderCursor);
				txt=cursor.selectedText();
				cursor.removeSelectedText();
				cursor.deleteChar();
				doc->moveCursor(QTextCursor::NextBlock,QTextCursor::MoveAnchor);
				cursor=doc->textCursor();
				cursor.insertText(txt+"\n");
				doc->setTextCursor(cursor);
				doc->moveCursor(QTextCursor::Up,QTextCursor::MoveAnchor);
				cursor=doc->textCursor();
				cursor.movePosition(QTextCursor::StartOfLine,QTextCursor::MoveAnchor);
				cursor.movePosition(QTextCursor::Right,QTextCursor::MoveAnchor,anc);
				doc->setTextCursor(cursor);
				break;
			case MOVESELECTIONUPSHORTCUT:
				txt=cursor.selectedText();
				cursor.removeSelectedText();
				cursor.movePosition(QTextCursor::PreviousBlock,QTextCursor::MoveAnchor);
				anc=cursor.anchor();
				cursor.insertText(txt);
				cursor.setPosition(anc);
				cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor,txt.length());
				doc->setTextCursor(cursor);
				emit doc->cursorPositionChanged();
				break;
			case MOVESELECTIONDOWNSHORTCUT:
				txt=cursor.selectedText();
				cursor.removeSelectedText();
				cursor.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor);
				anc=cursor.anchor();
				cursor.insertText(txt);
				cursor.setPosition(anc);
				cursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor,txt.length());
				doc->setTextCursor(cursor);
				emit doc->cursorPositionChanged();
				break;
			case FORCESHOWCOMPLETE:
				if(this->completer)
					{
						QRect	cr;
						QString	completionPrefix;

						completionPrefix=doc->textUnderCursor();
						this->completer->setCompletionPrefix(completionPrefix);
						cr=doc->cursorRect();
						cr.setWidth(this->completer->popup()->sizeHintForColumn(0)+this->completer->popup()->verticalScrollBar()->sizeHint().width());
			  		 	if(this->completer->widget()==doc)
							this->completer->complete(cr);
					}

			break;
		}
	cursor.endEditBlock();
}
