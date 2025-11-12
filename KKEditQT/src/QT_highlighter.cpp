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

#include "qtincs.h"
#include "QT_notebook.h"
#include "QT_themeClass.h"
#include "QT_highlighter.h"

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

Highlighter::Highlighter(QTextDocument *parent,QPlainTextEdit *doc,KKEditClass *kk) : QSyntaxHighlighter(parent)
{
	this->mainKKEditClass=kk;
	this->loadLangPlugins();
	this->loadToolkitPlugins();
	this->setLanguage("SH");
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

			if(this->getBit(prev,closecomment)==true)
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
	int				cnt=0;
	QDir				pluginsDir(QString("%1/.KKEditQT/langplugins").arg(pluginsDir.homePath()));
	QDirIterator		lit(pluginsDir.canonicalPath(),QStringList("*.so"), QDir::Files,QDirIterator::Subdirectories);

	if(this->mainKKEditClass->verySafeFlag==true)
		return;

	while(lit.hasNext())
		{
			QString			s=lit.next();
			langPluginStruct	ps;

			ps.plugPath=s;
			if(this->loadLangPlug(&ps)==false)
				{
					qWarning() << "Error loading plug " << s;
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
					qWarning() << "Error loading plug " << s;
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
	int				cnt=0;
	QDir				pluginsDir(QString("%1/toolkitplugins/").arg(DATADIR));
	QDirIterator		lit(pluginsDir.canonicalPath(),QStringList("*.so"),QDir::Files,QDirIterator::Subdirectories);

	while(lit.hasNext())
		{
			QString					s=lit.next();
			langPluginStruct	ps;

			ps.plugPath=s;
			if(this->loadToolkitPlug(&ps)==false)
				{
					qWarning() << "Error loading plug " << s;
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
					qWarning() << "Error loading plug " << s;
					continue;
				}
			this->toolkitPlugins[cnt++]=ps;
		}
}

void Highlighter::setTheme(void)
{
	if(this->mainKKEditClass->verySafeFlag==true)
		{
			this->documentBackground="#ffffff";
			this->documentForeground="#000000";
			this->docBackgroundCSS=QString("QPlainTextEdit {background-color: %1; color: %2;}").arg(this->documentBackground).arg(this->documentForeground);

			this->findBGColour="#800000";
			this->findFGColour="#ffffff";

			this->lineNumbersBackground="#c0c0c0";
			this->lineNumbersForeground="#000000";

			this->bookMarkBGColour="#a0a0a4";
			this->bookMarkFGColour=this->lineNumbersForeground;
			this->mainKKEditClass->theme->themeParts["insertcolour"].colour=QColor("blue");
			return;
		}
	this->documentBackground=this->mainKKEditClass->theme->themeParts.value("docbgcolour").colourString;
	this->documentForeground=this->mainKKEditClass->theme->themeParts.value("docfgcolour").colourString;
	this->docBackgroundCSS=QString("QPlainTextEdit {background-color: %1; color: %2;}").arg(this->documentBackground).arg(this->documentForeground);

	this->findBGColour=this->mainKKEditClass->theme->themeParts.value("findbgcol").colourString;
	this->findFGColour=this->mainKKEditClass->theme->themeParts.value("findfgcol").colourString;

	this->lineNumbersBackground=this->mainKKEditClass->theme->themeParts.value("linebgcolour").colourString;
	this->lineNumbersForeground=this->mainKKEditClass->theme->themeParts.value("linefgcolour").colourString;

	this->bookMarkBGColour=this->mainKKEditClass->theme->themeParts.value("bookmarkbgcolour").colourString;
	this->bookMarkFGColour=this->lineNumbersForeground;

	if(this->currentPlug!=-1)
		{
			for(int j=0;j<this->toolkitPlugins.count();j++)
				this->toolkitPlugins[j].instanceTK->setTheme(this->mainKKEditClass->theme->themeParts);

			const QSignalBlocker block(this->document);
			this->langPlugins[this->currentPlug].instance->setTheme(this->mainKKEditClass->theme->themeParts);
			this->resetRules();
			this->rehighlight();
		}

	this->resetformat.setForeground(QColor(this->documentForeground));
	this->resetformat.setFontWeight(0);
	this->resetformat.setFontItalic(false);
}


