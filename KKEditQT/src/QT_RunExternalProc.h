/*
 *
 * ©K. D. Hedger. Thu 13 Aug 11:48:19 BST 2026 keithdhedger@gmail.com

 * This file (QT_RunExternalProc.h) is part of KKEditQT.

 * KKEditQT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * KKEditQT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with KKEditQT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _QT_RUNEXTERNALPROC_
#define _QT_RUNEXTERNALPROC_

enum class stdErrOption{swallow,toFile,output,multiToFile};

class QT_RunExternalProc
{
	using QT_REP_Callback=std::function<void(QString)>;

	public:
		QT_RunExternalProc();
		~QT_RunExternalProc();

		bool							readByLine=false;
		bool							sync=true;
		qint64						lastBGPID;

		QString						runCommands(void);
		QString						runCommandsInShell(QString commands);
		bool							setCommands(QStringList sl);
		void							setStdErrFileOption(stdErrOption what,QString path="",QIODeviceBase::OpenModeFlag opt=QIODeviceBase::Append);
		void							setStdOutFileOption(QString path,QIODeviceBase::OpenModeFlag opt=QIODeviceBase::Truncate);
		void							connectCB(QT_REP_Callback cb);
		void							clearCallbacks(void);

	private:
		QVector<QStringList>			commandArgs;
		QVector<QProcess*>			procs;
		QString						stdErrFilePath="";
		QString						stdOutFilePath="";
		QIODeviceBase::OpenModeFlag	stdErrMode=QIODeviceBase::Append;
		QIODeviceBase::OpenModeFlag	stdOutMode=QIODeviceBase::Truncate;
		QVector<QT_REP_Callback>		callbacks;
		stdErrOption					stdErrwhat=stdErrOption::swallow;

		void							fireAndForget(QString commands);
		void							triggerCallbacks(QString txt);
};

#endif
