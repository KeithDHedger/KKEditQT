/*
 *
 * ©K. D. Hedger. Wed 16 Mar 14:23:10 GMT 2022 keithdhedger@gmail.com

 * This file (phplang.cpp) is part of phplang.

 * phplang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * phplang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with phplang.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "phplang.h"

void phplang::initPlug(QString pathtoplug)
{
	this->plugPath=pathtoplug;
}

void phplang::unloadPlug(void)
{
}

//new format
void phplang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//functions
	hr.format.setForeground(this->theme.value("functions").colour);
	hr.format.setFontItalic(this->theme.value("functions").italic);
	if(this->theme.value("functions").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
	rules->append(hr);

//angle brackets
	hr.format.setForeground(this->theme.value("keywords").colour);
	hr.format.setFontItalic(this->theme.value("keywords").italic);
	if(this->theme.value("keywords").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("(<[/\\!?]?.*[/\\!?]?>)|(<[/\\!?]?[^\\n]*)|([/\\!?]?>)");
	rules->append(hr);

//quotes
	hr.format.setForeground(this->theme.value("quotes").colour);
	hr.format.setFontItalic(this->theme.value("quotes").italic);
	if(this->theme.value("quotes").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern = QRegularExpression("(\"([^\"\\\\]*(\\\\.[^\"\\\\]*)*)\")|('([^'\\\\]*(\\\\.[^'\\\\]*)*)')");
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
	hr.pattern=QRegularExpression("\\b(__FILE__|exception|php_user_filter|__LINE__|abstract|array|as|break|case|catch|cfunction|class|clone|const|continue|declare|default|do|while|for|each|echo|else|elseif|empty|enddeclare|endfor|endforeach|endif|endswitch|endwhile|eval|exit|extends|final|foreach|function|global|goto|if|implements|interface|instanceof|isset|list|namespace|new|old_function|print|private|protected|public|return|static|switch|throw|unset|use|var|__FUNCTION__|__CLASS__|__METHOD__|__DIR__|__NAMESPACE__)\\b");
	rules->append(hr);

//types
	hr.format.setForeground(this->theme.value("types").colour);
	hr.format.setFontItalic(this->theme.value("types").italic);
	if(this->theme.value("types").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b(and|or|xor|null|true|false|NULL|TRUE|FALSE)\\b");
	rules->append(hr);

//variables
	hr.format.setForeground(this->theme.value("variables").colour);
	hr.format.setFontItalic(this->theme.value("variables").italic);
	if(this->theme.value("variables").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\$[[:word:]]*");
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

void phplang::setMultLineRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;
	hr.format.setForeground(this->theme.value("comments").colour);
	hr.format.setFontItalic(this->theme.value("comments").italic);
	if(this->theme.value("comments").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("/\\*");
	hr.endPattern=QRegularExpression("\\*/");
	rules->append(hr);
}

void phplang::setTheme(QMap<QString,partsStruct> newtheme)
{
	this->theme=newtheme;
}

void phplang::runCustomRule(QString text,highLightingRule *hr)
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


