/*
 *
 * ©K. D. Hedger. Tue  8 Mar 16:31:34 GMT 2022 keithdhedger@gmail.com

 * This file (QT_highlighter.cpp) is part of KKEditQT.

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

#include "QT_highlighter.h"
#include <QTextDocument>

bool Highlighter::setLanguage(QString lang)
{
	for(int j=0;j<this->langPlugins.count();j++)
		{
			if(this->langPlugins[j].langName.compare(lang)==0)
				{
					this->currentPlug=j;
					return(true);
				}
		}
	this->currentPlug=-1;
	return(false);
}

void Highlighter::resetRules(void)
{
	if(this->currentPlug==-1)
		return;
	this->highlightingRules.clear();

//new format
	this->langPlugins[this->currentPlug].instance->setLanguageRules(&(this->highlightingRules));

//mult line comment format
	this->langPlugins[this->currentPlug].instance->setMultLineFormatStart(&(this->multiLineCommentStart));
	this->langPlugins[this->currentPlug].instance->setMultLineFormatStop(&(this->multiLineCommentStop));
}

Highlighter::Highlighter(QTextDocument *parent,QPlainTextEdit *doc) : QSyntaxHighlighter(parent)
{
	this->loadLangPlugins();
	this->setLanguage("SH");//TODO//default??
	this->resetRules();
}

void Highlighter::highlightBlock(const QString &text)
{

	if(this->currentPlug==-1)
		return;
	QRegularExpression	startExpression(this->multiLineCommentStart.pattern);
	QRegularExpression	endExpression(this->multiLineCommentStop.pattern);
	int					startIndex=0;

	foreach (const highLightingRule &rule,highlightingRules)
		{
			QRegularExpression expression(rule.pattern);
			QRegularExpressionMatchIterator i=expression.globalMatch(text);
			while(i.hasNext())
				{
					QRegularExpressionMatch	match=i.next();
					setFormat(match.capturedStart(),match.capturedLength(),rule.format);
				}
		}

	setCurrentBlockState(0);

	int		offset=0;
	bool	addoffset=false;

	if(startExpression.pattern().compare(endExpression.pattern())==0)//fix for start/stop patterns the same ( a la bloody awaful python )
		offset=startExpression.pattern().length();

	if(previousBlockState()!=1)
		{
			startIndex=text.indexOf(startExpression);
			if(startIndex!=-1)
				{
					addoffset=true;
					startIndex+=offset;
				}
		}

	while(startIndex>=0)
		{
			QRegularExpressionMatch endMatch;
			int endIndex=text.indexOf(endExpression,startIndex,&endMatch);
			int commentLength;
			if(endIndex==-1)
				{
					setCurrentBlockState(1);
					commentLength=text.length()-startIndex;
				}
			else
				{
					commentLength=endIndex-startIndex+endMatch.capturedLength();
				}
			if(addoffset==false)
				setFormat(startIndex,commentLength,this->multiLineCommentStart.format);
			else
				setFormat(startIndex-offset,commentLength+offset,this->multiLineCommentStart.format);
			startIndex=text.indexOf(startExpression,startIndex + commentLength);
		}
}

bool Highlighter::loadLangPlug(langPluginStruct *ps)
{
	QObject	*plugininst=NULL;

	if(ps->loaded==true)
		return(true);

	ps->pluginLoader=new QPluginLoader(ps->plugPath);
	plugininst=ps->pluginLoader->instance();
	if(plugininst!=nullptr)
		{
			ps->instance=qobject_cast<SyntaxHighlitePluginInterface*>(plugininst);
			ps->instance->initPlug(ps->plugPath);//TODO//return false if cant init
			ps->loaded=true;
			ps->plugName=ps->pluginLoader->metaData().value("MetaData").toObject().value("name").toString();
			ps->langName=ps->pluginLoader->metaData().value("MetaData").toObject().value("langname").toString();
			ps->plugVersion=ps->pluginLoader->metaData().value("MetaData").toObject().value("version").toString();
			ps->mimeType=ps->pluginLoader->metaData().value("MetaData").toObject().value("mimetypes").toString();
		}
	else
		{
			ps->loaded=false;
			ps->broken=true;
			ps->plugName=QFileInfo(ps->plugPath).fileName();
			return(false);
		}

	return(true);
}

void Highlighter::loadLangPlugins(void)
{
	SyntaxHighlitePluginInterface	*plugtest;
	int 							cnt=0;

	QDir 							pluginsDir(QString("%1/langplugins/").arg(DATADIR));

	QDirIterator					git(pluginsDir.canonicalPath(),QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);
	while (git.hasNext())
		{
			QString				s=git.next();
			langPluginStruct	ps;

			ps.plugPath=s;
			if(this->loadLangPlug(&ps)==false)
				{
					qDebug() << "Error loading plug " << s;
					continue;
				}
			this->langPlugins[cnt++]=ps;
		}

	pluginsDir.setPath(QString("%1/.KKEditQT/langplugins").arg(pluginsDir.homePath()));
	QDirIterator					lit(pluginsDir.canonicalPath(),QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);
	while (lit.hasNext())
		{
			QString					s=lit.next();
			langPluginStruct		ps;

			ps.plugPath=s;
			if(this->loadLangPlug(&ps)==false)
				{
					qDebug() << "Error loading plug " << s;
					continue;
				}
			this->langPlugins[cnt++]=ps;
		}
}

void Highlighter::setTheme(QString themename)//TODO//load from file
{
	QString					themepath;
	QHash<int,themeStruct>	theme;
	QJsonParseError			errorPtr;
	QVariantList			localList;
	QVariantMap				map;
	QJsonDocument			doc;
    QVariantMap				mainMap;
    QByteArray				data;
	const char				*entrynames[]={"functions","class","types","comments","quotes","includes","numbers","keywords","custom","lanuageextra",NULL};

	themepath=QString("%1/themes/%2.json").arg(DATADIR).arg(themename);

	QFile	inFile(themepath);
    inFile.open(QIODevice::ReadOnly|QIODevice::Text);
    data=inFile.readAll();
    inFile.close();

    doc=QJsonDocument::fromJson(data,&errorPtr);
	if(doc.isNull())
    	{
			qDebug() << "Parse failed for " << themepath;
			return;
		}

	mainMap=doc.object().toVariantMap();
	int cnt=0;
	while(entrynames[cnt]!=NULL)
		{
			localList=mainMap[entrynames[cnt]].toList();    
 			map=localList[0].toMap();
			theme[cnt].colour=QColor(map["colour"].toString());
			if(map["bold"].toInt()==1)
				theme[cnt].weight=QFont::Bold;
			else
				theme[cnt].weight=QFont::Normal;
			theme[cnt].italic=map["italic"].toInt();
			cnt++;
		}

	localList=mainMap["document"].toList();    
	map=localList[0].toMap();

	this->docbackground=QString("QPlainTextEdit {background-color: %1;}").arg(map["colour"].toString());
	localList=mainMap["linenumbers"].toList();    
	map=localList[0].toMap();
	this->lineNumbersBackground=map["bgcolour"].toString();
	this->lineNumbersForeground=map["fgcolour"].toString();
	this->bookMarkBGColour=map["bmbgcolour"].toString();
	this->bookMarkFGColour=map["bmfgcolour"].toString();

	if(this->currentPlug!=-1)
		{
			this->langPlugins[this->currentPlug].instance->setTheme(theme);
			this->resetRules();
			this->rehighlight();
		}

	return;

}
