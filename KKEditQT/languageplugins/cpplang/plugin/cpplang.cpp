/*
 *
 * ©K. D. Hedger. Wed 16 Mar 14:08:07 GMT 2022 keithdhedger@gmail.com

 * This file (cpplang.cpp) is part of cpplang.

 * cpplang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * cpplang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with cpplang.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cpplang.h"

void cpplang::initPlug(QString pathtoplug)
{
	this->plugPath=pathtoplug;
}

void cpplang::unloadPlug(void)
{
}

//new format
void cpplang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

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
	hr.format.setForeground(this->theme.value("class").colour);
	hr.format.setFontItalic(this->theme.value("class").italic);
	if(this->theme.value("class").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b[A-Za-z0-9_]+(?=::)\\b");
	rules->append(hr);

//variables
	hr.format.setForeground(this->theme.value("variables").colour);
	hr.format.setFontItalic(this->theme.value("variables").italic);
	if(this->theme.value("variables").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("([a-zA-Z0-9_\\.]|-\\>)+[[:space:]]*(?==)");
	rules->append(hr);

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
	hr.pattern=QRegularExpression("\\b(__asm|__cdecl|__declspec|__export|__far16|__fastcall|__fortran|__import|__pascal|__rtti|__stdcall|_asm|_cdecl|__except|_export|_far16|_fastcall|__finally|_fortran|_import|_pascal|_stdcall|__thread|__try|asm|auto|break|case|catch|cdecl|continue|default|do|else|enum|extern|goto|pascal|register|return|sizeof|static|struct|switch|typedef|union|volatile|class|const_cast|delete|dynamic_cast|explicit|friend|inline|mutable|namespace|new|operator|private|protected|public|reinterpret_cast|static_cast|template|this|throw|try|typeid|typename|using|virtual|for|if|while)\\b");
	rules->append(hr);

//includes
	hr.format.setForeground(this->theme.value("includes").colour);
	hr.format.setFontItalic(this->theme.value("includes").italic);
	if(this->theme.value("includes").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("#[[:space:]]*(define|include|undef|ifdef|ifndef|if|elif|else|endif|pragma|error)\\b.*$");
	rules->append(hr);

//types
	hr.format.setForeground(this->theme.value("types").colour);
	hr.format.setFontItalic(this->theme.value("types").italic);
	if(this->theme.value("types").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("\\b(bool|char|double|float|int|long|short|signed|unsigned|void|wchar_t|const)\\b");
	rules->append(hr);

//custom
	hr.format.setForeground(this->theme.value("custom").colour);
	hr.format.setFontItalic(this->theme.value("custom").italic);
	if(this->theme.value("custom").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("NULL|nullptr|true|false|TRUE|FALSE");
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
void cpplang::setMultLineRules(QVector<highLightingRule> *rules)
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
//if 0/else/end
	hr.format.setForeground(this->theme.value("comments").colour);
	hr.format.setFontItalic(this->theme.value("comments").italic);
	if(this->theme.value("comments").bold==true)
		hr.format.setFontWeight(QFont::Bold);
	else
		hr.format.setFontWeight(QFont::Normal);
	hr.pattern=QRegularExpression("^#if 0");
	hr.endPattern=QRegularExpression("^(#endif|#else)");
	rules->append(hr);
}

void cpplang::setTheme(QMap<QString,partsStruct> newtheme)
{
	this->theme=newtheme;
}

void cpplang::runCustomRule(QString text,highLightingRule *hr)
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


