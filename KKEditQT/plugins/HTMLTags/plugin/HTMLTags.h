/*
 *
 * ©K. D. Hedger. Wed 18 Sep 12:56:26 BST 2024 keithdhedger@gmail.com

 * This file (HTMLTags.h) is part of HTMLTags.

 * HTMLTags is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * HTMLTags is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with HTMLTags.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HTMLTAGSPLUG_H
#define HTMLTAGSPLUG_H

#include "../../../src/kkeditqtPluginInterface.h"

class HTMLTags : public QObject, kkEditQTPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.kkEditQTPluginInterface" FILE "HTMLTags.json")
	Q_INTERFACES(kkEditQTPluginInterface)

	public:
		void				initPlug(KKEditClass *kk,QString pathtoplug) override;
		void				unloadPlug(void) override;
		void				plugAbout(void) override;
		void				plugSettings(void) override {};
		void				plugRun(plugData *data) override;
		unsigned int		plugWants(void) override;

	private:
		QMenu			*htmlContextMenu=NULL;
		QString			runPipeAndCapture(QString command);
		QString			rootMenuName;
		DocumentClass 	*document;
		QString			selection;
		QTextCursor		tc;
		KKEditClass		*mainKKEditClass;
		QString			plugPath;
};

#endif
