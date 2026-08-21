/*
 *
 * ©K. D. Hedger. Fri 21 Aug 16:04:47 BST 2026 keithdhedger@gmail.com

 * This file (ChooserDialog.cpp) is part of KKEditQT.

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

#include "ChooserDialog.h"

chooserDialogClass::~chooserDialogClass()
{
}

chooserDialogClass::chooserDialogClass(chooserDialogType type,QString name,QString startfolder)
{
	QSettings	prefs("KDHedger","ChooserDialog");
	QDir			folders("/");
	QString		command;

	this->recentFoldersPath=QString("%1/.config/KDHedger/recentfolders").arg(QDir::homePath());
	this->recentFilesPath=QString("%1/.config/KDHedger/recentfiles").arg(QDir::homePath());
	folders.mkpath(this->recentFoldersPath);
	folders.mkpath(this->recentFilesPath);

	this->dialogType=type;

	if(type==chooserDialogType::loadDialog||type==chooserDialogType::folderDialog)
		{
			if(startfolder.isEmpty()==true)
				this->currentFolderPath=prefs.value("lastloadfolder").toString();
			else
				this->currentFolderPath=startfolder;
		}

	if(type==chooserDialogType::saveDialog)
		{
			if(startfolder.isEmpty()==true)
				this->currentFolderPath=prefs.value("lastsavefolder").toString();
			else
				this->currentFolderPath=startfolder;
			this->filepathEdit->setText(name);
		}

	if((this->currentFolderPath.isEmpty()==true) || (QFileInfo(this->currentFolderPath).exists()==false))
		this->currentFolderPath="/";

	this->buildMainGui();
	this->dialogWindow.restoreGeometry(prefs.value("choosersize").toByteArray());

	command=QString("cd %1/ >/dev/null;ls -t1|tail -n +%2| xargs -I {} rm '{}'").arg(this->recentFilesPath).arg(this->maxRecents);
	system(command.toStdString().c_str());
	command=QString("cd %1 >/dev/null;ls -t1|tail -n +%2| xargs -I {} rm '{}'").arg(this->recentFoldersPath).arg(this->maxRecents);
	system(command.toStdString().c_str());
}

void chooserDialogClass::setShowImagesInList(bool show)
{
	this->showThumbsInList=show;
	this->setFileList(this->currentFolderPath);
}

//general app functions
void chooserDialogClass::fileEntryTextEdited(QString text)
{
	this->fileList.clearSelection();
	this->multiFileList.clear();
	this->apply->setEnabled(!text.isEmpty());

	if(this->dialogType==chooserDialogType::saveDialog)
		this->apply->setText("Save");

	if(this->dialogType==chooserDialogType::loadDialog)
		this->apply->setText("Select");
}


void chooserDialogClass::buildMainGui(void)
{
	QVBoxLayout	*windowvlayout=new QVBoxLayout;
	QVBoxLayout	*sidevlayout=new QVBoxLayout;
	QVBoxLayout	*infovlayout=new QVBoxLayout;
	QVBoxLayout	*controlsvlayout=new QVBoxLayout;
	QHBoxLayout	*hlayout=new QHBoxLayout;
	QSplitter	*splitter=new QSplitter(Qt::Horizontal,&this->dialogWindow);

	this->dialogWindow.setWindowTitle("Select File");

//sidelist
	this->sideListModel=new QStandardItemModel(0,1);
    this->sideList.setModel(this->sideListModel);
	this->sideList.setEditTriggers(QAbstractItemView::NoEditTriggers);
	QObject::connect(&this->sideList,&QListView::clicked,[this](const QModelIndex &index)
		{
			this->selectSideItem(index);
		});
	QObject::connect(&this->sideList,&QListView::doubleClicked,[this](const QModelIndex &index)
		{
			this->doubleClickSideList(index);
		});

//filelist
	this->fileListModel=new QStandardItemModel(0,1);
    this->fileList.setModel(this->fileListModel);
	this->fileList.setEditTriggers(QAbstractItemView::NoEditTriggers);
	QObject::connect(&this->fileList,&QListView::clicked,[this](const QModelIndex &index)
		{
			this->setSelectedFiles(index,true);
		});
	QObject::connect(&this->fileList,&QListView::doubleClicked,[this](const QModelIndex &index)
		{
			this->doubleClickFileList(index);
		});

	QObject::connect(splitter,&QSplitter::splitterMoved,[this,splitter](int pos, int index)
		{
			QSettings	prefs("KDHedger","ChooserDialog");
			prefs.setValue("splittersize", splitter->saveState());
		});

	QPushButton *deletefav=new QPushButton("Remove Fav");
	deletefav->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ListRemove));
	QObject::connect(deletefav,&QPushButton::clicked,[this]()
		{
			QModelIndex ind=this->sideList.currentIndex();
			if(ind.data(Qt::StatusTipRole).toString().isEmpty()==false)
				this->sideListModel->removeRow(this->sideList.currentIndex().row());
		});

	sidevlayout->addWidget(&this->sideList);
	sidevlayout->addWidget(deletefav);

	this->previewIcon.setMaximumWidth(128);
	this->previewIcon.setMinimumWidth(128);
	this->previewIcon.setAlignment(Qt::AlignCenter);
	this->previewMimeType.setWordWrap(true);
	infovlayout->addWidget(&this->previewIcon);
	infovlayout->addWidget(&this->previewMimeType);
	infovlayout->addWidget(&this->previewSize);
	infovlayout->addWidget(&this->previewMode);
	infovlayout->addStretch();

	QWidget *wrapper=new QWidget(&this->dialogWindow);
	sidevlayout->setContentsMargins(0,0,0,0);
	wrapper->setLayout(sidevlayout);
	splitter->addWidget(wrapper);
	splitter->addWidget(&this->fileList);
	
	hlayout->addWidget(splitter);
	hlayout->addLayout(infovlayout);

	windowvlayout->addLayout(hlayout);

	this->folderCombo=new QComboBox;
	QObject::connect(this->folderCombo,&QComboBox::currentTextChanged,[this](const QString &text)
		{
			this->setFileList(text);
		});
	controlsvlayout->addWidget(this->folderCombo);
	this->filepathEdit=new QT_lineEditCompleterClass("",&this->dialogWindow);
	this->filepathEdit->setCompleteType(STRINGCOMPLETE);
	controlsvlayout->addWidget(this->filepathEdit);

	this->fileTypes.setMinimumContentsLength(64);
	controlsvlayout->addWidget(&this->fileTypes);
	QObject::connect(&this->fileTypes,&QComboBox::currentTextChanged,[this](const QString &text)
		{
			this->setFileList(this->currentFolderPath);
		});

	hlayout=new QHBoxLayout;
	QPushButton *cancel=new QPushButton("Cancel");
	cancel->setIcon(QIcon::fromTheme("dialog-cancel"));
	QObject::connect(cancel,&QPushButton::clicked,[this]()
		{
			QSettings	prefs("KDHedger","ChooserDialog");

			prefs.setValue("choosersize",this->dialogWindow.saveGeometry());
			this->setExitData(false);
		});

	QPushButton *hidden=new QPushButton("Hidden");
	hidden->setCheckable(true);
	hidden->setIcon(QIcon::fromTheme("dialog-question"));
	QObject::connect(hidden,&QPushButton::clicked,[this]()
		{
			this->showHidden=!this->showHidden;
			this->setFileList(this->currentFolderPath);
		});

	QPushButton *newfolder=new QPushButton("New Folder");
	newfolder->setIcon(QIcon::fromTheme("folder-open"));
	QObject::connect(newfolder,&QPushButton::clicked,[this]()
		{
			bool		ok;
			int		cnt=1;
			QString	nfname="New Folder";
			if(QFileInfo::exists(QString("%1/%2").arg(this->currentFolderPath).arg(nfname)))
				{
					while(QFileInfo::exists(QString("%1/%2-%3").arg(this->currentFolderPath).arg(nfname).arg(cnt)))
						cnt++;
					nfname=QString("%1-%2").arg(nfname).arg(cnt);
				}
     		QString	text=QInputDialog::getText(&this->dialogWindow,"New Folder","New folder name",QLineEdit::Normal,nfname,&ok);
			if(ok==true)
				{
					QDir dirp(this->currentFolderPath);
					if(dirp.mkdir(text)==true)
						this->setFileList(this->currentFolderPath+"/"+text);
					else
						QMessageBox::critical(&this->dialogWindow,"Failed","Can't create folder ...");
				}
		});

	QPushButton *refresh=new QPushButton("Refresh");
	refresh->setIcon(QIcon::fromTheme("refresh"));
	QObject::connect(refresh,&QPushButton::clicked,[this]()
		{
			this->setFileList(this->currentFolderPath);
		});

	if(this->dialogType==chooserDialogType::loadDialog)
		this->apply=new QPushButton("Open");
	else
		this->apply=new QPushButton("Save");
	this->apply->setIcon(QIcon::fromTheme("dialog-ok"));
	this->apply->setDefault(true);

	QObject::connect(apply,&QPushButton::clicked,[this]()
		{
			this->doChoose();
		});

	hlayout->addWidget(cancel);
	hlayout->addStretch();
	hlayout->addWidget(hidden);
	hlayout->addStretch();
	hlayout->addWidget(newfolder);
	hlayout->addStretch();
	hlayout->addWidget(refresh);
	hlayout->addStretch();
	hlayout->addWidget(apply);
	controlsvlayout->addLayout(hlayout);
	windowvlayout->addLayout(controlsvlayout);

	this->dialogWindow.setLayout(windowvlayout);
	this->setSideList();
	////this->setFileList();
//	if(this->saveDialog==false)
//		this->filepathEdit->setText("");
//	else
//		{
//			this->selectedFilePath=this->localWD+"/"+this->saveName;
//			this->selectedFileName=this->saveName;
//			this->filepathEdit->setText(this->saveName);
//		}
	this->fileList.setDragEnabled(true);
	this->sideList.setAcceptDrops(true);

	QSettings	prefs("KDHedger","ChooserDialog");
	splitter->restoreState(prefs.value("splittersize").toByteArray());

	QObject::connect(this->fileList.selectionModel(),&QItemSelectionModel::selectionChanged,[this](const QItemSelection &selected,const QItemSelection &deselected)
		{
			this->fileListSelectionChanged();
		});

	this->filepathEdit->useInternaleSC=false;
	this->pathActivateKey=new QShortcut(QKeySequence("TAB"),&this->dialogWindow);
	this->pathActivateKey->setContext(Qt::WidgetWithChildrenShortcut);
	QObject::connect(this->pathActivateKey,&QShortcut::activated,[this]()
		{
			this->filepathEdit->doActivateKey();
		});

	this->pathCancelKey=new QShortcut(QKeySequence("Esc"),&this->dialogWindow);
	this->pathCancelKey->setContext(Qt::WidgetWithChildrenShortcut);
	QObject::connect(this->pathCancelKey,&QShortcut::activated,[this]()
		{
			this->filepathEdit->doCancelKey();
		});

	QObject::connect(this->filepathEdit,&QT_lineEditCompleterClass::textEdited,[this](const QString &text)
		{
			if(this->filepathEdit->text().isEmpty()==true)
				{
					this->fileList.clearSelection();
					this->apply->setEnabled(false);
				}
		});

	QObject::connect(this->filepathEdit,&QT_lineEditCompleterClass::editingFinished,[this]()
		{
			QModelIndex				index;
			QList<QStandardItem*>	foundItems=this->fileListModel->findItems(this->filepathEdit->text(),Qt::MatchStartsWith);
			if(foundItems.size()>0)
				{
					index=this->fileListModel->indexFromItem(foundItems.at(0));
					this->fileList.setCurrentIndex(index);
				}
		});

	QObject::connect(this->filepathEdit,&QT_lineEditCompleterClass::textChanged,[this](const QString &text)
		{
			if(this->filepathEdit->text().isEmpty()==false)
				{
					QModelIndex				index;
					QList<QStandardItem*>	foundItems=this->fileListModel->findItems(text,Qt::MatchStartsWith);
					if(foundItems.size()==1)
						{
							index=this->fileListModel->indexFromItem(foundItems.at(0));
							this->fileList.setCurrentIndex(index);
						}
				}
		});
}

void chooserDialogClass::setMultipleSelect(bool select)
{
	if(this->dialogType!=chooserDialogType::loadDialog)
		return;

	this->useMulti=select;
	if(select==true)
		this->fileList.setSelectionMode(QAbstractItemView::ExtendedSelection);
	else
		this->fileList.setSelectionMode(QAbstractItemView::SingleSelection);
}

QIcon chooserDialogClass::getFileIcon(QString path)
{
	if(QFileInfo::exists(path)==false)
		return(QIcon::fromTheme("application-octet-stream"));

	QIcon				icon;
	QMimeDatabase		db;
	QString				realpath(QFileInfo(path).canonicalFilePath());
    QMimeType			type=db.mimeTypeForFile(realpath);

	if(type.name().compare("application/x-desktop")==0)
		{
			QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << "/usr/share/pixmaps");
			QSettings df(realpath,QSettings::IniFormat);
			icon=QIcon::fromTheme(df.value("Desktop Entry/Icon").toString());
		}
	else
		{
			if(type.name().contains("image"))
				{
					if((QFileInfo(path).size()<MAXIMAGESIZETOTHUMB) && (this->showThumbsInList==true))
						icon=QIcon(path);
					else
						icon=QIcon::fromTheme(type.iconName(),QIcon::fromTheme("image-x-generic"));
				}
			else
				{
					icon=QIcon::fromTheme(type.iconName());
				}
		}

	if(icon.isNull()==true)
		icon=QIcon::fromTheme("application-octet-stream");
	return(icon);
}

void chooserDialogClass::showPreViewData(QString file)
{	QIcon			icon;
	QPixmap			pixmap;
	QMimeDatabase	db;
	QString			mod;
	QMimeType		type;;

	if(file.isEmpty()==true)
		return;

	type=db.mimeTypeForFile(file);
	this->previewMimeType.setText(type.name());

	if(type.name().contains("image"))
		{
			pixmap.load(file);
			pixmap=pixmap.scaled(128,128,Qt::KeepAspectRatio,Qt::SmoothTransformation);
			if(pixmap.isNull()==true)
				{
					icon=QIcon::fromTheme(type.iconName(),QIcon::fromTheme("image"));
					pixmap=icon.pixmap(QSize(128,128)).scaledToWidth(128,Qt::SmoothTransformation);;
				}
		}
	else
		{
			icon=this->getFileIcon(file);
			pixmap=icon.pixmap(QSize(128,128)).scaledToWidth(128,Qt::SmoothTransformation);;
		}

	this->previewIcon.setPixmap(pixmap);

	if(QFileInfo(file).isDir()==true)
		{
			int d=QDir(file,nullptr,QDir::Name,QDir::NoDotAndDotDot|QDir::AllDirs|QDir::System|QDir::Hidden).count();
			int f=QDir(file,nullptr,QDir::Name,QDir::NoDotAndDotDot|QDir::Files|QDir::System|QDir::Hidden).count();
			this->previewSize.setText(QString("Folders: %1\nFiles: %2").arg(d).arg(f));
		}
	else
		this->previewSize.setText(QString("Size: %1").arg(QFileInfo(file).size()));

	this->getFilePermissions(file);
}

void chooserDialogClass::getFilePermissions(QString filePath)
{
	QFile		file(filePath);
	QFileInfo	fileInfo(file);
	QString		perms;

    if(!fileInfo.exists())
	    	{
	    		this->previewMode.setText("File Doesn't Exist");
			return;
		}

    // Get permissions
	QString permissions = QString::number(fileInfo.permissions(), 8);
	QString permissionString="";
	permissionString+="Owner ";
	permissionString+=(fileInfo.permissions() & QFile::ReadUser) ? "r" : "-";
	permissionString+=(fileInfo.permissions() & QFile::WriteUser) ? "w" : "-";
	permissionString+=(fileInfo.permissions() & QFile::ExeUser) ? "x" : "-";
	permissionString+="\nGroup ";
	permissionString+=(fileInfo.permissions() & QFile::ReadGroup) ? "r" : "-";
	permissionString+=(fileInfo.permissions() & QFile::WriteGroup) ? "w" : "-";
	permissionString+=(fileInfo.permissions() & QFile::ExeGroup) ? "x" : "-";
	permissionString+="\nOther ";
	permissionString+=(fileInfo.permissions() & QFile::ReadOther) ? "r" : "-";
	permissionString+=(fileInfo.permissions() & QFile::WriteOther) ? "w" : "-";
	permissionString+=(fileInfo.permissions() & QFile::ExeOther) ? "x" : "-";

	perms=QString::number((fileInfo.permissions() & 0xfff),16);
    // Get ownership
	QString owner=QString::number(fileInfo.ownerId());
	QString group=QString::number(fileInfo.groupId());

	this->previewMode.setText(QString("Access: 0%1\n%2\n\nUid:%3\nGid:%4").arg(perms).arg(permissionString).arg(owner).arg(group));
}

void chooserDialogClass::setFavs(void)
{
	QSettings			prefs("KDHedger","ChooserDialog");
	QStringList			sl;
	QItemSelectionModel	*model;
	QModelIndexList		list;
	QString				filepath;

	this->sideList.setSelectionMode(QAbstractItemView::ExtendedSelection);
	this->sideList.selectAll();
	model=this->sideList.selectionModel();
	list=model->selectedIndexes();

	for(int j=0;j<list.count();j++)
		{
			filepath=QFileInfo(list.at(j).data(Qt::StatusTipRole).toString()).absoluteFilePath();
			if(filepath.isEmpty()==false)
				sl<<filepath;
		}
	prefs.setValue("customfolders",sl);
}

void chooserDialogClass::doChoose(void)
{
	if(this->apply->text()=="Open")
		{
			if(this->selectedFolderPath.isEmpty()==false)
				{
					if(this->fileList.currentIndex().isValid()==false)
						return;
					if(this->fromRecents==true)
						{
							this->fromRecents=false;
							this->setFileList(QFileInfo(this->selectedFolderPath).canonicalFilePath());
						}
					else
						this->setFileList(QFileInfo(this->selectedFolderPath).absoluteFilePath());
				}
		}
	else
		{
			this->setExitData(true);
		}
}

void chooserDialogClass::setExitData(bool valid)
{
	QSettings	prefs("KDHedger","ChooserDialog");
	QString		lastfolder="";
	QString		recentfolder;
	QFile 		fold;

	if(valid==true)
		{
			if(this->dialogType!=chooserDialogType::folderDialog)
				{
					if(this->multiFileList.count()==0)
						{
							if(this->filepathEdit->text().isEmpty()==false)
								this->multiFileList.push_back(QString("%1/%2").arg(this->currentFolderPath).arg(this->filepathEdit->text()));
							else
								return;
						}
				}
			else
				{
					this->multiFileList.push_back(QString("%1").arg(this->currentFolderPath));
				}

			if(this->dialogType==chooserDialogType::saveDialog)
				{
					if(QFileInfo::exists(this->multiFileList.at(0))==true)
						{
							QMessageBox::StandardButton	reply;

							reply=QMessageBox::question(&this->dialogWindow,"Save",QString("'%1' exists.\nDo you want to replace it?").arg(QFileInfo(this->multiFileList.at(0)).fileName()),QMessageBox::Yes|QMessageBox::No);
							if(reply==QMessageBox::No)
								return;
						}
				}

			for(const QString& str : this->multiFileList)
				{
//files
					fold.setFileName(str);
					recentfolder=QString("%1/%2").arg(this->recentFilesPath).arg(QFileInfo(str).fileName());
					fold.link(recentfolder);					
//folders
					fold.setFileName(QFileInfo(str).path());
					recentfolder=QString("%1/%2").arg(this->recentFoldersPath).arg(QFileInfo(str).dir().dirName());
					if(this->dialogType==chooserDialogType::folderDialog)
						lastfolder=str;
					else
						lastfolder=QFileInfo(str).path();
					fold.link(recentfolder);	
				}

			if(lastfolder.isEmpty()==false)
				{
					if(this->dialogType==chooserDialogType::folderDialog)
						{
							if(lastfolder!=this->recentFoldersPath)
								prefs.setValue("lastloadfolder",lastfolder);
						}
					if(this->dialogType==chooserDialogType::loadDialog)
						prefs.setValue("lastloadfolder",lastfolder);
					if(this->dialogType==chooserDialogType::saveDialog)
						prefs.setValue("lastsavefolder",lastfolder);
				}

			prefs.setValue("choosersize",this->dialogWindow.saveGeometry());
		}
	this->setFavs();
	this->valid=valid;
	this->dialogWindow.hide();
}

void chooserDialogClass::addFileTypes(QString types)
{
	this->fileTypes.addItem(types.simplified());
}

void chooserDialogClass::setSelectedFiles(const QModelIndex &index,bool clear)
{
	QString	filename=index.data(Qt::UserRole).toString();
	
	this->showPreViewData(filename);

	if(QFileInfo(filename).isDir()==true || filename=="..")
		{
			if(this->dialogType==chooserDialogType::folderDialog)
				{
					if(filename.endsWith(".."))
						this->apply->setText("Open");
					else
						{
							this->apply->setText("Choose");
							this->filepathEdit->setText(QFileInfo(filename).fileName());
							this->apply->setEnabled(true);
						}
				}
			else
				this->apply->setText("Open");
			if(this->dialogType==chooserDialogType::loadDialog)
				this->filepathEdit->setText("");
			if(clear==true)
				{
					this->multiFileList.clear();
					this->fileList.clearSelection();
				}
			this->fileList.setCurrentIndex(index);
			this->selectedFolderPath=filename;
		}
	else
		{
			if(this->dialogType==chooserDialogType::loadDialog)
				{
					this->selectedFolderPath="";
					this->apply->setText("Select");
					this->filepathEdit->setText(QFileInfo(filename).fileName());
				}

			if(this->dialogType==chooserDialogType::saveDialog)
				{
					this->selectedFolderPath="";
					this->apply->setText("Save");
					this->filepathEdit->setText(QFileInfo(filename).fileName());
				}
	
//			if(this->dialogType==chooserDialogType::folderDialog)
//				{
//					this->selectedFolderPath="";
//					this->apply->setText("Choose");
//					this->filepathEdit->setText(QFileInfo(filename).fileName());
//				}
		}
	this->apply->setEnabled(true);
}

//sidelist functions
void chooserDialogClass::setSideList(void)
{
	QVariant				fullFilePathData;
	QStorageInfo			storage;
	QStandardItem		*item;
	QList<QStorageInfo>	ml=QStorageInfo::mountedVolumes();
	QString				disktype;
	QSettings			prefs("KDHedger","ChooserDialog");
	QStringList			sl=prefs.value("customfolders").toStringList();

///standard items
	this->sideListModel->clear();
	item=new QStandardItem(QIcon::fromTheme(QIcon::ThemeIcon::Computer),"Computer");
	fullFilePathData="/";
	item->setData(fullFilePathData,Qt::UserRole);
	this->sideListModel->appendRow(item);
	fullFilePathData=QDir::homePath();
	item=new QStandardItem(QIcon::fromTheme("user-home"),QFileInfo(QDir().homePath()).baseName());
	item->setData(fullFilePathData,Qt::UserRole);
	this->sideListModel->appendRow(item);
	
//recent folders
	item=new QStandardItem(QIcon::fromTheme("folder-saved-search"),"Recent Folders");
	fullFilePathData=this->recentFoldersPath;
	item->setData(fullFilePathData,Qt::UserRole);
	this->sideListModel->appendRow(item);

//recent files
	if(this->dialogType!=chooserDialogType::folderDialog)
		{
			item=new QStandardItem(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpenRecent),"Recent Files");
			fullFilePathData=this->recentFilesPath;
			item->setData(fullFilePathData,Qt::UserRole);
			this->sideListModel->appendRow(item);
		}

	item=new QStandardItem("");
	item->setEnabled(false);
	this->sideListModel->appendRow(item);

//mounted drives	 
	for(int j=0;j<ml.size();j++)
		{
			storage=ml.at(j);
			if((storage.fileSystemType().compare("tmpfs")!=0) && (storage.rootPath().compare("/")!=0) && (storage.rootPath().compare(QDir().homePath())!=0))
				{
					disktype="drive-harddisk";
					if(storage.fileSystemType().contains("nfs"))
						disktype="folder-remote";
					else if(storage.fileSystemType().contains("ssh"))
						disktype="network_local";
					else if(storage.fileSystemType().contains("iso9660"))
						{
							disktype="";
							item=new QStandardItem(QIcon::fromTheme(QIcon::ThemeIcon::DriveOptical),QFileInfo(storage.rootPath()).baseName());
						}

					if(disktype.isEmpty()==false)
						item=new QStandardItem(QIcon::fromTheme(disktype),QFileInfo(storage.rootPath()).baseName());
						
					fullFilePathData=storage.rootPath();
					item->setData(fullFilePathData,Qt::UserRole);
					this->sideListModel->appendRow(item);
				}
		}
	item=new QStandardItem("");
	item->setEnabled(false);
	this->sideListModel->appendRow(item);

//favs
	for(int j=0;j<sl.size();j++)
		{
			item=new QStandardItem(QIcon::fromTheme("user-bookmarks"),QFileInfo(sl.at(j)).fileName());
			item->setData(QFileInfo(sl.at(j)).fileName(),Qt::UserRole);
			item->setStatusTip(sl.at(j));
			this->sideListModel->appendRow(item);	
		}
}

void chooserDialogClass::selectSideItem(const QModelIndex &index)
{
	QList<QStorageInfo>		ml=QStorageInfo::mountedVolumes();
	QStorageInfo				storage;
	QString					disktype;
	QPixmap					pixmap;
	QIcon					icon; 
	QLocale					locale;
	QString					type;
	qint64					sze=0;
	qint64					freeb=0;
	QIcon::ThemeIcon			standardicon=QIcon::ThemeIcon::DriveHarddisk;
	const QAbstractItemModel	*model;
	QMap<int,QVariant>		map;
	int						itemoffset=2;

	disktype="";

	switch(index.row())
		{
			case 0:
				disktype="";
				standardicon=QIcon::ThemeIcon::Computer;
				storage=ml.at(index.row());
				sze=storage.bytesTotal();
				type=storage.fileSystemType();
				freeb=storage.bytesFree();
				break;
			case 1:
				disktype="user-home";
				type="";
				sze=0;
				freeb=0;
				break;
			case 2:
				disktype="folder-saved-search";
				type="";
				sze=0;
				freeb=0;
				break;
			case 3:
				disktype="";
				standardicon=QIcon::ThemeIcon::DocumentOpenRecent;
				type="";
				sze=0;
				freeb=0;
				break;
			case 4:
				return;
			default:
				if((index.row()-itemoffset)<ml.size())
					{
						storage=ml.at(index.row()-itemoffset);
						if((storage.rootPath().compare("/")!=0) && (storage.rootPath().compare(QDir().homePath())!=0))
							{
								if(storage.fileSystemType().contains("nfs"))
									disktype="folder-remote";
								else if(storage.fileSystemType().contains("ssh"))
									disktype="network_local";
								else if(storage.fileSystemType().contains("iso9660"))
									{
										standardicon=QIcon::ThemeIcon::DriveOptical;
										disktype="";
									}
								sze=storage.bytesTotal();
								type=storage.fileSystemType();
								freeb=storage.bytesFree();
							}
					}
				else
					{
						QString		mod;
						QString		str;
						int			d;
						int			f;
	
						model=index.model();
						map=model->itemData(index);
						if(map.find(Qt::StatusTipRole)!=map.end())
							{
								str=map[Qt::StatusTipRole].toString();
								if(str.isEmpty()==true)
									return;
								
								icon=QIcon::fromTheme("user-bookmarks");
								pixmap=icon.pixmap(QSize(128,128)).scaledToWidth(128,Qt::SmoothTransformation);
								this->previewIcon.setPixmap(pixmap);
								d=QDir(str,nullptr,QDir::Name,QDir::NoDotAndDotDot|QDir::AllDirs|QDir::System|QDir::Hidden).count();
								f=QDir(str,nullptr,QDir::Name,QDir::NoDotAndDotDot|QDir::Files|QDir::System|QDir::Hidden).count();
								this->previewSize.setText(QString("Folders: %1\nFiles: %2").arg(d).arg(f));
								this->previewMimeType.setText("inode/directory");
								this->getFilePermissions(str);
							}
						return;
					}
		}
	if(disktype.isEmpty()==false)
		icon=QIcon::fromTheme(disktype);
	else
		icon=QIcon::fromTheme(standardicon);
	pixmap=icon.pixmap(QSize(128,128)).scaledToWidth(128,Qt::SmoothTransformation);
	this->previewIcon.setPixmap(pixmap);
	this->previewSize.setText(QString("Size: %1").arg(locale.formattedDataSize(sze)));
	this->previewMimeType.setText(QString("FS Type: %1").arg(type));
	this->previewMode.setText(QString("Free: %1").arg(locale.formattedDataSize(freeb)));
}

void chooserDialogClass::doubleClickSideList(const QModelIndex &index)
{
	const QAbstractItemModel	*model;
	model=index.model();
	QMap map(model->itemData(index));
	QString dirstr;

	if(map.find(Qt::StatusTipRole)!=map.end())
		dirstr=map[Qt::StatusTipRole].toString();
	else
		dirstr=index.data(Qt::UserRole).toString();

	if(dirstr==this->recentFoldersPath || dirstr==this->recentFilesPath)
		this->fromRecents=true;
	else
		this->fromRecents=false;

	this->setFileList(dirstr);
}

//filelist functions
void chooserDialogClass::setFileList(QString dir,QDir::SortFlags sortas)
{
	QStandardItem	*item=NULL;
	QStringList		namefilters;
	QDir				d=dir;
	QDir::Filters	dfilts=QDir::System|QDir::Dirs|QDir::NoDot;
	QFileInfoList	fl;
	QStringList		sl;

	this->multiFileList.clear();
	this->currentFolderPath=dir;

	this->apply->setEnabled(false);
	if(this->dialogType==chooserDialogType::loadDialog)
		{
			this->filepathEdit->setText("");
			if(this->fromRecents==true)
				this->apply->setEnabled(false);
		}

	else if(this->dialogType==chooserDialogType::saveDialog)
		{
			if(this->filepathEdit->text().isEmpty()==false)
				{
					this->apply->setEnabled(true);
					this->apply->setText("Save");
				}
		}
	else if(this->dialogType==chooserDialogType::folderDialog)
		{
			this->apply->setEnabled(!this->fromRecents);
			this->apply->setText("Choose");
		}

	if(this->showHidden==true)
		dfilts|=QDir::Hidden;

	if(this->fileTypes.currentText()=="All Files")
		namefilters.clear();
	else
		{
			QString filts=this->fileTypes.currentText();
			filts=filts.replace(QRegularExpression("^.*\\((.*)\\).*"),"\\1");
			namefilters=filts.simplified().split(" ");
		}

	if(this->fromRecents==true)
		dfilts|=QDir::NoDotDot;

	fl=d.entryInfoList(QStringList(),dfilts,sortas);

	if(this->dialogType!=chooserDialogType::folderDialog)
		{
			dfilts=QDir::Files|QDir::System|QDir::NoDot;
			if(this->showHidden==true)
				dfilts|=QDir::Hidden;
			fl.append(d.entryInfoList(namefilters,dfilts,sortas));
		}

	this->fileListModel->clear();
	for(int j=0;j<fl.size();j++)
		{
			item=NULL;
			if(fl[j].isSymLink() && (fl[j].isFile() || fl[j].isDir()))
				{
					item=new QStandardItem(this->getFileIcon(fl[j].canonicalFilePath()),QString("%1->%2").arg(fl[j].fileName()).arg(fl[j].symLinkTarget()));
					item->setFont(QFont(item->font().family(),-1,QFont::Bold));				
				}
			else if(fl[j].isSymLink() && fl[j].exists()==false)
				{
					item=new QStandardItem(this->getFileIcon(fl[j].canonicalFilePath()),QString("%1 - Broken Link").arg(fl[j].fileName()));
					item->setFont(QFont(item->font().family(),-1,QFont::Bold));
				}
			else
				{
					item=new QStandardItem(this->getFileIcon(fl[j].canonicalFilePath()),fl[j].fileName());
				}

			if((fl[j].isSymLink() && fl[j].isDir()) || (fl[j].isDir()))
				item->setDragEnabled(true);
			else
				item->setDragEnabled(false);
				
			if(item!=NULL)
				{
					if(fl[j].fileName()!="..")
						sl<<fl[j].fileName();
					item->setData(fl[j].filePath(),Qt::UserRole);

					if(this->fromRecents==true)
						item->setStatusTip(fl[j].symLinkTarget());
					else
						item->setStatusTip(fl[j].absoluteFilePath());

					this->fileListModel->appendRow(item);
				}
		}

	this->fileList.scrollToTop();

	this->filepathEdit->setCompleteType(STRINGCOMPLETE);
	this->filepathEdit->setUpCompleter(sl);

	this->folderCombo->blockSignals(true);
		this->folderCombo->clear();
		if(this->fromRecents==false)
			{
				QString tstr=dir;
				for(int j=0;j<dir.count('/')+1;j++)
					{
						if(tstr.isEmpty()==false)
							this->folderCombo->addItem(tstr);
						tstr=QFileInfo(tstr).path();
					}
			}
	this->folderCombo->blockSignals(false);
//	this->apply->setEnabled(false);
}

void chooserDialogClass::doubleClickFileList(const QModelIndex &index)
{
	QString	filename=index.data(Qt::UserRole).toString();

	if(filename.endsWith(".."))
		{
			this->setFileList(QFileInfo(this->currentFolderPath).path());
		}
	else if(QFileInfo(filename).isDir()==true)
		{
			this->fromRecents=false;
			this->setFileList(QFileInfo(filename).canonicalFilePath());
		}
	else
		{
			this->setExitData(true);
		}
}

void chooserDialogClass::fileListSelectionChanged(void)
{
	QItemSelectionModel	*selectionmodel;
	QList<QModelIndex>	selectedindexes;

	this->multiFileList.clear();

	selectionmodel=this->fileList.selectionModel();

// Retrieve selected indexes
	selectedindexes=selectionmodel->selectedIndexes();
	if(selectedindexes.count()>0)
		{
// Iterate through selected indexes and access items
			for(const QModelIndex &index : selectedindexes)
				{
					QString itemtext=this->fileListModel->data(index, Qt::StatusTipRole).toString();
					if(itemtext.isEmpty()==true)
						{
							itemtext=this->fileListModel->data(index, Qt::UserRole).toString();
							itemtext.remove(" - Broken Link");
							this->multiFileList.push_back(itemtext);
						}
					else
						{
							this->multiFileList.push_back(itemtext);
						}
				}
			this->setSelectedFiles(selectedindexes.at(selectedindexes.count()-1),false);
		}
}

