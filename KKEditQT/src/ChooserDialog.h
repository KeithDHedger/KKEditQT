/*
 *
 * ©K. D. Hedger. Fri 21 Aug 12:58:27 BST 2026 keithdhedger@gmail.com

 * This file (ChooserDialog.h) is part of KKEditQT.

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

#ifndef _CHOOSERDIALOG_
#define _CHOOSERDIALOG_

#include "qtincs.h"


#define MAXIMAGESIZETOTHUMB 2000000

enum class chooserDialogType{saveDialog,loadDialog,folderDialog};

class chooserDialogClass
{
	public:
		chooserDialogClass(chooserDialogType type,QString savename="Untitled",QString startfolder="");
		~chooserDialogClass();

		QDialog				dialogWindow;
		QVector<QString>		multiFileList;
		bool					valid=false;

		void					setShowImagesInList(bool show=false);
		void					setMultipleSelect(bool select);
		void					addFileTypes(QString types);

	private:

//main
		QString				selectedFolderPath="";
		QString				currentFolderPath="/";
		QComboBox			*folderCombo=NULL;
		QPushButton			*apply=NULL;

		QListView			fileList;
		QStandardItemModel	*fileListModel;
		QListView			sideList;
		QStandardItemModel	*sideListModel;

		QLineEdit			filepathEdit;
		QComboBox			fileTypes;

		QLabel				previewIcon;
		QLabel				previewMimeType;
		QLabel				previewSize;
		QLabel				previewMode;

		bool					useMulti=false;

		chooserDialogType	dialogType=chooserDialogType::loadDialog;
		void					buildMainGui(void);
		void					showPreViewData(QString file);
		void					doChoose(void);
		void					setFavs(void);
		void					setExitData(bool valid);
		void					getFilePermissions(QString filePath);
		void					fileEntryTextEdited(QString text);

//sidlist cbs
		QString				recentFoldersPath;
		QString				recentFilesPath;
		int					maxRecents=21;

		void					setSideList(void);
		void					selectSideItem(const QModelIndex &index);
		void					doubleClickSideList(const QModelIndex &index);

//filelist cbs
		bool					showHidden=false;
		bool					showThumbsInList=false;
		bool					fromRecents=false;

		QIcon				getFileIcon(QString path);
		void					doubleClickFileList(const QModelIndex &index);
		void					fileListSelectionChanged(void);
		void					setSelectedFiles(const QModelIndex &index,bool clear=false);
		void					setFileList(QString dir,QDir::SortFlags sortas=QDir::Name);
};


#endif
