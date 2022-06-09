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
		const QString			subMenus[21]={	QString("Accents+Punctuation<<--´`¨ˆ¸!¡‼¿…"),\
											QString("Arrows<<--←→↑↓↔↕↨↗↘↳↵⇐⇒⇔⇤⇥⇦⇧⇨⇩"),\
											QString("Emoticons 1<<--☻☺☹😀😁😂😃😄😅😆😇😈😉😊😋😌😍😎😏😐😑😒😓😔😕😖😗😘😙😚"),\
											QString("Emoticons 2<<--😛😜😝😞😟😠😡😢😣😥😦😧😨😩😪😫😭😮😯😰😱😲😳😴😵😶😷"),\
											QString("Lines Single<<--—─¯¦|│∟⌐¬┌┬┐├┼┤└┴┘┤Γ"),\
											QString("Lines Double<<--╡╢╖╕╣║╗╝╜╛╞╟╚╔╩╦╠═╬╧╨╤╥╙╘╒╓╫╪"),\
											QString("Marks<<--©℗®℠™¤¢$£¥₧€℅№†‡§¶"),\
											QString("Math<<--÷×±≠<≤≥>√~≈≃‰‱∞ⁿº#0⅛¼⅜½⅝¾⅞¹²³"),\
											QString("Numbering<<--➊➋➌➍➎➏➐➑➒➓①②③④⑤⑥⑦⑧⑨⑩"),\
											QString("Quotes<<--'‘’“”‹›«»"),\
											QString("Science<<--℃℉°µøØƒΩßαΔδεΘπΣτΦφΨΩ℮∂∆∏∑∩≡⌠⌡⊕⊖⊗⊘⊙"),\
											QString("Shapes<<--►◄▲▼△▽░▒▓█▌▐■▄▀▬"),\
											QString("Symbols 1<<--✓✔☑✗✘✕✖☓☒☐•●○❍◯◎☉▢❏❑□◘◙◆◇✦★☆✮✯✰"),\
											QString("Symbols 2<<--☞☜☝☟☚☛✂✄☎✆✉✍✎✏✐✑✒⌨☮☯♡❤✽✾✿❀❁❃❇❈❅❄❆"),\
											QString("Symbols 3<<--☼☀☁☂☃☄♂♀♪♫♩♬♭♮♯♥♠♣♦·⚓�"),\
											QString("Accented Letters[A]<<--áÁàÀâÂåÅäÄãÃªæÆ"),\
											QString("Accented Letters[C+D+E]<<--çÇðÐéÉèÈêÊëË"),\
											QString("Accented Letters[G+I+N]<<--ğĞíÍìÌîÎïÏıİñÑ"),\
											QString("Accented Letters[O+S]<<--óÓòÒôÔöÖõÕœŒšŠşŞþÞ"),\
											QString("Accented Letters[U+Y+Z]<<--úÚùÙûÛüÜýÝÿŸžŽ"),\
											QString("")};
};

#endif
