/*
 *
 * ©K. D. Hedger. Sun  4 Feb 19:39:43 GMT 2024 keithdhedger@gmail.com

 * This file (QT5ChooserDialog.h) is part of QT5FileDialog.

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

#ifndef _CHOOSERDIALOG_
#define _CHOOSERDIALOG_

#include <QWidget>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QListView>
#include <QStringListModel>
#include <QDebug>
#include <QStorageInfo>
#include <QVariant>
#include <QLineEdit>
#include <QPushButton>

#include <QStandardItemModel>
#include <QMimeDatabase>
#include <QInputDialog>

#include "LFSTKFindClass.h"
#include "LFSTKUtilityClass.h"

#define MAXIMAGESIZETOTHUMB 1000000

class chooserDialogClass
{
	public:
		chooserDialogClass(QString folder="/",QString savename="");
		~chooserDialogClass();

		QDialog				dialogWindow;
		QString				localWD;
		QString				selectedFileName;
		QString				selectedFilePath;
		QString				realFolderPath;
		QString				realName;
		QString				realFilePath;
		QVector<QString>		multiFileList;
		LFSTK_findClass		gFind;
		bool					fileExists=false;
		bool					useMulti=false;

		void					setSideListMode(QListView::ViewMode mode);
		void					setFileListMode(QListView::ViewMode mode);
		void					setShowImagesInList(bool show=false);
		void					setSaveDialog(bool save=false);
		void					setMultipleSelect(bool select);

	private:
		QLineEdit			filepathEdit;
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

		void					buildMainGui(void);
		void					setSideList(void);
		void					setFileList(void);
		QIcon				getFileIcon(QString path);
		void					selectItem(const QModelIndex &index);
		void					selectSideItem(const QModelIndex &index);
		void					showPreViewData(void);
		void					setFileData(void);
};
#endif