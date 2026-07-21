/*
 *
 * ©K. D. Hedger. Sun 19 Jul 14:10:41 BST 2026 keithdhedger@gmail.com

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

enum class chooserDialogType{saveDialog,loadDialog};

class chooserDialogClass
{
	public:
		chooserDialogClass(chooserDialogType type,QString savename="",QString startfolder="");
		~chooserDialogClass();

		QDialog				dialogWindow;
		QString				localWD;
		QString				selectedFileName;
		QString				selectedFilePath;
		QString				realFolderPath;
		QString				realName;
		QString				realFilePath;
		QVector<QString>		multiFileList;
		bool					fileExists=false;
		bool					useMulti=false;
		bool					valid=false;

		void					setShowImagesInList(bool show=false);
		void					setMultipleSelect(bool select);
		void					addFileTypes(QString types);

	private:
		QLineEdit			filepathEdit;
		QComboBox			fileTypes;
		QListView			fileList;
		QStandardItemModel	*fileListModel;

		QListView			sideList;
		QStandardItemModel	*sideListModel;

		QLabel				previewIcon;
		QLabel				previewMimeType;
		QLabel				previewSize;
		QLabel				previewMode;
		QLabel				previewFileName;
		QString				saveName;

		bool					showHidden=false;
		bool					showThumbsInList=false;
		bool					saveDialog=false;
		bool					overwriteWarning=true;
		QString				lastSaveFolder;
		QString				lastLoadFolder;
		int					maxRecents=21;
		QString				recentFoldersPath;
		QString				recentFilesPath;

		void					buildMainGui(void);
		void					setSideList(void);
		void					setFileList(void);
		QIcon				getFileIcon(QString path);
		void					selectItem(const QModelIndex &index);
		void					selectSideItem(const QModelIndex &index);
		void					showPreViewData(void);
		void					setFileData(void);
		void					setFavs(void);
};

#endif
