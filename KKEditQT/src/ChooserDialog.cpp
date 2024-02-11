/*
 *
 * ©K. D. Hedger. Sun  4 Feb 19:41:20 GMT 2024 keithdhedger@gmail.com

 * This file (QT5ChooserDialog.cpp) is part of QT5FileDialog.

 * QT5FileDialog is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * QT5FileDialog is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with QT5FileDialog.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ChooserDialog.h"

chooserDialogClass::~chooserDialogClass()
{
}

void chooserDialogClass::setShowImagesInList(bool show)
{
	this->showThumbsInList=show;
}

void chooserDialogClass::setSaveDialog(bool save)
{
	this->saveDialog=save;
}

void chooserDialogClass::setMultipleSelect(bool select)
{
	if(this->saveDialog==true)
		return;

	this->useMulti=select;
	if(select==true)
		this->fileList.setSelectionMode(QAbstractItemView::ExtendedSelection);
	else
		this->fileList.setSelectionMode(QAbstractItemView::SingleSelection);
}

QIcon chooserDialogClass::getFileIcon(QString path)
{
	QIcon				icon;
	QMimeDatabase		db;
	QString				realpath(QFileInfo(path).canonicalFilePath());
    QMimeType			type=db.mimeTypeForFile(realpath);

	if(type.name().compare("application/x-desktop")==0)
		{
			QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << "/usr/share/pixmaps");
			std::map<unsigned long,std::vector<std::string>> entrys=LFSTK_UtilityClass::LFSTK_readFullDesktopFile(path.toStdString());
			std::string e;
			e=LFSTK_UtilityClass::LFSTK_getFullEntry("Desktop Entry","Icon",entrys,true);
			icon=QIcon::fromTheme(e.c_str());
		}
	else
		{
			if(type.name().contains("image"))
				{
					if((QFileInfo(path).size()<MAXIMAGESIZETOTHUMB) && (this->showThumbsInList==true))
						icon=QIcon(path);
					else
						icon=QIcon::fromTheme(type.iconName());
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


void chooserDialogClass::setSideListMode(QListView::ViewMode mode)
{
	this->sideList.setViewMode(mode);
	this->sideList.setGridSize(QSize(96,48));
	this->sideList.setResizeMode(QListView::Adjust);
}

void chooserDialogClass::setFileListMode(QListView::ViewMode mode)
{
	this->fileList.setViewMode(mode);
}

void chooserDialogClass::setFileList(void)
{
	QStandardItem	*item;

	this->fileListModel->clear();
	this->gFind.LFSTK_setIgnoreNavLinks(true);
	this->gFind.LFSTK_setIncludeHidden(this->showHidden);
	this->gFind.LFSTK_findFiles(this->localWD.toStdString().c_str());//TODO//
	this->gFind.LFSTK_sortByTypeAndName();

	item=new QStandardItem(QIcon(),"..");
	item->setData("..",Qt::UserRole);
	this->fileListModel->appendRow(item);

	for(int j=0;j<gFind.LFSTK_getDataCount();j++)
		{
			if(QFileInfo(this->localWD+"/"+gFind.data.at(j).name.c_str()).isSymLink()==true)
				{
					item=new QStandardItem(this->getFileIcon(this->localWD+"/"+gFind.data.at(j).name.c_str()),QString("->%1").arg(gFind.data.at(j).name.c_str()));
					item->setFont(QFont(item->font().family(),-1,QFont::Bold));
				}
			else
				item=new QStandardItem(this->getFileIcon(this->localWD+"/"+gFind.data.at(j).name.c_str()),gFind.data.at(j).name.c_str());
			item->setData(gFind.data.at(j).name.c_str(),Qt::UserRole);
			this->fileListModel->appendRow(item);
		}
	this->fileList.scrollToTop();
}

void chooserDialogClass::setSideList(void)
{
	QVariant				fullFilePathData;
	QStorageInfo			storage;
	QStandardItem		*item;
	QList<QStorageInfo>	ml=QStorageInfo::mountedVolumes();
	QString				disktype;

///standard items
	this->sideListModel->clear();
	item=new QStandardItem(QIcon::fromTheme("computer"),"Computer");
	fullFilePathData="/";
	item->setData(fullFilePathData,Qt::UserRole);
	this->sideListModel->appendRow(item);
	fullFilePathData=QDir::homePath();
	item=new QStandardItem(QIcon::fromTheme("user-home"),QFileInfo(QDir().homePath()).baseName());
	item->setData(fullFilePathData,Qt::UserRole);
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
					if(storage.fileSystemType().contains("ssh"))
						disktype="network_local";
					fullFilePathData=storage.rootPath();
					item=new QStandardItem(QIcon::fromTheme(disktype),QFileInfo(storage.rootPath()).baseName());
					item->setData(fullFilePathData,Qt::UserRole);
					this->sideListModel->appendRow(item);
				}
		}
//TODO//
//recent
//favs
}

void chooserDialogClass::showPreViewData(void)
{
	if(this->selectedFilePath.isEmpty()==true)
		return;

	QIcon			icon;
	QPixmap			pixmap;
	QMimeDatabase	db;
	QString			mod;
	int				md;
	struct stat		sb;
	QMimeType		type=db.mimeTypeForFile(this->selectedFilePath);

	this->previewMimeType.setText(type.name());

	if(type.name().contains("image"))
		{
			pixmap=QIcon(this->selectedFilePath).pixmap(QSize(128,128));
		}
	else
		{
			icon=this->getFileIcon(this->selectedFilePath);
			pixmap=icon.pixmap(QSize(128,128));
		}

	this->previewIcon.setPixmap(pixmap);
	this->previewSize.setText(QString("Size: %1").arg(QFileInfo(this->selectedFilePath).size()));

	if(lstat(this->selectedFilePath.toStdString().c_str(),&sb)!=-1)
		{
			md=sb.st_mode & 07777;
			mod.setNum(md,8);
			this->previewMode.setText(QString("Mode: %1").arg(mod));
		}
}

void chooserDialogClass::selectItem(const QModelIndex &index)
{
	QString t;
	t=QFileInfo(this->localWD+"/"+index.data(Qt::UserRole).toString()).absoluteFilePath();
	if(this->saveDialog==false)
		this->filepathEdit.setText(t);
	else
		this->filepathEdit.setText(index.data(Qt::UserRole).toString());

	this->selectedFileName=index.data(Qt::UserRole).toString();
	this->selectedFilePath=t;
	this->realFilePath=QFileInfo(this->selectedFilePath).canonicalFilePath();
	this->realFolderPath=QFileInfo(this->realFilePath).canonicalPath();
	this->realName=QFileInfo(this->realFilePath).fileName();;

	this->showPreViewData();
}

void chooserDialogClass::selectSideItem(const QModelIndex &index)
{
	QList<QStorageInfo>	ml=QStorageInfo::mountedVolumes();
	QStorageInfo			storage;
	QString				disktype;
	QPixmap				pixmap;
	QIcon				icon; 
	QLocale				locale;
	QString				type;
	qint64				sze=0;
	qint64				freeb=0;

	disktype="drive-harddisk";

	switch(index.row())
		{
			case 0:
				disktype="computer";
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
			default:
				storage=ml.at(index.row()+1);
				if((storage.rootPath().compare("/")!=0) && (storage.rootPath().compare(QDir().homePath())!=0))
					{
						if(storage.fileSystemType().contains("nfs"))
							disktype="folder-remote";
						if(storage.fileSystemType().contains("ssh"))
							disktype="network_local";	
						sze=storage.bytesTotal();
					type=storage.fileSystemType();
						freeb=storage.bytesFree();
					}
		}
	icon=QIcon::fromTheme(disktype);
	pixmap=icon.pixmap(QSize(128,128));
	this->previewIcon.setPixmap(pixmap);
	this->previewSize.setText(QString("Size: %1").arg(locale.formattedDataSize(sze)));
	this->previewMimeType.setText(QString("FS Type: %1").arg(type));
	this->previewMode.setText(QString("Free: %1").arg(locale.formattedDataSize(freeb)));
}

void chooserDialogClass::setFileData(void)
{
	QString				fp;
	QItemSelectionModel	*model;
	QModelIndexList		list;
	QString				filepath;

	this->localWD=QFileInfo(this->localWD).absoluteFilePath();
	if(this->saveDialog==true)
		{
			this->selectedFilePath=QFileInfo(this->localWD+"/"+this->filepathEdit.text()).absoluteFilePath();
			this->selectedFileName=QFileInfo(this->localWD+"/"+this->filepathEdit.text()).fileName();;

			this->realName=QFileInfo(QFileInfo(this->selectedFilePath).canonicalFilePath()).fileName();
			this->realFilePath=QFileInfo(this->selectedFilePath).canonicalFilePath();
		}
	else
		{
			this->selectedFilePath=QFileInfo(this->filepathEdit.text()).absoluteFilePath();
			this->selectedFileName=QFileInfo(this->selectedFilePath).fileName();

			this->realFilePath=QFileInfo(QFileInfo(this->filepathEdit.text()).absoluteFilePath()).canonicalFilePath();
			this->realName=QFileInfo(this->realFilePath).fileName();

		}

	fp=QFileInfo(this->selectedFilePath).canonicalFilePath();
	if(fp.isEmpty()==false)
		this->realFolderPath=QFileInfo(this->selectedFilePath).canonicalPath();
	else
		this->realFolderPath=QFileInfo(this->localWD).canonicalFilePath();

	this->fileExists=QFileInfo(this->realFilePath).exists();
	
	model=this->fileList.selectionModel();
	list=model->selectedIndexes();
	this->multiFileList.clear();

	for(int j=0;j<list.count();j++)
		{
			filepath=QFileInfo(this->localWD+"/"+list.at(j).data(Qt::UserRole).toString()).absoluteFilePath();
			if(QFileInfo(filepath).isDir()==false)
				this->multiFileList.push_back(filepath);
		}
}

void chooserDialogClass::buildMainGui(void)
{
	QVBoxLayout	*windowvlayout=new QVBoxLayout;
	QVBoxLayout	*filevlayout=new QVBoxLayout;
	QVBoxLayout	*sidevlayout=new QVBoxLayout;
	QVBoxLayout	*infovlayout=new QVBoxLayout;
	QVBoxLayout	*controlsvlayout=new QVBoxLayout;
	QHBoxLayout	*hlayout=new QHBoxLayout;

	this->dialogWindow.setWindowTitle("Select File");

	this->fileListModel=new QStandardItemModel(0,1);
    this->fileList.setModel(this->fileListModel);

	QObject::connect(&this->fileList,&QListView::clicked,[this](const QModelIndex &index)
		{
			if((this->saveDialog==true) && (QFileInfo(this->localWD+"/"+index.data(Qt::UserRole).toString()).isDir()==true))
				return;
			if(index.data(Qt::UserRole).toString().compare("..")!=0)
				this->selectItem(index);
		});

	QObject::connect(&this->fileList,&QListView::doubleClicked,[this](const QModelIndex &index)
		{
			QString t;
			QString dirp;
			if((this->localWD.compare("/")==0) && (index.data(Qt::UserRole).toString().compare("..")==0))
				{
					this->localWD="/";
					this->filepathEdit.setText(this->localWD);
					return;
				}

			if(this->localWD.compare("/")==0)
				t=this->localWD+index.data(Qt::UserRole).toString();
			else
				t=this->localWD+"/"+index.data(Qt::UserRole).toString();

			if(QFileInfo(t).isDir()==true)
				{
					dirp=QString("%1").arg(t);
					dirp=QDir::cleanPath(dirp);
					if(this->saveDialog==false)
						this->filepathEdit.setText(dirp);
					this->localWD=dirp;
					this->setFileList();
				}
			else
				{
					this->setFileData();
					this->dialogWindow.hide();
				}
		});

//sidelist
	QObject::connect(&this->sideList,&QListView::clicked,[this](const QModelIndex &index)
		{
				this->selectSideItem(index);
		});

	QObject::connect(&this->sideList,&QListView::doubleClicked,[this](const QModelIndex &index)
		{
			this->localWD=index.data(Qt::UserRole).toString();
			this->setFileList();
			if(this->saveDialog==false)
				this->filepathEdit.setText(this->localWD);
		});

	this->sideListModel=new QStandardItemModel(0,1);
    this->sideList.setModel(this->sideListModel);
	this->sideList.setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->fileList.setEditTriggers(QAbstractItemView::NoEditTriggers);

	sidevlayout->addWidget(&this->sideList);
	filevlayout->addWidget(&this->fileList);

	this->previewIcon.setMaximumWidth(128);
	this->previewIcon.setMinimumWidth(128);
	this->previewIcon.setAlignment(Qt::AlignCenter);

	hlayout->addLayout(sidevlayout,1);
	hlayout->addLayout(filevlayout,3);
	hlayout->addLayout(infovlayout);
	
	this->previewMimeType.setWordWrap(true);
	infovlayout->addWidget(&this->previewIcon);
	infovlayout->addWidget(&this->previewMimeType);
	infovlayout->addWidget(&this->previewSize);
	infovlayout->addWidget(&this->previewMode);
	infovlayout->addStretch();

	windowvlayout->addLayout(hlayout);

	controlsvlayout->addWidget(&this->filepathEdit);

	hlayout=new QHBoxLayout;
	QPushButton *cancel=new QPushButton("Cancel");
	cancel->setIcon(QIcon::fromTheme("stock_cancel"));
	QObject::connect(cancel,&QPushButton::clicked,[this]()
		{
			this->dialogWindow.hide();
		});

	QPushButton *hidden=new QPushButton("Hidden");
	hidden->setCheckable(true);
	hidden->setIcon(QIcon::fromTheme("stock_dialog_question"));
	QObject::connect(hidden,&QPushButton::clicked,[this]()
		{
			this->showHidden=!this->showHidden;
			this->setFileList();
		});

	QPushButton *newfolder=new QPushButton("New Folder");
	newfolder->setIcon(QIcon::fromTheme("folder-open"));
	QObject::connect(newfolder,&QPushButton::clicked,[this]()
		{
			bool		ok;
     		QString	text=QInputDialog::getText(&this->dialogWindow,"New Folder","New folder name",QLineEdit::Normal,"New Folder",&ok);
			if(ok==true)
				{
					QString	dirpath;
					QDir dirp(this->localWD);
					dirp.mkdir(text);
					this->localWD=QString("%1/%2").arg(this->localWD).arg(text);
					if(this->saveDialog==false)
						this->filepathEdit.setText(this->localWD);
					this->setFileList();
				}
		});

	QPushButton *apply;
	if(this->saveDialog==false)
		apply=new QPushButton("Apply");
	else
		apply=new QPushButton("Save");
	apply->setIcon(QIcon::fromTheme("stock_apply"));
	apply->setDefault(true);

	QObject::connect(apply,&QPushButton::clicked,[this]()
		{
			if(QFileInfo(this->selectedFilePath).isDir()==true)
				{
					this->localWD=selectedFilePath;
					this->setFileList();	
				}
			else
				{
					this->setFileData();
					this->dialogWindow.hide();
				}
		});

	hlayout->addWidget(cancel);
	hlayout->addStretch();
	hlayout->addWidget(hidden);
	hlayout->addStretch();
	hlayout->addWidget(newfolder);
	hlayout->addStretch();
	hlayout->addWidget(apply);
	controlsvlayout->addLayout(hlayout);
	windowvlayout->addLayout(controlsvlayout);

	this->dialogWindow.setLayout(windowvlayout);
	this->setSideList();
	this->setFileList();
	if(this->saveDialog==false)
		this->filepathEdit.setText(QFileInfo(this->localWD).absoluteFilePath());
	else
		{
			this->selectedFilePath=this->localWD+"/"+this->saveName;
			this->selectedFileName=this->saveName;
			this->filepathEdit.setText(this->saveName);
		}
}

chooserDialogClass::chooserDialogClass(QString folder,QString savename)
{
	if(folder.isEmpty()==true)
		this->localWD="/";
	else
		this->localWD=folder;

	this->saveDialog=!savename.isEmpty();
	this->saveName=savename;
	this->buildMainGui();
}
