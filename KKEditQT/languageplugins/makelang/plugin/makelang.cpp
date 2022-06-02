/*
 *
 * ©K. D. Hedger. Sat  5 Mar 16:00:46 GMT 2022 keithdhedger@gmail.com

 * This file (kkeditqtPluginExample.cpp) is part of kkeditqtPluginExample.

 * kkeditqtPluginExample is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * kkeditqtPluginExample is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with kkeditqtPluginExample.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "makelang.h"

void makelang::initPlug(QString pathtoplug)
{
	this->plugPath=pathtoplug;
}

void makelang::unloadPlug(void)
{
}

//new format
void makelang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//functions
	hr.format.setForeground(this->theme.value("functions").colour);
	hr.format.setFontItalic(this->theme.value("functions").italic);
	if(this->theme.value("functions").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("^[[:space:]]*[A-Za-z0-9_]+(?=[[:space:]]*=?[[:space:]]*[{\\(])");
	rules->append(hr);

//targets
	hr.format.setForeground(this->theme.value("lanuageextra").colour);
	hr.format.setFontItalic(this->theme.value("lanuageextra").italic);
	if(this->theme.value("lanuageextra").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("^[[:print:]]*(?=:)");
	rules->append(hr);

//keywords
	hr.format.setForeground(this->theme.value("keywords").colour);
	hr.format.setFontItalic(this->theme.value("keywords").italic);
	if(this->theme.value("keywords").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b(addprefix|addsuffix|basename|call|dir|error|filter|filter-out|findstring|firstword|foreach|join|notdir|origin|patsubst|shell|sort|strip|subst|suffix|warning|wildcard|word|words|define|else|endef|endif|if|ifdef|ifeq|ifndef|ifneq|include|override|unexport|case|esac|fi|elif|echo|exit|for|printf|continue|do|done|test|break)\\b");
	rules->append(hr);

//variables
	hr.format.setForeground(this->theme.value("variables").colour);
	hr.format.setFontItalic(this->theme.value("variables").italic);
	if(this->theme.value("variables").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("([[:word:]_]*)(?=[[:space:]]*=)");
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

//types
	hr.format.setForeground(this->theme.value("types").colour);
	hr.format.setFontItalic(this->theme.value("types").italic);
	if(this->theme.value("types").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b(true|false|yes|no)|\\b");
	rules->append(hr);

//angle brackets
	hr.format.setForeground(this->theme.value("class").colour);
	hr.format.setFontItalic(this->theme.value("class").italic);
	if(this->theme.value("class").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("(\\$\\$?{[[:word:]][[:word:]]*}|\\$\\([[:word:]][[:word:]]*\\))");
	rules->append(hr);

//extras
	hr.format.setForeground(this->theme.value("class").colour);
	hr.format.setFontItalic(this->theme.value("class").italic);
	if(this->theme.value("class").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\$\\$?[[:word:]][[:word:]]*");
	rules->append(hr);

//quotes
	hr.format.setForeground(this->theme.value("quotes").colour);
	hr.format.setFontItalic(this->theme.value("quotes").italic);
	if(this->theme.value("quotes").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern = QRegularExpression("(\".*\")|('.*')");
	rules->append(hr);

//comments
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

void makelang::setTheme(QMap<QString,partsStruct> newtheme)
{
	this->theme=newtheme;
}

void makelang::runCustomRule(QString text,highLightingRule *hr)
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

					if((inquote==false) && (text.at(cnt)=='#'))
						{
							hr->start=cnt;
							hr->len=text.length()-cnt;
							return;
						}
					cnt++;
				}
		}
}


