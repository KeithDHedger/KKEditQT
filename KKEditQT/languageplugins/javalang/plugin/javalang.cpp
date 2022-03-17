/*
 *
 * ©K. D. Hedger. Wed 16 Mar 16:31:43 GMT 2022 keithdhedger@gmail.com

 * This file (javalang.cpp) is part of javalang.

 * javalang is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

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
	themeStruct	blank={Qt::black,QFont::Normal,false};

	this->plugPath=pathtoplug;
	if(this->theme.count()==0)
		for(int j=FUNCTIONTHEME;j<THEMEDONE;j++)
			this->theme[j]=blank;
}

void javalang::unloadPlug(void)
{
}

//new format
void javalang::setLanguageRules(QVector<highLightingRule> *rules)
{
	highLightingRule	hr;

//functions
	hr.format.setFontItalic(this->theme[FUNCTIONTHEME].italic);
	hr.format.setFontWeight(this->theme[FUNCTIONTHEME].weight);
	hr.format.setForeground(this->theme[FUNCTIONTHEME].colour);

	hr.pattern=QRegularExpression("\\b[[:word:]\\.]*[[:word:]_]+(?=\\()");
	rules->append(hr);

//classes
	hr.format.setForeground(this->theme[CLASSTHEME].colour);
	hr.format.setFontWeight(this->theme[CLASSTHEME].weight);
	hr.format.setFontItalic(this->theme[CLASSTHEME].italic);
	hr.pattern=QRegularExpression("(class|enum|extends|implements|instanceof|interface|native|throws|private|protected|public)");
	rules->append(hr);

//quotes
	hr.format.setForeground(this->theme[QUOTESTHEME].colour);
	hr.format.setFontWeight(this->theme[QUOTESTHEME].weight);
	hr.format.setFontItalic(this->theme[QUOTESTHEME].italic);
	hr.pattern = QRegularExpression("(\"([^\"\\\\]*(\\\\.[^\"\\\\]*)*)\")|('\\\\.')|('.')");
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
	hr.pattern=QRegularExpression("\\b(assert|break|case|catch|continue|default|do|else|finally|for|if|return|throw|switch|try|while|new|super|this|goto|abstract|final|static|strictfp|synchronized|transient|volatile|class|enum|extends|implements|instanceof|interface|native|throws)\\b");
	rules->append(hr);

//includes
	hr.format.setForeground(this->theme[INCLUDETHEME].colour);
	hr.format.setFontWeight(this->theme[INCLUDETHEME].weight);
	hr.format.setFontItalic(this->theme[INCLUDETHEME].italic);
	hr.pattern=QRegularExpression("\\b(import|package)\\b");
	rules->append(hr);

//types
	hr.format.setForeground(this->theme[TYPETHEME].colour);
	hr.format.setFontWeight(this->theme[TYPETHEME].weight);
	hr.format.setFontItalic(this->theme[TYPETHEME].italic);
	hr.pattern=QRegularExpression("\\b(boolean|byte|char|double|float|int|long|short|void)\\b");
	rules->append(hr);

//java extras
	hr.format.setForeground(this->theme[CUSTOMTHEME].colour);
	hr.format.setFontWeight(this->theme[CUSTOMTHEME].weight);
	hr.format.setFontItalic(this->theme[CUSTOMTHEME].italic);
	hr.pattern=QRegularExpression("\\b(abstract|final|static|strictfp|synchronized|transient|volatile)\\b");
	rules->append(hr);

//single line comment
	hr.format.setForeground(this->theme[COMMENTTHEME].colour);
	hr.format.setFontWeight(this->theme[COMMENTTHEME].weight);
	hr.format.setFontItalic(this->theme[COMMENTTHEME].italic);
	hr.pattern=QRegularExpression("//[^\n]*");
	rules->append(hr);
}

//odd single formats set to "" for no multiline comment
void javalang::setMultLineFormatStart(highLightingRule *hr)
{
	hr->format.setForeground(this->theme[COMMENTTHEME].colour);
	hr->format.setFontWeight(this->theme[COMMENTTHEME].weight);
	hr->format.setFontItalic(this->theme[COMMENTTHEME].italic);
	hr->pattern = QRegularExpression("/\\*");
}

void javalang::setMultLineFormatStop(highLightingRule *hr)
{
	hr->pattern = QRegularExpression("\\*/");
}

void javalang::setTheme(QHash<int,themeStruct> newtheme)
{
	this->theme=newtheme;
}

