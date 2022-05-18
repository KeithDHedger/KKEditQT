/*
 *
 * ©K. D. Hedger. Wed 16 Mar 14:07:43 GMT 2022 keithdhedger@gmail.com

 * This file (jslang.cpp) is part of jslang.

 * jslang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * jslang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with jslang.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "jslang.h"

void jslang::initPlug(QString pathtoplug)
{
	themeStruct	blank={Qt::black,QFont::Normal,false};

	this->plugPath=pathtoplug;
	if(this->theme.count()==0)
		for(int j=FUNCTIONTHEME;j<THEMEDONE;j++)
			this->theme[j]=blank;
}

void jslang::unloadPlug(void)
{
}

//new format
void jslang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//functions
	hr.format.setFontItalic(this->theme[FUNCTIONTHEME].italic);
	hr.format.setFontWeight(this->theme[FUNCTIONTHEME].weight);
	hr.format.setForeground(this->theme[FUNCTIONTHEME].colour);
	hr.pattern=QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
	rules->append(hr);

//quotes
	hr.format.setForeground(this->theme[QUOTESTHEME].colour);
	hr.format.setFontWeight(this->theme[QUOTESTHEME].weight);
	hr.format.setFontItalic(this->theme[QUOTESTHEME].italic);
	hr.pattern = QRegularExpression("(\"([^\"\\\\]*(\\\\.[^\"\\\\]*)*)\")|('([^'\\\\]*(\\\\.[^'\\\\]*)*)')");
	rules->append(hr);

//numbers
	hr.format.setForeground(this->theme[NUMBERTHEME].colour);
	hr.format.setFontWeight(this->theme[NUMBERTHEME].weight);
	hr.format.setFontItalic(this->theme[NUMBERTHEME].italic);
	hr.pattern=QRegularExpression(NUMBERSREGEX);
	rules->append(hr);

//keywords
	hr.format.setForeground(this->theme[KEYWORDTHEME].colour);
	hr.format.setFontWeight(this->theme[KEYWORDTHEME].weight);
	hr.format.setFontItalic(this->theme[KEYWORDTHEME].italic);
	hr.pattern=QRegularExpression("\\b(let|abstract|break|case|catch|const|continue|debugger|default|delete|do|else|enum|export|extends|final|finally|for|function|goto|if|implements|in|instanceof|interface|native|new|private|protected|prototype|public|return|super|switch|synchronized|throw|throws|this|transient|try|typeof|var|volatile|while|with)\\b");
	rules->append(hr);

//types
	hr.format.setForeground(this->theme[TYPETHEME].colour);
	hr.format.setFontWeight(this->theme[TYPETHEME].weight);
	hr.format.setFontItalic(this->theme[TYPETHEME].italic);
	hr.pattern=QRegularExpression("\\b(NULL|null|true|false|null|undefined)\\b");
	rules->append(hr);

//custom
	hr.format.setForeground(this->theme[CUSTOMTHEME].colour);
	hr.format.setFontWeight(this->theme[CUSTOMTHEME].weight);
	hr.format.setFontItalic(this->theme[CUSTOMTHEME].italic);
	hr.pattern=QRegularExpression("\\b(Infinity|Math|NaN|NEGATIVE_INFINITY|POSITIVE_INFINITY)\\b");
	rules->append(hr);

//java script extras
	hr.format.setForeground(this->theme[LANGUAGEEXTRAS].colour);
	hr.format.setFontWeight(this->theme[LANGUAGEEXTRAS].weight);
	hr.format.setFontItalic(this->theme[LANGUAGEEXTRAS].italic);
	hr.pattern=QRegularExpression("\\b(abstract|boolean|byte|char|class|debugger|double|enum|extends|final|float|goto|implements|interface|int|long|native|package|private|protected|public|short|static|super|synchronized|throws|transient|volatile)\\b");
	rules->append(hr);

//single line comment
//	hr.format.setForeground(this->theme[COMMENTTHEME].colour);
//	hr.format.setFontWeight(this->theme[COMMENTTHEME].weight);
//	hr.format.setFontItalic(this->theme[COMMENTTHEME].italic);
//	hr.pattern=QRegularExpression("//[^\n]*");
//	rules->append(hr);
	hr.format.setForeground(this->theme[COMMENTTHEME].colour);
	hr.format.setFontWeight(this->theme[COMMENTTHEME].weight);
	hr.format.setFontItalic(this->theme[COMMENTTHEME].italic);
	hr.type="comment";
	hr.customRule=true;
	hr.pattern=QRegularExpression(".*");
	rules->append(hr);
}

//odd single formats set to "" for no multiline comment
void jslang::setMultLineFormatStart(highLightingRule *hr)
{
	hr->format.setForeground(this->theme[COMMENTTHEME].colour);
	hr->format.setFontWeight(this->theme[COMMENTTHEME].weight);
	hr->format.setFontItalic(this->theme[COMMENTTHEME].italic);
	hr->pattern = QRegularExpression("/\\*");
}

void jslang::setMultLineFormatStop(highLightingRule *hr)
{
	hr->pattern = QRegularExpression("\\*/");
}

void jslang::setTheme(QHash<int,themeStruct> newtheme)
{
	this->theme=newtheme;
}

void jslang::runCustomRule(QString text,highLightingRule *hr)
{
	if(hr->type.compare("comment")==0)
		{
			bool		inquote=false;
			int		cnt=0;
			QChar	openquote=0;

			while(cnt<text.length())
				{
					if(text.at(cnt)=='\\')
						{
							cnt+=2;
							continue;
						}

					if(openquote==0)
						{
							if(text.at(cnt)=='"')
								openquote='"';
							if(text.at(cnt)=='\'')
								openquote='\'';
						}

					if(text.at(cnt)==openquote)
						{
							inquote=!inquote;
							if(inquote==false)
								openquote=0;
							cnt++;
							continue;
						}

					if((inquote==false) && (text.mid(cnt,2).compare("//")==0))
						{
							hr->start=cnt;
							hr->len=text.length()-cnt;
							return;
						}
					cnt++;
				}
		}
}
