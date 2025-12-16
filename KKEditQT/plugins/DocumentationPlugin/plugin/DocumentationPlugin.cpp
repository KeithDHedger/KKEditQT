/*
 *
 * ©K. D. Hedger. Fri  5 Jul 11:15:45 BST 2024 keithdhedger@gmail.com

 * This file (DocumentationPlugin.cpp) is part of DocumentationPlugin.

 * DocumentationPlugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * DocumentationPlugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with DocumentationPlugin.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DocumentationPlugin.h"

QString DocumentationPlugin::runPipeAndCapture(QString command)
{
	QString	dump("");
	FILE		*fp=NULL;
	char		line[1024];
	bool		flip=false;

	fp=popen(command.toStdString().c_str(), "r");
	if(fp!=NULL)
		{
			while(fgets(line,1024,fp))
				{
					flip=!flip;
					if((flip==true) && (this->noStatusInfo==false))
						{
							QString text=QString("Adding %1").arg(QString(line).simplified());
							this->mainKKEditClass->statusBar->showMessage(text,STATUSBARTIMEOUT);
							this->mainKKEditClass->statusBar->repaint();
						}
					dump+=line;
				}
			pclose(fp);
		}
	return(dump);
}

void DocumentationPlugin::runAllSearchs(void)
{
	QTextCursor			tc;
	QString				comm;
	QVector<QStringList>	links;
	QFile				html(this->mainKKEditClass->htmlFile);

	if(this->doc==NULL)
		return;

	links.clear();
	this->noStatusInfo=false;			
	tc=this->doc->textCursor();
	if(tc.hasSelection()==true)
		{
			for(int j=0;j<this->resList.size();j+=SETTINGSCNT)
				{
					if((this->resList.at(j+SEARCHALL).compare("inall")!=0) || (this->resList.at(j+SHOWITEM).compare("show")!=0))
						continue;
					
					comm=QString("pushd %1/actions &>/dev/null;%2 %3 %4;popd &>/dev/null").arg(QFileInfo(this->plugPath).absolutePath()).arg(this->resList.at(j+1)).arg(tc.selectedText().trimmed()).arg(this->mainKKEditClass->htmlFile);
					QStringList	reslist;
					QString		results=this->runPipeAndCapture(comm);
					if(results.compare("Just Open\n")!=0)
						{
							reslist=results.split("\n",Qt::SkipEmptyParts);
							if(reslist.isEmpty()==false)
								links.push_back(reslist);
						}
				}

			if(html.open(QFile::WriteOnly|QFile::Truncate))
				{
					QTextStream out(&html);

					out << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">" << Qt::endl;
					out << "<html>" << Qt::endl;
					out << "<head>" << Qt::endl;
					out << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">" << Qt::endl;
					out << "</head>" << Qt::endl;
					out << "<body>" << Qt::endl;
					for(int outerlooop=0;outerlooop<links.size();outerlooop++)
						{
							out << "<span style=\"font-weight: bold;\">"<<links[outerlooop].at(0)<<"</span></font><br>" << Qt::endl;
							for(int loop=1;loop<links[outerlooop].size();loop+=2)
								{
									if(links[outerlooop].size()>loop+1)
										out << "<a href=\"" << links[outerlooop].at(loop) << "\">" << links[outerlooop].at(loop+1) << "</a><br>" << Qt::endl;
								}
							out <<"<br>"<< Qt::endl;
						}
					out << "</body>" << Qt::endl;
					out << "</html>" << Qt::endl;
					html.close();
					this->mainKKEditClass->htmlURI="file://"+this->mainKKEditClass->htmlFile;
					comm=QString("kkeditqtmsg -k %1 -c openindocview -d %2").arg(this->mainKKEditClass->sessionID).arg(this->mainKKEditClass->htmlFile);
					system(comm.toStdString().c_str());
				}
		}
}

void DocumentationPlugin::runSearch(QString command)
{
	QTextCursor	tc;

	if(this->doc==NULL)
		return;

	this->noStatusInfo=false;

	tc=this->doc->textCursor();

	if(tc.hasSelection()==true)
		{
			QString		comm=QString("pushd %1/actions &>/dev/null;%2 %3 %4;popd &>/dev/null").arg(QFileInfo(this->plugPath).absolutePath()).arg(command).arg(tc.selectedText().trimmed()).arg(this->mainKKEditClass->htmlFile);
			QFile		html(this->mainKKEditClass->htmlFile);
			QStringList	reslist;
			QString		results=this->runPipeAndCapture(comm);

			if(results.startsWith("Just Open\n")==true)
				{
					comm=QString("kkeditqtmsg -k %1 -c openindocview -d %2").arg(this->mainKKEditClass->sessionID).arg(this->mainKKEditClass->htmlFile);
					system(comm.toStdString().c_str());
					return;
				}

			reslist=results.split("\n",Qt::SkipEmptyParts);
			if(reslist.isEmpty()==false)
				{
					if(QString(reslist.at(0)).startsWith("Just Open\n")==true)
						{
							comm=QString("kkeditqtmsg -k %1 -c openindocview -d %2").arg(this->mainKKEditClass->sessionID).arg(this->mainKKEditClass->htmlFile);
							system(comm.toStdString().c_str());
							return;
						}

					if(reslist.size()==3)
						{
							comm=QString("kkeditqtmsg -k %1 -c openindocview -d %2").arg(this->mainKKEditClass->sessionID).arg(reslist.at(1));
							system(comm.toStdString().c_str());
							return;
						}

					if(html.open(QFile::WriteOnly|QFile::Truncate))
						{
							QTextStream out(&html);

							out << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">" << Qt::endl;
							out << "<html>" << Qt::endl;
							out << "<head>" << Qt::endl;
							out << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">" << Qt::endl;
							out << "</head>" << Qt::endl;
							out << "<body>" << Qt::endl;
							if(reslist.size()>0)
								out << "<span style=\"font-weight: bold;\">"<<reslist.at(0)<<"</span></font><br>" << Qt::endl;
							for(int loop=1;loop<reslist.size();loop+=2)
								{
									if(reslist.size()>loop+1)
										out << "<a href=\"" << reslist.at(loop) << "\">" << reslist.at(loop+1) << "</a><br>" << Qt::endl;
								}					
							out << "</body>" << Qt::endl;
							out << "</html>" << Qt::endl;
							html.close();
							this->mainKKEditClass->htmlURI="file://"+this->mainKKEditClass->htmlFile;
							comm=QString("kkeditqtmsg -k %1 -c openindocview -d %2").arg(this->mainKKEditClass->sessionID).arg(this->mainKKEditClass->htmlFile);
							system(comm.toStdString().c_str());
						}
				}
		}
}

miniPrefsReturnStruct DocumentationPlugin::miniPrefsDialog(QString prefsname,QStringList items,bool liveupdate)
{
	miniPrefsReturnStruct	prefs;
	QSettings				miniprefsname("KDHedger",prefsname);
	QWidget					*hbox;
	QVBoxLayout				*docvlayout=new QVBoxLayout;
	QHBoxLayout				*hlayout;
	QPushButton				*cancelbutton=new QPushButton("&Cancel");
	QPushButton				*okbutton=new QPushButton("&Apply");
	std::string				prefsentry;

	prefs.theDialog=new QDialog();

	QObject::connect(cancelbutton,&QPushButton::clicked,[this,prefs]()
		{
			prefs.theDialog->reject();
		});
	QObject::connect(okbutton,&QPushButton::clicked,[this,prefs]()
		{
			prefs.theDialog->accept();
		});

	for(int j=0;j<items.size();j++)
		{
			hbox=new QWidget;
			hlayout=new QHBoxLayout;
			hlayout->setContentsMargins(0,0,0,0);
			hbox->setLayout(hlayout);
			hlayout->addWidget(new QLabel(items.at(j)),0,Qt::AlignLeft);
			//prefsentry=LFSTK_UtilityClass::LFSTK_strStrip(items.at(j).toStdString());
			//prefsentry=LFSTK_UtilityClass::LFSTK_strReplaceAllChar(prefsentry," ","",true);
			prefsentry=items.at(j).toStdString();
			//prefsentry=LFSTK_UtilityClass::LFSTK_strReplaceAllChar(prefsentry," ","",true);

			prefs.boxes[j]=new QLineEdit(miniprefsname.value(prefsentry.c_str(),"").toString().simplified());	
			if(liveupdate==true)
				{
					QObject::connect(prefs.boxes[j],&QLineEdit::textEdited,[j,items,prefsname,prefs](const QString)
						{
							QSettings	miniprefsname("KDHedger",prefsname);
							//std::string	prefsentry=LFSTK_UtilityClass::LFSTK_strStrip(items.at(j).toStdString());
							std::string	prefsentry=items.at(j).toStdString();
							//prefsentry=LFSTK_UtilityClass::LFSTK_strReplaceAllChar(prefsentry," ","",true);
							miniprefsname.setValue(prefsentry.c_str(),prefs.boxes[j]->text());
						});
				}
			hlayout->addWidget(prefs.boxes[j],1,Qt::AlignRight);
			docvlayout->addWidget(hbox);
		}
	hbox=new QWidget;
	hlayout=new QHBoxLayout;
	hlayout->setContentsMargins(0,0,0,0);
	hbox->setLayout(hlayout);
	hlayout->addWidget(cancelbutton);
	hlayout->addStretch(0);
	hlayout->addWidget(okbutton);

	docvlayout->addWidget(hbox);

	prefs.theDialog->setLayout(docvlayout);
	return(prefs);
}

void DocumentationPlugin::showDocs(void)
{
	if(this->data!=NULL)
		{
			QString		comm;
			QDir::setCurrent(this->folderPath);
			comm=QString("kkeditqtmsg -k %1 -c openindocview -d %2").arg(this->mainKKEditClass->sessionID).arg(QString("'%1/%2'").arg(this->folderPath).arg("html/index.html"));
			system(comm.toStdString().c_str());		
		}
}


void DocumentationPlugin::buildDocset(void)
{
	if(this->data!=NULL)
		{
			QString					projectname;
			QString					versionbox;
			QString					holddir;
			QSettings				settings("KDHedger","DocumentationPlugin");
			QString					iconbox;
			QString					destdir;

			sqlite3					*DB;
			miniPrefsReturnStruct	myprefs;
			bool						gotdoxyfile=false;
			FILE						*fp;
			char						line[4096];
			int						res;

			holddir=QDir::currentPath();
			if(QFileInfo(QString("%1/Doxyfile").arg(QFileInfo(this->folderPath).absolutePath())).exists()==true)
				gotdoxyfile=true;
			else
				QProcess::execute("cp",QStringList()<<QString("%1/Doxyfile").arg(QFileInfo(this->plugPath).absolutePath())<<this->tempFolderPath);

			QDir::setCurrent(this->folderPath);

			if(gotdoxyfile==true)
				{
					projectname=this->runPipeAndCapture(QString("sed -n 's/PROJECT_NAME=\\(.*\\)/\\1/p' %1").arg(this->folderPath+"/Doxyfile")).simplified();
					versionbox=this->runPipeAndCapture(QString("sed -n 's/PROJECT_NUMBER=\\(.*\\)/\\1/p' %1").arg(this->folderPath+"/Doxyfile")).simplified();
				}
			else
				{
					projectname=settings.value("Project Name").toString();
					if(projectname.isEmpty()==true)
						projectname=this->runPipeAndCapture(QString("sed -n 's/PROJECT_NAME=\\(.*\\)/\\1/p' %1").arg(this->folderPath+"/Doxyfile")).simplified();
					versionbox=settings.value("Project Version").toString();
					if(versionbox.isEmpty()==true)
						versionbox=this->runPipeAndCapture(QString("sed -n 's/PROJECT_NUMBER=\\(.*\\)/\\1/p' %1").arg(this->folderPath+"/Doxyfile")).simplified();
				}

			destdir=settings.value("DestinationFolder",QString("%1/.local/share/Zeal/Zeal/docsets").arg(QDir::homePath())).toString();
			iconbox=settings.value("IconPath",QString("%1/LFSTux.png").arg(QFileInfo(this->plugPath).absolutePath())).toString();

			settings.sync();
			myprefs=this->miniPrefsDialog("DocumentationPlugin",QStringList()<<"Project Name"<<"Project Version"<<"Icon Path"<<"Destination Folder",true);
			myprefs.boxes[0]->setText(projectname.trimmed());
			myprefs.boxes[1]->setText(versionbox.trimmed());
			myprefs.boxes[2]->setText(iconbox.trimmed());
			myprefs.boxes[3]->setText(destdir.trimmed());
			res=myprefs.theDialog->exec();

			if(res==1)
				{
					this->runPipeAndCapture(QString("sed -i 's/^PROJECT_NAME=.*$/PROJECT_NAME=%1/;s/^PROJECT_NUMBER=.*$/PROJECT_NUMBER=%2/;s@^OUTPUT_DIRECTORY.*=.*$@OUTPUT_DIRECTORY=%3@;s/^GENERATE_DOCSET.*=.*$/GENERATE_DOCSET=YES/;s/^SEARCHENGINE.*=.*$/SEARCHENGINE=NO/;s/^DOT_IMAGE_FORMAT.*=.*$/DOT_IMAGE_FORMAT=png/' '%3/Doxyfile'").arg(myprefs.boxes[0]->text()).arg(myprefs.boxes[1]->text()).arg(this->tempFolderPath));

					this->showBarberPole("Building Docset","Please Wait","","0",QString("%1/progress").arg(this->tempFolderPath));
					fp=popen(QString("pushd '%1'&>/dev/null;doxygen '%2/Doxyfile';popd &>/dev/null").arg(this->folderPath).arg(this->tempFolderPath).toStdString().c_str(),"r");
					if(fp!=NULL)
						{
							while(fgets(line,4095,fp))
								{
									line[strlen(line)-1]=0;
									this->runPipeAndCapture(QString("echo -n \"%1\n0\" >\"%2/progress\"").arg(line).arg(this->tempFolderPath));
								}
							pclose(fp);
						}
					else
						{
							this->runPipeAndCapture(QString("echo -e \"quit\n100\">\"%1/progress\"").arg(this->tempFolderPath));
							return;
						}

					holddir=QDir::currentPath();
					QDir::setCurrent(this->tempFolderPath+"/html");
					QDir			d;
					int			exit=0;
					QString		sql;
					char			*messaggeError=NULL;
					QString		thename;
					QString		thetype;
					QString		thepath;
					QString		theanchor; 

					d.mkpath(myprefs.boxes[3]->text()+"/"+myprefs.boxes[0]->text()+".docset/Contents/Resources");
					exit=sqlite3_open(QString("%1/%2.docset/%3").arg(myprefs.boxes[3]->text()).arg(myprefs.boxes[0]->text()).arg("Contents/Resources/docSet.dsidx").toStdString().c_str(),&DB);
					if(exit!=SQLITE_OK)
						{ 
							std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl; 
							return; 
						}

					sql="CREATE TABLE searchIndex(id INTEGER PRIMARY KEY, name TEXT, type TEXT, path TEXT);CREATE UNIQUE INDEX anchor ON searchIndex (name, type, path);";
					exit=sqlite3_exec(DB,sql.toStdString().c_str(),NULL,0,&messaggeError); 

					if(exit != SQLITE_OK)
						{ 
							std::cerr << "Error Creating Table" << messaggeError << std::endl; 
							sqlite3_free(messaggeError); 
						} 
 
					sqlite3_exec(DB,"BEGIN TRANSACTION",NULL,NULL,&messaggeError);
					this->runPipeAndCapture(QString("cp -rp '%1' '%2'").arg(this->tempFolderPath+"/html/").arg(myprefs.boxes[3]->text()+"/"+myprefs.boxes[0]->text()+".docset/Contents/Resources/Documents"));
					this->runPipeAndCapture(QString("cp '%1/html/Info.plist' '%2'").arg(this->tempFolderPath).arg(myprefs.boxes[3]->text()+"/"+myprefs.boxes[0]->text()+".docset/Contents/"));
					this->runPipeAndCapture(QString("convert '%1' -resize 16x16 '%2/icon.png'").arg(myprefs.boxes[2]->text()).arg(myprefs.boxes[3]->text()+"/"+myprefs.boxes[0]->text()+".docset"));
					this->runPipeAndCapture(QString("convert '%1' -resize 32x32 '%2/icon@2x.png'").arg(myprefs.boxes[2]->text()).arg(myprefs.boxes[3]->text()+"/"+myprefs.boxes[0]->text()+".docset"));
					fp=popen("kkeditqttagreader Tokens.xml Token Name Type Path Anchor","r");
					if(fp!=NULL)
						{
							while(fgets(line,4095,fp))
								{
									line[strlen(line)-1]=0;
									thename=QString(line).section(' ',0,0).section('=',1,1);
									thetype=QString(line).section(' ',1,1).section('=',1,1);
									thepath=QString(line).section(' ',2,2).section('=',1,1);
									theanchor=QString(line).section(' ',3,3).section('=',1,1);
									if((thename.isEmpty()==true) || (thepath.isEmpty()==true))
										continue;
									if(thename.length()<NAME_MAX)
										{
											this->runPipeAndCapture(QString("echo -n \"%1 %3 ...\n0\" >\"%2/progress\"").arg("Adding").arg(this->tempFolderPath).arg(thename));
											if(theanchor.isEmpty()==false)
												thepath=thepath+"#"+theanchor;
											sql="insert into searchindex (name,type,path) values ('"+thename+"','"+thetype+"','"+thepath+"');";
											exit=sqlite3_exec(DB,sql.toStdString().c_str(),NULL,0,&messaggeError);
											if(exit != SQLITE_OK)
												{ 
													std::cerr << "Error Insert: " << messaggeError<<std::endl; 
													sqlite3_free(messaggeError); 
												}
										}
								}
							pclose(fp);
						}

					sqlite3_exec(DB,"END TRANSACTION",NULL,NULL,&messaggeError);
					sqlite3_close(DB); 

					QDir::setCurrent(holddir);
					this->runPipeAndCapture(QString("echo -e \"quit\n100\">\"%1/progress\"").arg(this->tempFolderPath));
				}
			delete myprefs.theDialog;
		}
}

void DocumentationPlugin::showBarberPole(QString windowtitle,QString bodylabel,QString cancellabel,QString maxitems,QString controlfile)//TODO//
{
	QStringList	arguments;
	QString		app="kkeditqtprogressbar";

	arguments<<"-c"<<QString("\"%6\" \"%1\" \"%2\" \"%3\" \"%4\" \"%5\"").arg(windowtitle).arg(bodylabel).arg(cancellabel).arg(maxitems).arg(controlfile).arg(app);
	QProcess::startDetached("sh",arguments);
}

void DocumentationPlugin::buildDocumentation(void)
{
	if(this->data!=NULL)
		{
			QString					projectname;
			QString					versionbox;
			QString					dotimagetype;
			QString					holddir;
			QString					dump;
			QSettings				settings("KDHedger","DocsPrefs");
			miniPrefsReturnStruct	myprefs;
			bool						gotdoxyfile=false;
			FILE						*fp;
			char						line[4096];
			int						res;

			holddir=QDir::currentPath();
			if(QFileInfo(QString("%1/Doxyfile").arg(QFileInfo(this->folderPath).absoluteFilePath())).exists()==true)
				gotdoxyfile=true;
			else
				QProcess::execute("cp",QStringList()<<QString("%1/Doxyfile").arg(QFileInfo(this->plugPath).absolutePath())<<this->folderPath);

			QDir::setCurrent(this->folderPath);

			if(gotdoxyfile==true)
				{
					projectname=this->runPipeAndCapture(QString("sed -n 's/PROJECT_NAME=\\(.*\\)/\\1/p' %1").arg(this->folderPath+"/Doxyfile"));
					versionbox=this->runPipeAndCapture(QString("sed -n 's/PROJECT_NUMBER=\\(.*\\)/\\1/p' %1").arg(this->folderPath+"/Doxyfile"));
					dotimagetype=this->runPipeAndCapture(QString("sed -n 's/DOT_IMAGE_FORMAT=\\(.*\\)/\\1/p' %1").arg(this->folderPath+"/Doxyfile"));
				}
			else
				{
					projectname=settings.value("Project Name").toString();
					if(projectname.isEmpty()==true)
						projectname=this->runPipeAndCapture(QString("sed -n 's/PROJECT_NAME=\\(.*\\)/\\1/p' %1").arg(this->folderPath+"/Doxyfile"));
					versionbox=settings.value("Project Version").toString();
					if(versionbox.isEmpty()==true)
						versionbox=this->runPipeAndCapture(QString("sed -n 's/PROJECT_NUMBER=\\(.*\\)/\\1/p' %1").arg(this->folderPath+"/Doxyfile"));
					dotimagetype=settings.value("DOT Image Type","").toString();
				}

			myprefs=this->miniPrefsDialog("DocsPrefs",QStringList()<<"Project Name"<<"Project Version"<<"DOT Image Type",true);
			myprefs.boxes[0]->setText(projectname.trimmed());
			myprefs.boxes[1]->setText(versionbox.trimmed());
			myprefs.boxes[2]->setText(dotimagetype.trimmed());

			res=myprefs.theDialog->exec();	
			if(res==1)
				{
					QString com=QString("sed -i 's|^PROJECT_NAME=.*$|PROJECT_NAME=%1|;s|^PROJECT_NUMBER=.*$|PROJECT_NUMBER=%2|;s|^GENERATE_DOCSET=.*$|GENERATE_DOCSET=YES|;s|^SERVER_BASED_SEARCH=.*$|SERVER_BASED_SEARCH=NO|;s|^SEARCHENGINE=.*$|SEARCHENGINE=NO|' '%3'").arg(myprefs.boxes[0]->text()).arg(myprefs.boxes[1]->text()).arg("Doxyfile");
					if(myprefs.boxes[2]->text().isEmpty()==false)
						{
							dump=this->runPipeAndCapture(QString("sed -i 's/HAVE_DOT=.*/HAVE_DOT=YES/' %1").arg(this->folderPath+"/Doxyfile"));
							dump=this->runPipeAndCapture(QString("sed -i 's/DOT_IMAGE_FORMAT=.*/DOT_IMAGE_FORMAT=%2/' %1").arg(this->folderPath+"/Doxyfile").arg(myprefs.boxes[2]->text()));
						}
					else
						{
							dump=this->runPipeAndCapture(QString("sed -i 's/HAVE_DOT=.*/HAVE_DOT=NO/' %1").arg(this->folderPath+"/Doxyfile"));
							dump=this->runPipeAndCapture(QString("sed -i 's/DOT_IMAGE_FORMAT=.*/DOT_IMAGE_FORMAT=/' %1").arg(this->folderPath+"/Doxyfile"));
						}
					dump=this->runPipeAndCapture(com);
					delete myprefs.theDialog;
				}
			else
				{
					if(gotdoxyfile==false)
						QProcess::execute("rm",QStringList()<<QString("%1/Doxyfile").arg(this->folderPath));
					delete myprefs.theDialog;
					QDir::setCurrent(holddir);
					return;
				}

			this->showBarberPole("Building Docs","Please Wait","","0",QString("%1/progress").arg(this->tempFolderPath));
			this->runPipeAndCapture(QString("echo -n \"%1\n0\" >\"%2/progress\"").arg("Building Docs, Please Wait...").arg(this->tempFolderPath));
			fp=popen("doxygen Doxyfile","r");
			if(fp!=NULL)
				{
					while(fgets(line,4095,fp))
						{
							line[strlen(line)-1]=0;
							this->runPipeAndCapture(QString("echo -n \"%1\n0\" >\"%2/progress\"").arg(line).arg(this->tempFolderPath));
						}
					pclose(fp);
				}
			else
				{
					this->runPipeAndCapture(QString("echo -e \"quit\n100\">\"%1/progress\"").arg(this->tempFolderPath));
					return;
				}		

			this->runPipeAndCapture(QString("echo -e \"quit\n100\">\"%1/progress\"").arg(this->tempFolderPath));
			QProcess::execute("rm",QStringList()<<"html/tabs.css");
			QDir::setCurrent(holddir);
		}
}


void DocumentationPlugin::searchDoxyDocs(const QString txt)
{
	struct docResultStruct
		{
			QString tagName;
			QString tagPath;
			QString tagType;
		};

	int							cnt=0;
	QFile						file(QString("%1/html/Tokens.xml").arg(this->folderPath));
	QXmlStreamReader				reader(&file);
	QFile						html(this->mainKKEditClass->htmlFile);
	QString						searchfor;
	QString						tagname;
	QString						tagtype; 
	QString						tagpath;
	QString						taganchor;
	QMap<int,docResultStruct>	resultmap;

	if(!file.open(QFile::ReadOnly | QFile::Text))
		{
			qDebug() << "Cannot read file" << file.errorString();
			return;
		}
	searchfor=txt;
	if (reader.readNextStartElement())
		{
			if (reader.name().compare(QString("Tokens")) == 0)
				{
					tagname="";
					tagtype="";
					tagpath="";
					taganchor="";
					while(reader.readNextStartElement())
						{
							if (reader.name().compare(QString("Token")) == 0)
								{
									while(reader.readNextStartElement())
										{
											if (reader.name().compare(QString("TokenIdentifier")) == 0)
												{
													while(reader.readNextStartElement())
														{
															if (reader.name().compare(QString("Name")) == 0)
																{
																	tagname=reader.readElementText();
																	continue;
																}
															if (reader.name().compare(QString("Type")) == 0)
																{
																	tagtype=reader.readElementText();
																	continue;
																}
															reader.skipCurrentElement();
														}
													continue;
												}
											if (reader.name().compare(QString("Path")) == 0)
												{
													tagpath=reader.readElementText();
													continue;
												}
											if (reader.name().compare(QString("Anchor")) == 0)
												{
													taganchor=reader.readElementText();
													continue;
												}

											reader.skipCurrentElement();
										}
								}
							else
								reader.skipCurrentElement();
							if(tagname.contains(searchfor,Qt::CaseInsensitive)==true)
								{
									resultmap[cnt++]={tagname,this->folderPath+"/html/"+tagpath+"#"+taganchor,tagtype};
								}
						}
				}
			else
				reader.raiseError(QObject::tr("Incorrect file"));
		}

	if(cnt>0)
		{
			if(html.open(QFile::WriteOnly|QFile::Truncate))
				{
		 			QTextStream out(&html);
					out << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">" << Qt::endl;
					out << "<html>" << Qt::endl;
					out << "<head>" << Qt::endl;
					out << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">" << Qt::endl;
					out << "</head>" << Qt::endl;
					out << "<style>" << Qt::endl;
					//out << "tab0  {position:absolute;left:80px;}" << Qt::endl;
					out << "</style>" << Qt::endl;
					out << "<body>" << Qt::endl;

					for(int loop=0;loop<cnt;loop++)
						//out <<QString("<div align=\"left\">%3<tab0><a href=\"%1\">%2</a></div>\n").arg(resultmap[loop].tagPath).arg(resultmap[loop].tagName).arg(resultmap[loop].tagType) << Qt::endl;
						out <<QString("<div align=\"left\">%3&nbsp;&nbsp;&nbsp;&nbsp;<a href=\"%1\">%2</a></div>\n").arg(resultmap[loop].tagPath).arg(resultmap[loop].tagName).arg(resultmap[loop].tagType) << Qt::endl;

					out << "</body>" << Qt::endl;
					out << "</html>" << Qt::endl;
					html.close();
					QString comm=QString("kkeditqtmsg -k %1 -c openindocview -d %2").arg(this->mainKKEditClass->sessionID).arg(this->mainKKEditClass->htmlFile);
					system(comm.toStdString().c_str());
				}
			}
}

void DocumentationPlugin::findInDocs(void)
{
	QTextCursor	tc;

	if(this->doc==NULL)
		return;

	tc=this->doc->textCursor();
	if(tc.hasSelection()==true)
		this->searchDoxyDocs(tc.selectedText());
}

void DocumentationPlugin::initPlug(KKEditClass *kk,QString pathtoplug)
{
	QString		results;
	QString		command;
	QSettings	plugprefs("KDHedger","DocumentationPlugin");
	bool			breakit=false;

	this->noStatusInfo=true;
	this->customCommand=plugprefs.value("customcommand").toString();

	this->mainKKEditClass=kk;
	this->plugPath=pathtoplug;

	this->doxyMenu=new QAction("Build Documentation");
	this->doxyMenu->setIcon(QIcon::fromTheme("edit-copy"));
	QObject::connect(this->doxyMenu,&QAction::triggered,[this]()
		{
			this->buildDocumentation();
		});

	this->docsetMenu=new QAction("Build Docset");
	this->docsetMenu->setIcon(QIcon::fromTheme("edit-copy"));
	QObject::connect(this->docsetMenu,&QAction::triggered,[this]()
		{
			this->buildDocset();
		});

	this->showDocsMenu=new QAction("Show Documentation");
	this->showDocsMenu->setIcon(QIcon::fromTheme("edit-find"));
	QObject::connect(this->showDocsMenu,&QAction::triggered,[this]()
		{
			this->showDocs();
		});

	this->mainKKEditClass->pluginMenu->addAction(this->doxyMenu);
	this->mainKKEditClass->pluginMenu->addAction(this->docsetMenu);
	this->mainKKEditClass->pluginMenu->addAction(this->showDocsMenu);

	this->apiMenu=new QMenu("Find API");
	this->apiMenu->setIcon(QIcon::fromTheme("edit-find"));

	foreach(QAction *action,this->mainKKEditClass->navMenu->actions())
		{
			if(breakit==true)
				{
					this->mainKKEditClass->navMenu->insertMenu(action,this->apiMenu);
					break;
				}
			if(action->text().compare("Search For Define")==0)
				breakit=true;
		}
	command=QString("cat $(find '%1/actions' -iname '*.info'|sort -V)").arg(QFileInfo(this->plugPath).absolutePath());
	results=this->runPipeAndCapture(command);
	this->resList=results.split("\n",Qt::SkipEmptyParts);

	for(int j=0;j<this->resList.size();j+=SETTINGSCNT)
		{
			if(this->resList.at(j+SHOWITEM).compare("show")==0)
				{
					QAction	*menuitem=new QAction(this->resList.at(j+MENUNAME));
					this->apiMenu->addAction(menuitem);
					QObject::connect(menuitem,&QAction::triggered,[this,j]()
						{
							this->runSearch(this->resList.at(j+COMMAND));
						});
				}
		}

	this->apiMenu->addSeparator();
	QAction	*menuitem=new QAction("Run All Searches");
	this->apiMenu->addAction(menuitem);
	QObject::connect(menuitem,&QAction::triggered,[this]()
		{
			this->runAllSearchs();
		});

	menuitem=new QAction("Find In Documentation");
	this->apiMenu->addAction(menuitem);
	QObject::connect(menuitem,&QAction::triggered,[this]()
		{
			this->findInDocs();
		});

	if(this->customCommand.isEmpty()==false)
		{
			this->apiMenu->addSeparator();
			this->customCommandMenu=new QAction("Custom Command");
			this->apiMenu->addAction(this->customCommandMenu);
			QObject::connect(this->customCommandMenu,&QAction::triggered,[this]()
				{
					QTextCursor	tc;
					QString		com;

					if(this->doc!=NULL)
						{
							tc=this->doc->textCursor();
							if(tc.hasSelection()==true)
								{
									QStringList sl;
									QStringList args;

									sl=customCommand.split(" ",Qt::SkipEmptyParts);
									args=sl.replaceInStrings("%p",tc.selectedText().trimmed());
									args.erase(args.begin());
									QProcess::startDetached(sl.at(0),args);
								}
						}
				});
		}
}

void DocumentationPlugin::unloadPlug(void)
{
	delete this->doxyMenu;
	delete this->docsetMenu;
	delete this->showDocsMenu;
	delete this->apiMenu;
}

void DocumentationPlugin::plugAbout(void)
{
	QMessageBox	msgBox;
	QFileInfo	fileinfo(this->plugPath);
	QString		txt="Search API Docs Plugin\n\n©K.D.Hedger 2024\n\n<a href=\"" GLOBALWEBSITE "\">Homepage</a>\n\n<a href=\"mailto:" MYEMAIL "\">Email Me</a>";

	msgBox.setText(txt);
	msgBox.setIconPixmap(QPixmap("/usr/share/KKEditQT/pixmaps/KKEditQTPlug.png"));
	msgBox.setWindowTitle("Plugin About");
	msgBox.setTextFormat(Qt::MarkdownText);
	msgBox.setStandardButtons(QMessageBox::Help|QMessageBox::Close);
	int ret=msgBox.exec();
	switch(ret)
		{
			case QMessageBox::Close:
				break;
			case QMessageBox::Help:
				{
					QStringList args;
					args<<"-k";
					args<<QString("%1").arg(this->mainKKEditClass->sessionID);
					args<<"-c"<<"openindocview";
					args<<"-d"<<fileinfo.canonicalPath()+"/docs/help.html";
					QProcess::startDetached("kkeditqtmsg",args);
					this->mainKKEditClass->pluginPrefsWindow->hide();
				}
				break;
		}
}

void DocumentationPlugin::plugSettings(void)
{
	QDialog		settings(this->mainKKEditClass->pluginPrefsWindow);
	QVBoxLayout	*vlayout;
	QHBoxLayout	*hlayout;
	QSettings	plugprefs("KDHedger","DocumentationPlugin");
	QLineEdit	customcommand(this->customCommand);
	QPushButton	*btn;
	QLabel		label("Custom Command %p will be replaced with selected text:");

	settings.setWindowTitle("DocumentationPlugin Settings");
	vlayout=new QVBoxLayout(&settings);
	vlayout->addWidget(&label);
	vlayout->addWidget(&customcommand);

	hlayout=new QHBoxLayout();
	btn=new QPushButton("Apply");
	QObject::connect(btn,&QPushButton::clicked,[&settings]() { settings.done(1); });
	hlayout->addWidget(btn);

	btn=new QPushButton("Cancel");
	QObject::connect(btn,&QPushButton::clicked,[&settings]() { settings.done(0); });
	hlayout->addWidget(btn);
	vlayout->addLayout(hlayout);

	settings.exec();
	if(settings.result()==1)
		{
			QSettings	plugprefs("KDHedger","DocumentationPlugin");
			plugprefs.setValue("customcommand",customcommand.displayText());
		}
}

void DocumentationPlugin::plugRun(plugData *data)
{
//settings and about handled seperatly
	if(data==NULL)
		return;

	this->data=data;

	switch(data->what)
		{
			case DOSETSENSITVE:
				{
					bool hasselection=false;
					this->data=data;
					this->doc=data->doc;
					if(data->doc!=NULL)
						hasselection=data->doc->textCursor().hasSelection();
					this->apiMenu->setEnabled(hasselection);
					if(hasselection==true)
						this->customCommandMenu->setText(QString("%1").arg(QString(this->customCommand).replace("%p",data->doc->textCursor().selectedText().trimmed())));
				}
				break;

			case DOSWITCHPAGE:
				{
					this->data=data;
					this->doc=data->doc;
					this->filePath=data->userStrData1;
					this->folderPath=data->userStrData3;
					this->tempFolderPath=data->tempFolder;
//qDebug()<<">>>>>>"<<data->userStrData1<<data->userStrData2<<data->userStrData3<<this->data->tempFolder;
					break;
				}

			case DOCONTEXTMENU:
				{
					bool breakit=false;

					foreach(QAction *action,data->menu->actions())
						{
							if(breakit==true)
								{
									data->menu->insertMenu(action,this->apiMenu);
									break;
								}
							if(action->text().compare("Search For Define")==0)
								breakit=true;
						}
				}
				break;
		}
}

unsigned int DocumentationPlugin::plugWants(void)
{
	return(DOCONTEXTMENU|DOSETTINGS|DOABOUT|DOSWITCHPAGE|DOSETSENSITVE|DOPOSTLOAD);
}
