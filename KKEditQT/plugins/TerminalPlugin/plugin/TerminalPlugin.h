/*
 *
 * ©K. D. Hedger. Sat 26 Mar 15:11:45 GMT 2022 keithdhedger@gmail.com

 * This file (TerminalPlugin.h) is part of TerminalPlugin.

 * TerminalPlugin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * TerminalPlugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with TerminalPlugin.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TerminalPluginPLUG_H
#define TerminalPluginPLUG_H

#include "../../../src/kkeditqtPluginInterface.h"
#include "../../../src/MainWindow.h"
#include "QT_prefsWidgets.h"

#include <QSettings>

class TerminalWidget : public QWidget
{
	public:
		QString				termName="";
		QString				xtermWinID="";
		QPointer<QProcess>	process=NULL;
		QWidget				*parentWidget=NULL;
		QString				backCol="white";
		QString				foreCol="black";
		int					windowid=0;
		qint64				xtermPID=0;
		QString				plugPath="";

		TerminalWidget(QString termname,QWidget *parent = nullptr) : QWidget(parent)
			{
				this->termName=termname;
				this->resize(parent->size());
				this->parentWidget=parent;
				this->setFocusPolicy(Qt::NoFocus);
			}

		void startXTerm(void)
			{
				process=new QProcess(this);
				QStringList arguments;

				arguments<<"-T"<<this->termName<<"-xrm"<<"*clickToFocus: true"<<"-into"<<QString::number(this->winId())<<"-xrm"<<QString("#include \"%1/xinst.tpxrc\"").arg(this->plugPath)<<"-bg"<<this->backCol<<"-fg"<<this->foreCol;

				process->start("xterm",arguments);
				process->waitForStarted();
				this->xtermPID=process->processId();
				this->windowid=this->winId();
				this->getXTermData();
				this->resize(this->parentWidget->size());
			}

		bool getXTermData(void)
			{
				QProcess		pr;
				QByteArray	result;

				pr.start("xdotool", QStringList() << "search"<<"-name"<<"^"+this->termName+"$");
				pr.waitForStarted();
				pr.waitForReadyRead();
				result=pr.readAll().trimmed();
				pr.waitForFinished(); 
				if(result.isEmpty()==true)
					return(false);
				if(result.contains('\n'))
					return(false);

				this->xtermWinID=result;
				//qDebug()<<"this->xtermWinID"<<this->xtermWinID;
				//qDebug()<<"this->windowid"<<this->windowid;
				//qDebug()<<"this->xtermPID"<<this->xtermPID;
				return(true);
			}

	protected:
		void resizeEvent(QResizeEvent *event) override
			{
				QWidget::resizeEvent(event);
				if(this->process==NULL)
					return;
				if(this->xtermWinID.isEmpty()==true)
					{
						if(this->getXTermData()==false)
							return;
					}
				if(process->state() == QProcess::Running)
					{
						QString s;
						int h=event->size().height();
						int w=event->size().width();
						s=QString("xdotool >/dev/null 2>/dev/null windowsize %1 %2 %3").arg(this->xtermWinID).arg(w).arg(h);
						system(s.toStdString().c_str());
					}
			}
	private:
};

struct termDataStruct
{
	TerminalWidget *console;
	QDockWidget	*dockWidget;
	QAction		*toggleTerm;
};

class TerminalPluginPlug : public QObject, kkEditQTPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.KKEditQT.kkEditQTPluginInterface" FILE "TerminalPlugin.json")
	Q_INTERFACES(kkEditQTPluginInterface)

	public:
		void						initPlug(KKEditClass *kk,QString pathtoplug) override;
		void						unloadPlug(void) override;
		void						plugAbout(void) override;
		void						plugSettings(void) override;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
		void						plugRun(plugData *data) override;
#pragma GCC diagnostic pop
		unsigned int				plugWants(void) override;
		void						showDocs(void);

	private:
		KKEditClass				*mainKKEditClass;
		QString					plugPath;
		QMenu					*TerminalPluginMenu;
		QMenu					*terminalsMenu;
		QAction					*toggleViewAct;
		QAction					*newAct;
		QAction					*toggleTabsAct;
		QVector<termDataStruct>	terminals;
		QDockWidget				*dw;
		QString					baseName="";
		plugData					*data=NULL;

		int						currentTerminal=0;
		int						namenum=0;
		bool						openOnStart=false;
		int						currentHeight=-1;
		int						openAtWidth=600;

		QString					filePath;
		QString					folderPath;
		void						doMenuItem(int what,int whome);
		void						addTerminal(void);
};

#endif
