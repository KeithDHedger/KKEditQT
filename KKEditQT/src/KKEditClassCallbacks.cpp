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

			retval=file.open(QIODevice::Text | QIODevice::WriteOnly);
			if(retval==true)
				{
					int		linenumber=10;
					bool		visible=true;
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
			retval=file.open(QIODevice::Text | QIODevice::ReadOnly);
			if(retval==true)
				{
					this->showBarberPole("Restore Session","Please Wait","Cancel",QString("%1/session").arg(this->tmpFolderName));
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
							this->runPipe(QString("echo \"Opening %1 ...\">\"%2/session\" &").arg(filename.trimmed()).arg(this->tmpFolderName));
							//this->openFile(filename);
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
					this->currentSessionNumber=sessionnumber;
					file.close();
					this->runPipe(QString("echo quit>\"%1/session\"").arg(this->tmpFolderName));
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
//TODO//rebuild func menu
	for(int j=this->mainNotebook->count()-1;j>-1;j--)
		{
			if(this->mainNotebook->isTabVisible(j)==true)
				{
					this->switchPage(j);
					break;
				}
		}
	//this->switchPage(0);
	//this->switchPage(this->mainNotebook->count()-1);
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
	QFile			file;
	QStringList		sl;
	int				holdindex;

	switch(mc->getMenuID())
		{
			 case MANAGETOOLSMENUITEM:
			 	this->toolsWindow->show();
			 	break;

			 default:
			 	sl=this->verifyTool(mc->getMenuString());
			 	if(sl.isEmpty()==true)
			 		return;

				if(sl.at(TOOL_COMMAND).section(TOOLCOMMAND,1,1).trimmed().isEmpty()==false)
					{
						QString str=sl.at(TOOL_COMMAND).section(TOOLCOMMAND,1,1).trimmed();
						if(document!=NULL)
							{
								QString filelist;
								for(int j=0;j<this->pages.count();j++)
									{
										if(this->pages[j]!=NULL)
											filelist+=this->pages[j]->getFilePath()+";";
									}
								setenv("KKEDIT_FILE_LIST",filelist.toStdString().c_str(),1);
								//%d doc folder
								setenv("KKEDIT_CURRENTDIR",document->getDirPath().toStdString().c_str(),1);
								str.replace("%d",document->getDirPath());
								//%f doc filepath
								setenv("KKEDIT_CURRENTFILE",document->getFilePath().toStdString().c_str(),1);
								str.replace("%f",document->getFilePath());
								//%t selected text
								setenv("KKEDIT_SELECTION",document->textCursor().selectedText().replace(QRegularExpression("\u2029|\\r\\n|\\r"),"\n").toStdString().c_str(),1);
								str.replace("%t",document->textCursor().selectedText());
								//%m
								setenv("KKEDIT_MIMETYPE",document->mimeType.toStdString().c_str(),1);
								str.replace("%m",document->mimeType);
							}

								//%h html file
								setenv("KKEDIT_HTMLFILE",this->htmlFile.toStdString().c_str(),1);
								str.replace("%h",this->htmlFile);
								//%i
								setenv("KKEDIT_DATADIR","TODO",1);
								str.replace("%i","TODO");

								int trm=sl.indexOf(QRegularExpression(QString(".*%1.*").arg(TOOLRUNINTERM)));//todo//
								//run in term
								if(sl.at(trm).section(TOOLRUNINTERM,1,1).trimmed().toInt()==1)
									{
										str=this->prefsTerminalCommand + " " + str;
										str=QString("cd %1;%2").arg(this->toolsFolder).arg(str);
										runPipe(str);
										return;
									}
								
								if(((sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_PASTE_OP)==TOOL_PASTE_OP) && (document!=NULL))
									{
										str=QString("cd %1;%2").arg(this->toolsFolder).arg(str);
										document->textCursor().beginEditBlock();
											document->textCursor().removeSelectedText();
											document->textCursor().insertText(runPipeAndCapture(str));
										document->textCursor().endEditBlock();
										return;
									}

								if(((sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_REPLACE_OP)==TOOL_REPLACE_OP) && (document!=NULL))
									{
										str=QString("cd %1;%2").arg(this->toolsFolder).arg(str);
										document->textCursor().beginEditBlock();
											document->setPlainText(runPipeAndCapture(str));
										document->textCursor().endEditBlock();
										return;
									}

								if((sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_VIEW_OP)==TOOL_VIEW_OP)//TODO//
									{
										str=QString("cd %1;%2").arg(this->toolsFolder).arg(str);
										str=runPipeAndCapture(str);
										if(sl.at(TOOL_CLEAR_VIEW).section(TOOLCLEAROP,1,1).toInt()==0)
											{
												QTextCursor tc=this->toolsOPText->textCursor();
												tc.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
												this->toolsOPText->setTextCursor(tc);
												this->toolsOPText->insertPlainText(str);
											}
										else
											{
												this->toolsOPText->setPlainText(str);
											}
										this->toolOutputWindow->setWindowTitle(sl.at(TOOL_NAME).section(TOOLNAME,1,1));
										this->toolOutputWindow->show();
										return;
									}

								if((sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_ASYNC)==TOOL_ASYNC)
									{
										str=QString("cd %1;%2 &").arg(this->toolsFolder).arg(str);
										runPipe(str);
										return;
									}
	
								if((sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_SHOW_DOC)==TOOL_SHOW_DOC)
									{
										str=QString("cd %1;%2").arg(this->toolsFolder).arg(str);
										runPipe(str);
										this->showWebPage(sl.at(TOOL_NAME).section(TOOLNAME,1,1).trimmed(),"file://" + this->htmlFile);
										return;
									}

								if((sl.at(TOOL_FLAGS).section(TOOLFLAGS,1,1).trimmed().toInt() & TOOL_ASYNC)==0)
									{
										str=QString("cd %1;%2").arg(this->toolsFolder).arg(str);
										runPipe(str);
										return;
									}
							}
			 	break;
		}
}

void KKEditClass::doHelpMenuItems()
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());
	DocumentClass	*document=this->getDocumentForTab(-1);

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
	DocumentClass	*document=this->getDocumentForTab(-1);

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
	DocumentClass	*document=this->getDocumentForTab(-1);

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
					this->toggleToolWindowMenuItem->setText("Hide Tool Output");
				else
					this->toggleToolWindowMenuItem->setText("Show Tool Output");
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
			case FINDMENUITEM:
				this->findReplaceDialog->show();
				break;
			case FINDNEXTMENUITEM:
				doFindReplace(FINDNEXT);
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
	bool			retval;

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
				this->saveAllFiles();
				break;
			case PRINTMENUITEM:
				this->printDocument();
				break;
			case CLOSEMENUITEM:
				this->closingAllTabs=false;
				retval=this->closeTab(-1);
				break;
			case CLOSEALLMENUITEM:
				this->closeAllTabs();
				break;
			case REVERTMENUITEM:
				this->reloadDocument();
				break;
			case QUITMENUITEM:
				this->shutDownApp();
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
}

void KKEditClass::doTimer(void)
{
	int				retcode=0;
	msgStruct		buffer;
	DocumentClass	*doc;
#ifdef _BUILDDOCVIEWER_
	this->setDocMenu();
#endif
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
								strcpy(buffer.mText,this->currentURL.toStdString().c_str());
								buffer.mType=SENDMSG;
								if((msgsnd(queueID,&buffer,strlen(buffer.mText)+1,0))==-1)
									{
										fprintf(stderr,"Can't send message :(\n");
										exit(NOSENDMSG);
									}
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
									long	tabnum=strtol(buffer.mText,NULL,0);
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
//													return;
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
								this->notDoneYet("BOOKMARKMSG not yet implemented");
								qDebug() << "BOOKMARKMSG";
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
								qDebug() << "SETUSERMARKMSG";
								break;
							case UNSETUSERMARKMASG:
								this->notDoneYet("UNSETUSERMARKMASG not yet implemented");
								qDebug() << "UNSETUSERMARKMASG";
								break;
							case MOVETOMSG:
								this->notDoneYet("MOVETOMSG not yet implemented");
								qDebug() << "MOVETOMSG";
								break;
							case SELECTBETWEENMSG:
								this->notDoneYet("SELECTBETWEENMSG not yet implemented");
								qDebug() << "SELECTBETWEENMSG";
								break;
							case PASTEMSG:
								this->notDoneYet("PASTEMSG not yet implemented");
								qDebug() << "PASTEMSG";
								break;
							case COPYMSG:
								this->notDoneYet("COPYMSG not yet implemented");
								qDebug() << "COPYMSG";
								break;
							case CUTMSG:
								this->notDoneYet("CUTMSG not yet implemented");
								qDebug() << "CUTMSG";
								break;
							case INSERTTEXTMSG:
								this->notDoneYet("INSERTTEXTMSG not yet implemented");
								qDebug() << "INSERTTEXTMSG";
								break;
							case INSERTNLMSG:
								this->notDoneYet("INSERTNLMSG not yet implemented");
								qDebug() << "INSERTNLMSG";
								break;
							case INSERTFILEMSG:
								this->notDoneYet("INSERTFILEMSG not yet implemented");
								qDebug() << "INSERTFILEMSG";
								break;
							case PRINTFILESMSG:
								this->notDoneYet("PRINTFILESMSG not yet implemented");
								qDebug() << "PRINTFILESMSG";
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
								this->notDoneYet("SENDPOSDATAMSG not yet implemented");
								qDebug() << "SENDPOSDATAMSG";
								break;
							case SENDSELECTEDTEXTMSG:
								strcpy(buffer.mText,"test return message\n");
								buffer.mType=SENDMSG;
								if((msgsnd(queueID,&buffer,strlen(buffer.mText)+1,0))==-1)
									{
										fprintf(stderr,"Can't send message :(\n");
										exit(NOSENDMSG);
									}
								this->notDoneYet("SENDSELECTEDTEXTMSG not yet implemented");
								qDebug() << "SENDSELECTEDTEXTMSG";
								break;
							case OPENFILEMSG:
								this->openFile(buffer.mText);
								break;
							case SAVEFILEMSG:
								this->notDoneYet("SAVEFILEMSG not yet implemented");
								qDebug() << "SAVEFILEMSG";
								break;
							case QUITAPPMSG:
								this->shutDownApp();
								break;
							case ACTIVATEAPPMSG://TODO//minimized
								this->application->setActiveWindow(this->mainWindow);
								this->mainWindow->activateWindow();
								break;
							case RESTORESESSIONMSG:
								for(int j=0;j<this->restoreSessionMenuItemsList.count();j++)
									{
										if(QString(buffer.mText).compare(this->restoreSessionMenuItemsList.at(j)->text())==0)
											emit this->restoreSessionMenuItemsList.at(j)->triggered();
									}
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
	this->prefsMaxFuncChars=qobject_cast<QSpinBox*>(this->prefsIntWidgets[MENUWIDTH])->value();
	this->maxBMChars=qobject_cast<QSpinBox*>(this->prefsIntWidgets[MAXBMWIDTH])->value();
	this->prefsFunctionMenuLayout=qobject_cast<QComboBox*>(this->prefsOtherWidgets[FUNCTIONCOMBO])->currentIndex();
	this->prefsSyntaxHilighting=qobject_cast<QCheckBox*>(this->prefsWidgets[SYNTAXHILITE])->checkState();
	this->prefsAutoShowCompletions=qobject_cast<QCheckBox*>(this->prefsWidgets[AUTOSHOW])->checkState();
	this->prefsNoOpenduplicate=qobject_cast<QCheckBox*>(this->prefsWidgets[NODUPLICATE])->checkState();
	this->prefsNoWarnings=qobject_cast<QCheckBox*>(this->prefsWidgets[NOWARN])->checkState();
	this->prefsNagScreen=qobject_cast<QCheckBox*>(this->prefsWidgets[BEKIND])->checkState();
	this->recentFiles->maxFiles=qobject_cast<QSpinBox*>(this->prefsIntWidgets[MAXRECENTS])->value();
	this->autoShowMinChars=qobject_cast<QSpinBox*>(this->prefsIntWidgets[COMPLETIONSIZE])->value();
	this->recentFiles->updateRecents();

	this->onExitSaveSession=qobject_cast<QCheckBox*>(this->prefsWidgets[AUTOSAVE])->checkState();

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
	this->resetAllFilePrefs();
	this->writeExitData();
	this->setAppShortcuts();
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
	bool				retval;
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
					int	tabnum=this->mainNotebook->currentIndex();
					doc->setReadOnly(!doc->isReadOnly());
					if(doc->isReadOnly()==true)
						this->mainNotebook->tabBar()->setTabTextColor(tabnum,QColor(Qt::darkGreen));
					else
						{
							if(doc->dirty==true)
								this->mainNotebook->tabBar()->setTabTextColor(tabnum,QColor(Qt::red));
							else
								this->mainNotebook->tabBar()->setTabTextColor(tabnum,QColor(doc->highlighter->documentForeground));
						}
				}
				break;
		}
	this->setToolbarSensitive();
}

void KKEditClass::doOddMenuItems(void)
{
	MenuItemClass	*mc=qobject_cast<MenuItemClass*>(sender());
	bool			retval;
	QClipboard		*clipboard=this->application->clipboard();
	DocumentClass	*doc=this->getDocumentForTab(-1);

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
	DocumentClass	*doc=this->getDocumentForTab(-1);
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
							this->showBarberPole("Rebuild Tools Menu","Rebuilding tools menu, please wait ...","Cancel",QString("%1/tools").arg(this->tmpFolderName));
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
							this->runPipe(QString("echo quit>\"%1/tools\"").arg(this->tmpFolderName));

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

					this->showBarberPole("Rebuild Tools Menu","Rebuilding tools menu, please wait ...","Cancel",QString("%1/tools").arg(this->tmpFolderName));
					file.remove();
					this->rebuildToolsMenu();
					this->runPipe(QString("echo quit>\"%1/tools\"").arg(this->tmpFolderName));
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
					if(QRegularExpression(".*/(class)(.*)$").match(str).captured(1).compare("class")==0)
						{
							if(goToDefinition(QRegularExpression(".*/(class)(.*)$").match(finalstring).captured(2))==true)
								return;
						}
					if(QRegularExpression(".*/(struct)(.*)$").match(str).captured(1).compare("struct")==0)
						{
							if(goToDefinition(QRegularExpression(".*/(struct)(.*)$").match(finalstring).captured(2))==true)
								return;
						}
					QString datafile=QRegularExpression("file://(.*\\.html)#.*$").match(url.toString()).captured(1);
					QString lnk=QRegularExpression("file://(.*)\\.html#(.*)$").match(url.toString()).captured(2);

					str=this->runPipeAndCapture(QString("cat %1|sed -n 's|^.*%2\">\\(.*\\)</a>.*$|\\1|p'|head -n1").arg(datafile).arg(lnk)).remove("\n");;
					if(goToDefinition(str)==true)
						return;	
				}
		}
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
														}
												}
											else
												{
													doc->refreshFromDisk();
												}
										}
									else
										doc->fromMe=false;
								}
						}
				}
		}
}


