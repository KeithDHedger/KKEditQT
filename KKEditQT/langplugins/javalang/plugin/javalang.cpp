/*
 *
 * ©K. D. Hedger. Sat  2 Apr 15:09:51 BST 2022 keithdhedger@gmail.com

 * This file (javalang.cpp) is part of javalang.

 * javalang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * javalang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with javalang.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "javalang.h"

void javalang::initPlug(QString pathtoplug)
{
	this->plugPath=pathtoplug;
}

void javalang::unloadPlug(void)
{
}

//new format
void javalang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//functions
	hr.format.setForeground(this->theme.value("functions").colour);
	hr.format.setFontItalic(this->theme.value("functions").italic);
	if(this->theme.value("functions").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b[[:word:]\\.]*[[:word:]_]+(?=\\()");
	rules->append(hr);

//classes
	hr.format.setForeground(this->theme.value("class").colour);
	hr.format.setFontItalic(this->theme.value("class").italic);
	if(this->theme.value("class").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("(class|enum|extends|implements|instanceof|interface|native|throws|private|protected|public)");
	rules->append(hr);

//quotes
	hr.format.setForeground(this->theme.value("quotes").colour);
	hr.format.setFontItalic(this->theme.value("quotes").italic);
	if(this->theme.value("quotes").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern = QRegularExpression("(\"([^\"\\\\]*(\\\\.[^\"\\\\]*)*)\")|('\\\\.')|('.')");
	rules->append(hr);

//numbers
	hr.format.setForeground(this->theme.value("numbers").colour);
	hr.format.setFontItalic(this->theme.value("numbers").italic);
	if(this->theme.value("numbers").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression(NUMBERSREGEX);
	rules->append(hr);

//keywords
	hr.format.setForeground(this->theme.value("keywords").colour);
	hr.format.setFontItalic(this->theme.value("keywords").italic);
	if(this->theme.value("keywords").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b(assert|break|case|catch|continue|default|do|else|finally|for|if|return|throw|switch|try|while|new|super|this|goto|abstract|final|static|strictfp|synchronized|transient|volatile|class|enum|extends|implements|instanceof|interface|native|throws)\\b");
	rules->append(hr);

//includes
	hr.format.setForeground(this->theme.value("includes").colour);
	hr.format.setFontItalic(this->theme.value("includes").italic);
	if(this->theme.value("includes").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b(import|package)\\b");
	rules->append(hr);

//types
	hr.format.setForeground(this->theme.value("types").colour);
	hr.format.setFontItalic(this->theme.value("types").italic);
	if(this->theme.value("types").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b(boolean|byte|char|double|float|int|long|short|void)\\b");
	rules->append(hr);

//java extras
	hr.format.setForeground(this->theme.value("custom").colour);
	hr.format.setFontItalic(this->theme.value("custom").italic);
	if(this->theme.value("custom").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b(abstract|final|static|strictfp|synchronized|transient|volatile)\\b");
	rules->append(hr);

//single line comment
	hr.format.setForeground(this->theme.value("comments").colour);
	hr.format.setFontItalic(this->theme.value("comments").italic);
	if(this->theme.value("comments").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.type="comment";
	hr.customRule=true;
	hr.pattern=QRegularExpression(".*");
	rules->append(hr);
}

void javalang::setMultLineRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;
///**/
	hr.format.setFontItalic(this->theme.value("comments").italic);
	if(this->theme.value("comments").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("/\\*");
	hr.endPattern=QRegularExpression("\\*/");
	rules->append(hr);
}

void javalang::setTheme(QMap<QString,partsStruct> newtheme)
{
	this->theme=newtheme;
}

void javalang::runCustomRule(QString text,highLightingRule *hr)
{
	if(hr->type.compare("comment")==0)
		{
			bool inquote=false;
			int	cnt=0;
			while(cnt<text.length())
				{
					if(text.at(cnt)=='\\')
						{
							cnt+=2;
							continue;
						}
					if((text.at(cnt)=='"') && (inquote==false))
						{
							inquote=true;
							cnt++;
							continue;
						}
					if((text.at(cnt)=='"') && (inquote==true))
						{
							inquote=false;
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


