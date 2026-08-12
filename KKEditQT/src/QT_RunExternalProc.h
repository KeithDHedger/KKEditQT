
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

		void							triggerCallbacks(QString txt);
};

#endif
