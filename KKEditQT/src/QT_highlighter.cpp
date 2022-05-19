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

//toolkit
	for(int j=0;j<this->toolkitPlugins.count();j++)
		{
			if(this->toolkitPlugins[j].langName.contains(this->langPlugins[this->currentPlug].langName)==true)
				this->toolkitPlugins[j].instanceTK->setToolkitRules(&(this->highlightingRules));
		}

//new format
	this->langPlugins[this->currentPlug].instance->setLanguageRules(&(this->highlightingRules));
//multi line rules
	this->langPlugins[this->currentPlug].instance->setMultLineRules(&(this->multiLineCommentRules));
}

Highlighter::Highlighter(QTextDocument *parent,QPlainTextEdit *doc) : QSyntaxHighlighter(parent)
{
	this->loadLangPlugins();
	this->loadToolkitPlugins();
	this->setLanguage("SH");//TODO//default??
	this->resetRules();
	this->document=doc;
}


void Highlighter::setBit(int *data,int bit)
{
	*data=*data | (1<<bit);
}

void Highlighter::resetBit(int *data,int bit)
{
	*data=*data & ~(-1 & (1<<bit));
}

int Highlighter::getBit(int data,int bit)
{
	return((data & (1<<bit)) && true);
}

void Highlighter::highlightBlock(const QString &text)
{
	int	closecomment;
	int	prev;
	int	startIndex;
	int	offset;
	bool	addoffset;

	if(this->currentPlug==-1)
		return;
	if(this->syntaxHighlighting==false)
		return;

	for(int j=0;j<this->highlightingRules.count();j++)
		{
			highLightingRule rule=highlightingRules[j];
			QRegularExpressionMatchIterator i=rule.pattern.globalMatch(text);
			while(i.hasNext())
				{
					QRegularExpressionMatch	match=i.next();
					if(rule.customRule==true)
						{
							this->langPlugins[this->currentPlug].instance->runCustomRule(text,&rule);
							setFormat(rule.start,rule.len,rule.format);
						}
					else
						{
							setFormat(match.capturedStart(),match.capturedLength(),rule.format);
						}
				}
		}

	this->setCurrentBlockState(-1);
	for(int j=0;j<this->multiLineCommentRules.count();j++)
		{
			QRegularExpression	startExpression(this->multiLineCommentRules[j].pattern);
			QRegularExpression	endExpression(this->multiLineCommentRules[j].endPattern);

			offset=0;
			addoffset=false;

			prev=this->previousBlockState();
			startIndex=0;
			closecomment=j;
			if(startExpression.pattern().length()==0)
				continue;

			if(startExpression.pattern().compare(endExpression.pattern())==0)//fix for start/stop patterns the same ( a la bloody awaful python )
				offset=startExpression.pattern().length();

			if(getBit(prev,closecomment)==true)
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
							int cbs=this->currentBlockState();
							resetBit(&cbs,closecomment);
							this->setCurrentBlockState(cbs);
							commentLength=text.length()-startIndex;
						}
					else
						{
							commentLength=endIndex-startIndex+endMatch.capturedLength();
						}
					if(addoffset==false)
						setFormat(startIndex,commentLength,this->multiLineCommentRules[j].format);
					else
						setFormat(startIndex-offset,commentLength+offset,this->multiLineCommentRules[j].format);
					startIndex=text.indexOf(startExpression,startIndex + commentLength);
				}
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
	int 								cnt=0;

	QDir 							pluginsDir(QString("%1/.KKEditQT/langplugins").arg(pluginsDir.homePath()));

	QDirIterator						lit(pluginsDir.canonicalPath(),QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);
	while(lit.hasNext())
		{
			QString			s=lit.next();
			langPluginStruct	ps;

			ps.plugPath=s;
			if(this->loadLangPlug(&ps)==false)
				{
					qDebug() << "Error loading plug " << s;
					continue;
				}
			this->langPlugins[cnt++]=ps;
		}

	pluginsDir.setPath(QString("%1/langplugins/").arg(DATADIR));
	QDirIterator					git(pluginsDir.canonicalPath(),QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);
	while (git.hasNext())
		{
			QString				s=git.next();
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

bool Highlighter::loadToolkitPlug(langPluginStruct *ps)
{
	QObject	*plugininst=NULL;

	if(ps->loaded==true)
		return(true);

	ps->pluginLoader=new QPluginLoader(ps->plugPath);
	plugininst=ps->pluginLoader->instance();
	if(plugininst!=nullptr)
		{
			ps->instanceTK=qobject_cast<ToolkitHighlitePluginInterface*>(plugininst);
			ps->instanceTK->initPlug(ps->plugPath);//TODO//return false if cant init
			ps->loaded=true;
			ps->plugName=ps->pluginLoader->metaData().value("MetaData").toObject().value("name").toString();
			ps->langName=ps->pluginLoader->metaData().value("MetaData").toObject().value("usefor").toString();
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

void Highlighter::loadToolkitPlugins(void)
{
	ToolkitHighlitePluginInterface	*plugtest;
	int 								cnt=0;
	//QDir 							pluginsDir(QString("%1/.KKEditQT/toolkitplugins").arg(pluginsDir.homePath()));
	QDir 							pluginsDir(QString("%1/toolkitplugins/").arg(DATADIR));
	QDirIterator						lit(pluginsDir.canonicalPath(),QStringList("*.so"),QDir::Files,QDirIterator::Subdirectories);

	while(lit.hasNext())
		{
			QString					s=lit.next();
			langPluginStruct	ps;

			ps.plugPath=s;
			if(this->loadToolkitPlug(&ps)==false)
				{
					qDebug() << "Error loading plug " << s;
					continue;
				}
			this->toolkitPlugins[cnt++]=ps;
		}

	//pluginsDir.setPath(QString("%1/toolkitplugins/").arg(DATADIR));
	pluginsDir.setPath(QString("%1/.KKEditQT/toolkitplugins").arg(pluginsDir.homePath()));
	QDirIterator						git(pluginsDir.canonicalPath(),QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);
	while(git.hasNext())
		{
			QString					s=git.next();
			langPluginStruct			ps;

			ps.plugPath=s;
			if(this->loadToolkitPlug(&ps)==false)
				{
					qDebug() << "Error loading plug " << s;
					continue;
				}
			this->toolkitPlugins[cnt++]=ps;
		}
}

void Highlighter::setTheme(QString themename)
{
	QString					themepath;
	QHash<int,themeStruct>	theme;
	QJsonParseError			errorPtr;
	QVariantList			localList;
	QVariantMap				map;
	QJsonDocument			doc;
    QVariantMap				mainMap;
    QByteArray				data;
	const char				*entrynames[]={"functions","class","types","comments","quotes","includes","numbers","keywords","custom","lanuageextra","variables","toolkit",NULL};

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
	this->docBackgroundCSS=QString("QPlainTextEdit {background-color: %1; color: %2;}").arg(map["bgcolour"].toString()).arg(map["fgcolour"].toString());
	this->documentBackground=map["bgcolour"].toString();
	this->documentForeground=map["fgcolour"].toString();
	localList=mainMap["linenumbers"].toList();    
	map=localList[0].toMap();
	this->lineNumbersBackground=map["bgcolour"].toString();
	this->lineNumbersForeground=map["fgcolour"].toString();
	this->bookMarkBGColour=map["bmbgcolour"].toString();
	this->bookMarkFGColour=map["bmfgcolour"].toString();

	if(this->currentPlug!=-1)
		{
			for(int j=0;j<this->toolkitPlugins.count();j++)
				this->toolkitPlugins[j].instanceTK->setTheme(theme);

			const QSignalBlocker block(this->document);
			this->langPlugins[this->currentPlug].instance->setTheme(theme);
			this->resetRules();
			this->rehighlight();
		}

	this->resetformat.setForeground(QColor(this->documentForeground));
	this->resetformat.setFontWeight(0);
	this->resetformat.setFontItalic(false);

	return;
}






















