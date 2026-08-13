/*
 *
 * ©K. D. Hedger. Thu 13 Aug 11:48:10 BST 2026 keithdhedger@gmail.com

 * This file (QT_RunExternalProc.cpp) is part of KKEditQT.

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

#include <wordexp.h>
#include "QT_RunExternalProc.h"

QT_RunExternalProc::~QT_RunExternalProc()
{
}

QT_RunExternalProc::QT_RunExternalProc()
{
}

void QT_RunExternalProc::setStdErrFileOption(stdErrOption what,QString path,QIODeviceBase::OpenModeFlag opt)
{
	this->stdErrwhat=what;
	this->stdErrMode=opt;
	this->stdErrFilePath=path;
}

void QT_RunExternalProc::setStdOutFileOption(QString path,QIODeviceBase::OpenModeFlag opt)
{
	this->stdOutMode=opt;
	this->stdOutFilePath=path;
}

void QT_RunExternalProc::fireAndForget(QString commands)
{
	QProcess::startDetached("sh",QStringList()<<"-c"<<commands,"",&this->lastBGPID);
}

QString QT_RunExternalProc::runCommandsInShell(QString commands)
{
	QString retstr="";
	FILE		*fp=NULL;
	char		line[1024];

	if(this->sync==false)
		{
			this->fireAndForget(commands);
		}
	else
		{
			fp=popen(qPrintable(commands),"r");
			if(fp!=NULL)
				{
					while(fgets(line,1024,fp))
						{
							retstr+=line;
							if(this->callbacks.count()>0)
								this->triggerCallbacks(line);
						}
					pclose(fp);
				}
		}
	return(retstr);
}

QString QT_RunExternalProc::runCommands(void)
{
	QString	retstr="";
	int		last;

	for(int j=0;j<this->commandArgs.count();j++)
		{
			QProcess *p=new QProcess;
			QString app=this->commandArgs.at(j).at(0);
			QStringList sl=this->commandArgs.at(j);
			sl.removeAt(0);
			p->setProgram(app);
			p->setArguments(sl);
			p->setProcessChannelMode(QProcess::SeparateChannels);
			if(this->stdErrwhat==stdErrOption::toFile)
				p->setStandardErrorFile(this->stdErrFilePath,this->stdErrMode);
			if(this->stdErrwhat==stdErrOption::multiToFile)
				p->setStandardErrorFile(QString("%1.proc-%2").arg(this->stdErrFilePath).arg(j),this->stdErrMode);

			this->procs.push_back(p);
		}

	last=this->procs.count()-1;
	if(this->stdOutFilePath.isEmpty()==false)
		this->procs.at(last)->setStandardOutputFile(this->stdOutFilePath,this->stdOutMode);

	for(int j=0;j<this->procs.count()-1;j++)
		{
			this->procs.at(j)->setStandardOutputProcess(this->procs.at(j+1));
			QObject::connect(this->procs.at(j), &QProcess::readyReadStandardError, [this,j]()
				{
					if(this->stdErrwhat==stdErrOption::output)
						qDebug().noquote() << this->procs.at(j)->readAllStandardError();
				});
		}

	for(int j=this->procs.count()-1;j>-1;j--)
		{
			this->procs.at(j)->start();
			this->procs.at(j)->waitForStarted();
		}

	QObject::connect(this->procs.at(last), &QProcess::readyReadStandardError, [this,last]()
		{
			if(this->stdErrwhat==stdErrOption::output)
				qDebug().noquote() << this->procs.at(last)->readAllStandardError();
		});

	QObject::connect(this->procs.at(last), &QProcess::readyReadStandardOutput,[this,last,&retstr]()
		{
			if(this->readByLine==true)
				{
					while(this->procs.at(last)->canReadLine())
						{
							QString line=procs.at(last)->readLine();
							retstr+=line;
							line.chop(1);
							if(this->callbacks.count()>0)
								this->triggerCallbacks(line);
						}
				}
			else
				{
					QByteArray out=this->procs.at(last)->readAllStandardOutput();
					if(this->callbacks.count()>0)
						this->triggerCallbacks(out);
					retstr+=out;
				}
		}); 

	for(int j=this->procs.count()-1;j>-1;j--)
		this->procs.at(j)->waitForFinished();

	for(int j=0;j<this->procs.count();j++)
		delete this->procs.at(j);

	this->procs.clear();
	return(retstr);
}

bool QT_RunExternalProc::setCommands(QStringList sl)
{
	QStringList	commands=sl;
	this->commandArgs.clear();

	for(int j=0;j<sl.count();j++)
		{
			wordexp_t	array;
			QStringList	strarg;
			int			retval=-1;

			retval=wordexp(qPrintable(sl.at(j)),&array,WRDE_SHOWERR);
			if(retval!=0)
				{
					qDebug()<<"Error in string:"<<sl.at(j);
					return(false);
				}

			strarg.clear();
			for(size_t i=0; i<array.we_wordc; i++)
				{
					//qDebug()<<array.we_wordv[i];
					strarg<<array.we_wordv[i];
				}

			wordfree(&array); 
			this->commandArgs.push_back(strarg);
		}
	return(true);
}

void QT_RunExternalProc::connectCB(QT_REP_Callback cb)
{
	this->callbacks.push_back(cb);
}

void QT_RunExternalProc::triggerCallbacks(QString txt)
{
	for(int j=0;j<this->callbacks.size();j++)
		this->callbacks.at(j)(txt);
}

void QT_RunExternalProc::clearCallbacks(void)
{
	this->callbacks.clear();
}
