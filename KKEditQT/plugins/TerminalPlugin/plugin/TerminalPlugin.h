/*
 *
 * ©K. D. Hedger. Sat 26 Mar 15:11:45 GMT 2022 keithdhedger@gmail.com

 * This file (TerminalPlugin.h) is part of TerminalPlugin.

 * TerminalPlugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * TerminalPlugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with TerminalPlugin.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TerminalPluginPLUG_H
#define TerminalPluginPLUG_H

#include "../../../src/kkeditqtPluginInterface.h"
#include "../../../../config.h"

#ifdef _USEQT6_
#include <qtermwidget6/qtermwidget.h>
#else
#include <qtermwidget5/qtermwidget.h>
#endif
#include <QSettings>

struct termDataStruct
{
	QTermWidget	*console;
	QDockWidget	*dockWidget;
	QAction		*toggleTerm;
};

class TerminalPluginPlug : public QObject, kkEditQTPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.kkEditQTPluginInterface" FILE "TerminalPlugin.json")
	Q_INTERFACES(kkEditQTPluginInterface)

	public:
		void						initPlug(KKEditClass *kk,QString pathtoplug) override;
		void						unloadPlug(void) override;
		void						plugAbout(void) override;
		void						plugSettings(void) override;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
		void						plugRun(plugData *data) override;
#pragma GCC diagnostic pop
		unsigned int				plugWants(void) override;

	private:
		KKEditClass				*mainKKEditClass;
		QString					plugPath;
		QMenu					*TerminalPluginMenu;
		QMenu					*terminalsMenu;
		QAction					*cdToAct;
		QAction					*toggleViewAct;
		QAction					*newAct;
		QAction					*toggleTabsAct;

		QTermWidget				*console;
		QDockWidget				*dw;

		QVector<termDataStruct>	terminals;
		int						currentTerminal=0;

		int						cbnum=0;
		bool						openOnStart=false;
		bool						saveCurrentVis=false;

		QString					filePath;
		QString					folderPath;
		void						doMenuItem(int what,int whome);
		void						addTerminal(void);
};

#endif
