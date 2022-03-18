/*
 *
 * ©K. D. Hedger. Tue  8 Mar 16:31:25 GMT 2022 keithdhedger@gmail.com

 * This file (QT_highlighter.h) is part of KKEditQT.

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

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QPlainTextEdit>

#include "SyntaxHighlitePluginInterface.h"

struct langPluginStruct
{
	QPluginLoader					*pluginLoader=NULL;
	QString							plugPath="";
	QString							plugName="";
	QString							langName="";
	QString							plugVersion="";
	QString							mimeType="";
	SyntaxHighlitePluginInterface	*instance=NULL;
	bool							loaded=false;
	bool							broken=false;
};

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	Highlighter(QTextDocument *parent,QPlainTextEdit *doc);
	bool						setLanguage(QString lang);
	void						setTheme(QString themename);
//plugins
//vars
	QHash<int,langPluginStruct>	langPlugins;

	QString						docBackgroundCSS;
	QString						lineNumbersBackground;
	QString						lineNumbersForeground;
	QString						bookMarkBGColour;
	QString						bookMarkFGColour;

	bool						syntaxHighlighting=true;
protected:
	void						highlightBlock(const QString &text);

private:

	QVector<highLightingRule>	highlightingRules;
	QPlainTextEdit				*document;

	highLightingRule			multiLineCommentStart;
	highLightingRule			multiLineCommentStop;

	int							currentPlug=-1;
//functions
	bool						loadLangPlug(langPluginStruct *ps);
	void						loadLangPlugins(void);
	void						resetRules(void);

};


#endif // HIGHLIGHTER_H
