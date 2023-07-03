/*
 *
 * ©K. D. Hedger. Sun 13 Mar 18:41:54 GMT 2022 keithdhedger@gmail.com

 * This file (shlang.cpp) is part of shlang.

 * shlang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * shlang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with shlang.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "shlang.h"

void shlang::initPlug(QString pathtoplug)
{
	this->plugPath=pathtoplug;
}

void shlang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//keywords rule
	hr.format.setForeground(this->theme.value("keywords").colour);
	hr.format.setFontItalic(this->theme.value("keywords").italic);
	if(this->theme.value("keywords").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b(alias|bg|bind|break|builtin|caller|case|command|compgen|complete|continue|declare|dirs|disown|do|done|elif|else|enable|esac|eval|exec|exit|export|false|fc|fg|fi|for|getopts|hash|help|history|if|in|jobs|let|local|logout|popd|printf|pushd|read|readonly|return|select|set|shift|shopt|suspend|test|then|times|trap|true|type|typeset|umask|unalias|unset|until|wait|while|echo)\\b");
	rules->append(hr);

//numbers rule
	hr.format.setForeground(this->theme.value("numbers").colour);
	hr.format.setFontItalic(this->theme.value("numbers").italic);
	if(this->theme.value("numbers").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression(NUMBERSREGEX);
	rules->append(hr);

//includes rule
	hr.format.setForeground(this->theme.value("includes").colour);
	hr.format.setFontItalic(this->theme.value("includes").italic);
	if(this->theme.value("includes").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("^[[:blank:]]*(\\.[^\\n]*|source[^\\n]*)");
	rules->append(hr);

//variables
	hr.format.setForeground(this->theme.value("variables").colour);
	hr.format.setFontItalic(this->theme.value("variables").italic);
	if(this->theme.value("variables").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("([[:word:]]*)(?==)");
	rules->append(hr);

//curly brackets class
	hr.format.setForeground(this->theme.value("class").colour);
	hr.format.setFontItalic(this->theme.value("class").italic);
	if(this->theme.value("class").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("(\\${[[:print:]]*}|\\$\\([[:print:]]*\\)|\\$[[:word:]_]+)");
	rules->append(hr);

//functions dec
	hr.format.setForeground(this->theme.value("functions").colour);
	hr.format.setFontItalic(this->theme.value("functions").italic);
	if(this->theme.value("functions").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("^(function[[:blank:]]*[[:word:]]*|[[:word:]]*)([[:blank:]]*\\([[:blank:]]*\\))");
	rules->append(hr);

//quotes rule
	hr.format.setForeground(this->theme.value("quotes").colour);
	hr.format.setFontItalic(this->theme.value("quotes").italic);
	if(this->theme.value("quotes").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("(\"([^\"\\\\]*(\\\\.[^\"\\\\]*)*)\")|('([^'\\\\]*(\\\\.[^'\\\\]*)*)')");

	rules->append(hr);

//single line comments
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

void shlang::unloadPlug(void)
{
}

//multi line rules
void shlang::setMultLineRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

	hr.format.setForeground(this->theme.value("comments").colour);
	hr.format.setFontItalic(this->theme.value("comments").italic);
	if(this->theme.value("comments").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("if false;then");
	hr.endPattern=QRegularExpression("else|fi");
	rules->append(hr);
}

void shlang::setTheme(QMap<QString,partsStruct> newtheme)
{
	this->theme=newtheme;
}

void shlang::runCustomRule(QString text,highLightingRule *hr)
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


