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

#ifndef SYNTAXHIGHLITEPLUGININTERFACE_H
#define SYNTAXHIGHLITEPLUGININTERFACE_H

#include <QtCore>
#include <QTextFormat>

#include "PluginCommon.h"

class SyntaxHighlitePluginInterface
{
	public:
		virtual			~SyntaxHighlitePluginInterface()=default;

		virtual void		initPlug(QString pathtoplug)=0;
		virtual void		unloadPlug(void)=0;
//theme
		virtual void		setTheme(QMap<QString,partsStruct> newtheme)=0;

//new rules format
		virtual void		setLanguageRules(QVector<highLightingRule> *rules)=0;
//multi line rules
		virtual void		setMultLineRules(QVector<highLightingRule> *rules)=0;

//custom code for edge cases etc
		virtual void		runCustomRule(QString text,highLightingRule *hr)=0;
};

QT_BEGIN_NAMESPACE

#define SyntaxHighlitePluginInterface_iid "org.KKEditQT.SyntaxHighlitePluginInterface"

Q_DECLARE_INTERFACE(SyntaxHighlitePluginInterface, SyntaxHighlitePluginInterface_iid)
QT_END_NAMESPACE

#endif


