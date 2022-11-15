/*
 *
 * ©K. D. Hedger. Mon 14 Nov 20:24:50 GMT 2022 keithdhedger@gmail.com

 * This file (golang.cpp) is part of golang.

 * golang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * golang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with golang.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "golang.h"

void golang::initPlug(QString pathtoplug)
{
	this->plugPath=pathtoplug;
}

void golang::unloadPlug(void)
{
}

//new format
void golang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//variables
	hr.format.setForeground(this->theme.value("variables").colour);
	hr.format.setFontItalic(this->theme.value("variables").italic);
	if(this->theme.value("variables").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("([a-zA-Z0-9_])+[[:space:]]*(?=)");
	rules->append(hr);


//functions
	hr.format.setForeground(this->theme.value("functions").colour);
	hr.format.setFontItalic(this->theme.value("functions").italic);
	if(this->theme.value("functions").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("([[:word:]]+(\\.|\\-\\>|(?=[[:space:]]*\\()))+");
	rules->append(hr);
//classes
//	hr.format.setForeground(this->theme.value("class").colour);
//	hr.format.setFontItalic(this->theme.value("class").italic);
//	if(this->theme.value("class").bold==true)
//		hr.format.setFontWeight(QFont::Bold);
//	else
//		hr.format.setFontWeight(QFont::Normal);
//	hr.pattern=QRegularExpression("\\b[A-Za-z0-9_]+(?=::)\\b");
//	rules->append(hr);

//variables
//	hr.format.setForeground(this->theme.value("variables").colour);
//	hr.format.setFontItalic(this->theme.value("variables").italic);
//	if(this->theme.value("variables").bold==true)
//		hr.format.setFontWeight(QFont::Bold);
//	else
//		hr.format.setFontWeight(QFont::Normal);
//	hr.pattern=QRegularExpression("([a-zA-Z0-9_])+[[:space:]]*(?=)");
//	rules->append(hr);

//numbers
	hr.format.setForeground(this->theme.value("numbers").colour);
	hr.format.setFontItalic(this->theme.value("numbers").italic);
	if(this->theme.value("numbers").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("([+-]?\\b[[:digit:]]*\\.?[[:digit:]]+([eE][+-]?[[:digit:]]+)?\\b)|([+-]?\\b0x[[:xdigit:]]*\\.?[[:xdigit:]]+\\b)|([+-]?\\b0b[01]*\\.?[01]+\\b)");
	rules->append(hr);

//keywords
	hr.format.setForeground(this->theme.value("keywords").colour);
	hr.format.setFontItalic(this->theme.value("keywords").italic);
	if(this->theme.value("keywords").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b(break|case|chan|const|continue|default|defer|else|fallthrough|for|func|go|goto|if|interface|map|range|return|select|struct|switch|type|var)\\b");
	rules->append(hr);

//includes
	hr.format.setForeground(this->theme.value("includes").colour);
	hr.format.setFontItalic(this->theme.value("includes").italic);
	if(this->theme.value("includes").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("[[:space:]]*\\b(package|import)\\b");
	rules->append(hr);

//types
	hr.format.setForeground(this->theme.value("types").colour);
	hr.format.setFontItalic(this->theme.value("types").italic);
	if(this->theme.value("types").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b(int|string|float32|float64|int8|int16|int32|int64|uint|bool)\\b");
	rules->append(hr);

//custom
	hr.format.setForeground(this->theme.value("custom").colour);
	hr.format.setFontItalic(this->theme.value("custom").italic);
	if(this->theme.value("custom").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("true|false");
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

//multi line rules
void golang::setMultLineRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

///**/
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

void golang::setTheme(QMap<QString,partsStruct> newtheme)
{
	this->theme=newtheme;
}

void golang::runCustomRule(QString text,highLightingRule *hr)
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


