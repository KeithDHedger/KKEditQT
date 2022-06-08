/*
 *
 * ©K. D. Hedger. Sat 26 Mar 15:11:45 GMT 2022 keithdhedger@gmail.com

 * This file (SymbolsMenu.h) is part of SymbolsMenu.

 * SymbolsMenu is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * SymbolsMenu is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with SymbolsMenu.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SYMOLSPLUG_H
#define SYMOLSPLUG_H

#include "../../../src/kkeditqtPluginInterface.h"

#define MAXCLIPMENULEN 48
#define MAXCLIPS 32

class SymbolsMenuPlug : public QObject, kkEditQTPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.kkEditQTPluginInterface" FILE "SymbolsMenu.json")
	Q_INTERFACES(kkEditQTPluginInterface)

	public:
		void						initPlug(KKEditClass *kk,QString pathtoplug) override;
		void						unloadPlug(void) override;
		void						plugAbout(void) override;
		void						plugSettings(void) override;
		void						plugRun(plugData *data) override {};
		unsigned int				plugWants(void) override;

	private:
		KKEditClass				*mainKKEditClass;
		QString					plugPath;
		QMenu					*symbolMenu;
		QClipboard				*clipboard;
		QMetaObject::Connection	symbolCon;
		const char				*AandP[20]={	"Accents+Punctuation<<--´`¨ˆ¸!¡‼¿…",\
											"Arrows<<--←→↑↓↔↕↨↗↘↳↵⇐⇒⇔⇤⇥⇦⇧⇨⇩",\
											"Lines Single<<--—─¯¦|│∟⌐¬┌┬┐├┼┤└┴┘┤Γ",\
											"Lines Double<<--╡╢╖╕╣║╗╝╜╛╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪",\
											"Marks<<--©℗®℠™¤¢$£¥₧€℅№†‡§¶",\
											"Math<<--÷×±≠<≤≥>√~≈≃‰‱∞ⁿº#0⅛¼⅜½⅝¾⅞¹²³",\
											"Numbering<<--➊➋➌➍➎➏➐➑➒➓①②③④⑤⑥⑦⑧⑨⑩",\
											"Quotes<<--'‘’“”‹›«»",\
											"Science<<--℃℉°µøØƒΩßαΔδεΘπΣτΦφΨΩ℮∂∆∏∑∩≡⌠⌡⊕⊖⊗⊘⊙",\
											"Shapes<<--►◄▲▼△▽░▒▓█▌▐■▄▀▬",\
											"Symbols 1<<--✓✔☑✗✘✕✖☓☒☐•●○❍◯◎☉▢❏❑□◘◙◆◇✦★☆✮✯✰",\
											"Symbols 2<<--☞☜☝☟☚☛✂✄☎✆✉✍✎✏✐✑✒⌨☮☯♡❤✽✾✿❀❁❃❇❈❅❄❆",\
											"Symbols 3<<--☼☀☁☂☃☄♂♀♪♫♩♬♭♮♯♥♠♣♦·⚓�",\
											"Accented Letters<<--áÁàÀâÂåÅäÄãÃªæÆçÇðÐéÉèÈêÊëËğĞíÍìÌîÎïÏıİñÑóÓòÒôÔöÖõÕœŒšŠşŞþÞúÚùÙûÛüÜýÝÿŸžŽ",\
											NULL};
};

#endif
