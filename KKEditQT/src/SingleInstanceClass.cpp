/*
 *
 * ©K. D. Hedger. Thu 23 Dec 20:37:56 GMT 2021 keithdhedger@gmail.com

 * This file (SingleInstanceClass.cpp) is part of KKEditQT.

 * KKEditQT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * at your option) any later version.

 * KKEditQT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with KKEditQT.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "SingleInstanceClass.h"

SingleInstanceClass::SingleInstanceClass(QApplication *app,int key,bool forcem)
{
	QSettings	prefs;
	bool		single=prefs.value("app/usesingle",QVariant(bool(true))).value<bool>();

	if(QX11Info::isPlatformX11()==false)
		this->isOnX11=false;

	if((QX11Info::isPlatformX11()==false) || (single==false) || (forcem==true))
		{
			srand(time(NULL));
			this->workspace=(int)random();
			this->usingMulti=true;
		}
	else
		this->workspace=this->getSIWorkSpace();

	if(key!=-1)
		this->useKey=key;
}

SingleInstanceClass::~SingleInstanceClass()
{
	if(this->deleteComfiles==true)
		{
			fileMsg.remove();
			filePID.remove();
		}
}

bool SingleInstanceClass::getRunning(void)
{
	QString	commsFolderName;
	QString	commsDeskfile;
	QString	commsDeskfilePID;
	bool	retval=false;
	QDir	commsDir;
	bool	isrunning=false;

	if(this->usingMulti==true)
		return(false);

	commsFolderName=commsDir.tempPath() + "/KKEditQTComms";
	commsDir.mkpath(commsFolderName);

	commsDeskfile=QString("%1/desktop%2").arg(commsFolderName).arg(this->workspace);
	commsDeskfilePID=QString("%1/pid%2").arg(commsFolderName).arg(this->workspace);

	this->fileMsg.setFileName(commsDeskfile);
	this->filePID.setFileName(commsDeskfilePID);

	QFileInfo	fileinfo(this->fileMsg);

	retval=this->filePID.open(QIODevice::Text | QIODevice::ReadOnly);
	if(retval==true)
		{
			QString		pidcontent=this->filePID.readAll();
			if(kill(pidcontent.toInt(nullptr,10),0)!=0)
				{
					this->fileMsg.remove();
					this->filePID.remove();
				}
			this->filePID.close();
		}

	if(fileinfo.exists()==true)
		{
			retval=this->fileMsg.open(QIODevice::Text | QIODevice::ReadOnly);
			if(retval==true)
				{
					msgStruct	message;
					int			msglen;
					QString		content=this->fileMsg.readAll();
					if(this->useKey==-1)
						{
							this->useKey=content.toInt(nullptr,10);
							this->queueID=msgget(content.toInt(nullptr,10),IPC_CREAT|0660);
						}
					else
						this->queueID=msgget(this->useKey,IPC_CREAT|0660);
					this->fileMsg.close();
					isrunning=true;
				}
		}
	else
		{
			retval=this->fileMsg.open(QIODevice::Text | QIODevice::WriteOnly);
			if(retval==true)
				{
					QTextStream	out(&this->fileMsg);
					if(this->useKey==-1)
						{
							out << MSGKEY+this->workspace << "\n";
							this->queueID=msgget(MSGKEY+this->workspace,IPC_CREAT|0660);
							this->useKey=MSGKEY+this->workspace;
						}
					else
						{
							out << this->useKey << "\n";
							this->queueID=msgget(this->useKey,IPC_CREAT|0660);
						}
					this->fileMsg.close();
				}
			retval=this->filePID.open(QIODevice::Text | QIODevice::WriteOnly);
			if(retval==true)
				{
					QTextStream	out(&this->filePID);
					out << getpid() << "\n";
					this->filePID.close();
				}
			this->deleteComfiles=true;
		}

	return(isrunning);
}

void* SingleInstanceClass::getX11Prop(Window w,Atom prop,Atom type,int fmt,unsigned long *rcountp)
{
	void			*ptr=NULL;
	unsigned long	count=32;
	Atom			rtype;
	int				rfmt;
	unsigned long	rafter;

	for (;;)
		{
			if (XGetWindowProperty(QX11Info::display(),w,prop,0L,count,False,type,&rtype,&rfmt,rcountp,&rafter,(unsigned char **)&ptr) != Success)
				return NULL;
			else if (rtype != type || rfmt != fmt)
				return NULL;
			else if (rafter>0)
				{
					XFree(ptr);
					ptr=NULL;
					count *= 2;
				}
			else
				return ptr;
		}
}

long SingleInstanceClass::getSIWorkSpace(void)
{
	Display			*disp=QX11Info::display();
	unsigned long	rootwin=QX11Info::appRootWindow(-1);
	unsigned long	n=0;
	Atom			NET_WM_DESKTOP;
	long			*deskp;

	NET_WM_DESKTOP=XInternAtom(disp, "_NET_CURRENT_DESKTOP",False);
	deskp=(long*)getX11Prop(rootwin,NET_WM_DESKTOP,XA_CARDINAL,32,&n);
	if (n !=0)
		return(*deskp);

	return(-1);
}

