/*
 *
 * ©K. D. Hedger. Mon 14 Mar 13:59:56 GMT 2022 keithdhedger@gmail.com

 * This file (pythonlang.cpp) is part of pythonlang.

 * pythonlang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * pythonlang is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with pythonlang.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pythonlang.h"

void pythonlang::initPlug(QString pathtoplug)
{
	themeStruct	blank={Qt::black,QFont::Normal,false};

	this->plugPath=pathtoplug;
	if(this->theme.count()==0)
		for(int j=FUNCTIONTHEME;j<THEMEDONE;j++)
			this->theme[j]=blank;
}

void pythonlang::unloadPlug(void)
{
}

//new format
void pythonlang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//functions
	hr.format.setFontItalic(this->theme[FUNCTIONTHEME].italic);
	hr.format.setFontWeight(this->theme[FUNCTIONTHEME].weight);
	hr.format.setForeground(this->theme[FUNCTIONTHEME].colour);
	hr.pattern=QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
	rules->append(hr);

//types
	hr.format.setForeground(this->theme[TYPETHEME].colour);
	hr.format.setFontWeight(this->theme[TYPETHEME].weight);
	hr.format.setFontItalic(this->theme[TYPETHEME].italic);
	hr.pattern=QRegularExpression("\\b(bool|char|double|float|int|long|short|signed|unsigned|void|wchar_t|const)\\b");
	rules->append(hr);

//keywords
	hr.format.setForeground(this->theme[KEYWORDTHEME].colour);
	hr.format.setFontWeight(this->theme[KEYWORDTHEME].weight);
	hr.format.setFontItalic(this->theme[KEYWORDTHEME].italic);
	hr.pattern=QRegularExpression("\\b(import|abs|all|any|apply|basestring|buffer|callable|chr|classmethod|cmp|coerce|compile|complex|delattr|dict|dir|divmod|enumerate|eval|execfile|file|filter|frozenset|getattr|globals|hasattr|hash|hex|id|input|intern|isinstance|issubclass|iter|len|list|locals|map|max|min|object|oct|open|ord|pow|property|range|raw_input|reduce|reload|repr|reversed|round|setattr|set|slice|sorted|staticmethod|str|sum|super|tuple|type|unichr|unicode|vars|xrange|zip|from|try|except|def|and|assert|break|class|continue|def|del|elif|else|except|exec|finally|for|global|if|in|is|lambda|not|or|pass|print|raise|return|try|while|with|yield|__name__|__debug__|__class__)\\b");
	rules->append(hr);

//quotes
	hr.format.setForeground(this->theme[QUOTESTHEME].colour);
	hr.format.setFontWeight(this->theme[QUOTESTHEME].weight);
	hr.format.setFontItalic(this->theme[QUOTESTHEME].italic);
	hr.pattern = QRegularExpression("(\"([^\"\\\\]*(\\\\.[^\"\\\\]*)*)\")|('([^'\\\\]*(\\\\.[^'\\\\]*)*)')");
	rules->append(hr);

//variables
	hr.format.setForeground(this->theme[CUSTOMTHEME].colour);
	hr.format.setFontWeight(this->theme[CUSTOMTHEME].weight);
	hr.format.setFontItalic(this->theme[CUSTOMTHEME].italic);
	hr.pattern=QRegularExpression("([[:word:]_]*)(?=[[:space:]]*=)");
	rules->append(hr);

//numbers
	hr.format.setForeground(this->theme[NUMBERTHEME].colour);
	hr.format.setFontWeight(this->theme[NUMBERTHEME].weight);
	hr.format.setFontItalic(this->theme[NUMBERTHEME].italic);
	hr.pattern=QRegularExpression(NUMBERSREGEX);
	rules->append(hr);

//custom
	hr.format.setForeground(this->theme[CUSTOMTHEME].colour);
	hr.format.setFontWeight(this->theme[CUSTOMTHEME].weight);
	hr.format.setFontItalic(this->theme[CUSTOMTHEME].italic);
	hr.pattern=QRegularExpression("NULL|nullptr|true|false|TRUE|FALSE|None|Ellipsis|NotImplemented|self|True|False");
	rules->append(hr);

//single line comment
	hr.format.setForeground(this->theme[COMMENTTHEME].colour);
	hr.format.setFontWeight(this->theme[COMMENTTHEME].weight);
	hr.format.setFontItalic(this->theme[COMMENTTHEME].italic);
	hr.pattern=QRegularExpression("#[^\n]*");
	rules->append(hr);

//python odds
	hr.format.setForeground(this->theme[LANGUAGEEXTRAS].colour);
	hr.format.setFontWeight(this->theme[LANGUAGEEXTRAS].weight);
	hr.format.setFontItalic(this->theme[LANGUAGEEXTRAS].italic);
	hr.pattern=QRegularExpression("\\b(ArithmeticError|AssertionError|AttributeError|EnvironmentError|EOFError|Exception|FloatingPointError|ImportError|IndentationError|IndexError|IOError|KeyboardInterrupt|KeyError|LookupError|MemoryError|NameError|NotImplementedError|OSError|OverflowError|ReferenceError|RuntimeError|StandardError|StopIteration|SyntaxError|SystemError|SystemExit|TabError|TypeError|UnboundLocalError|UnicodeDecodeError|UnicodeEncodeError|UnicodeError|UnicodeTranslateError|ValueError|WindowsError|ZeroDivisionError|Warning|UserWarning|DeprecationWarning|PendingDeprecationWarning|SyntaxWarning|OverflowWarning|RuntimeWarning|FutureWarning)\\b");
	rules->append(hr);
}

//odd single formats set to "" for no multiline comment
void pythonlang::setMultLineFormatStart(highLightingRule *hr)
{
	hr->format.setForeground(this->theme[COMMENTTHEME].colour);
	hr->format.setFontWeight(this->theme[COMMENTTHEME].weight);
	hr->format.setFontItalic(this->theme[COMMENTTHEME].italic);
	hr->pattern = QRegularExpression("\"\"\"");
}

void pythonlang::setMultLineFormatStop(highLightingRule *hr)
{
		hr->pattern = QRegularExpression("\"\"\"");
}

void pythonlang::setTheme(QHash<int,themeStruct> newtheme)
{
	this->theme=newtheme;
}

