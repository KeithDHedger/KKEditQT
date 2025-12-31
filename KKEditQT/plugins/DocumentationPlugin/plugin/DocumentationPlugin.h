/*
 *
 * ©K. D. Hedger. Fri  5 Jul 11:17:11 BST 2024 keithdhedger@gmail.com

 * This file (DocumentationPlugin.h) is part of DocumentationPlugin.

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

#ifndef DOCUMENTATIONPLUG_H
#define DOCUMENTATIONPLUG_H

#include "globalincludes.h"

enum {MENUNAME=0,COMMAND,SHOWITEM,SEARCHALL,SETTINGSCNT};

class DocumentationPlugin : public QObject, kkEditQTPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.Documentation.kkEditQTPluginInterface" FILE "DocumentationPlugin.json")
	Q_INTERFACES(kkEditQTPluginInterface)

	public:
		void				initPlug(KKEditClass *kk,QString pathtoplug) override;
		void				unloadPlug(void) override;
		void				plugAbout(void) override;
		void				plugSettings(void) override;
		void				plugRun(plugData *data) override;
		unsigned			int	plugWants(void) override;

	private:
		KKEditClass		*mainKKEditClass=NULL;
		QString			plugPath;
		plugData			*data=NULL;
		DocumentClass	*doc=NULL;
		QMenu			*apiMenu=NULL;
		QAction			*doxyMenu=NULL;
		QAction			*docsetMenu=NULL;
		QAction			*showDocsMenu=NULL;
		QString			customCommand="";
		QAction			*customCommandMenu=NULL;
		bool				noStatusInfo=true;
		QStringList		resList;
		QString			filePath="";
		QString			folderPath="";
		QString			tempFolderPath="";

		QString					runPipeAndCapture(QString command);
		void						runSearch(QString command);
		void						runAllSearchs(void);
		void						buildDocumentation(void);
		void						buildDocset(void);
		miniPrefsReturnStruct	miniPrefsDialog(QString prefsname,QStringList items,bool liveupdate=false);
		void						showBarberPole(QString windowtitle,QString bodylabel,QString cancellabel,QString maxitems,QString controlfile);
		void						showDocs(void);
		void						findInDocs(void);
		void						searchDoxyDocs(const QString txt);
};

#endif
