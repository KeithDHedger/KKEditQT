/*
 *
 * ©K. D. Hedger. Tue  8 Mar 16:36:35 GMT 2022 keithdhedger@gmail.com

 * This file (SyntaxHighlitePluginInterface.h) is part of KKEditQT.

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

#ifndef TOOLKITHIGHLITEPLUGININTERFACE_H
#define TOOLKITHIGHLITEPLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QTextCharFormat>
#include <QRegularExpression>

#include "PluginCommon.h"

class ToolkitHighlitePluginInterface
{
	public:
		virtual			~ToolkitHighlitePluginInterface()=default;

		virtual void		initPlug(QString pathtoplug)=0;
		virtual void		unloadPlug(void)=0;
//theme
		virtual void		setTheme(QMap<QString,partsStruct> newtheme)=0;

//new rules format
		virtual void		setToolkitRules(QVector<highLightingRule> *rules)=0;
};

#define ToolkitHighlitePluginInterface_iid "org.KKEditQT.ToolkitHighlitePluginInterface"
Q_DECLARE_INTERFACE(ToolkitHighlitePluginInterface,ToolkitHighlitePluginInterface_iid)

#endif

