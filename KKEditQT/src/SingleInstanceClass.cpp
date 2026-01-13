/*
 *
 * ©K. D. Hedger. Fri 25 Oct 13:32:51 BST 2024 keithdhedger@gmail.com

 * This file (SingleInstanceClass.cpp) is part of KKEditQT.

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

#include "SingleInstanceClass.h"

void* SingleInstanceClass::getX11Prop(Display	*display,Window w,Atom prop,Atom type,int fmt,unsigned long *rcountp)
{
	void				*ptr=NULL;
	unsigned long	count=32;
	Atom				rtype;
	int				rfmt;
	unsigned long	rafter;

	for (;;)
		{
			if (XGetWindowProperty(display,w,prop,0L,count,False,type,&rtype,&rfmt,rcountp,&rafter,(unsigned char **)&ptr) != Success)
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
	return(NULL);
}

long SingleInstanceClass::getSIWorkSpace(Display *display)
{
	if(display!=NULL)
		{
			unsigned long	rootwin=DefaultRootWindow(display);
			unsigned long	n=0;
			Atom				NET_WM_DESKTOP;
			long				*deskp;

			NET_WM_DESKTOP=XInternAtom(display,"_NET_CURRENT_DESKTOP",False);
			deskp=(long*)getX11Prop(display,rootwin,NET_WM_DESKTOP,XA_CARDINAL,32,&n);
			if (n !=0)
				{
					long retval=*deskp;
					XFree(deskp);
					return(retval);
				}
		}
	return(-1);
}

unsigned long SingleInstanceClass::hashFromKey(QString key)
{
	unsigned long hash=0;

	for(int i=0;i<key.length();i++)
		hash=31*hash+key.at(i).toLatin1();

	return(hash);
}

SingleInstanceClass::SingleInstanceClass(QString name,int suppliedkey)
{
	Display	*display=NULL;
	int		workspace=-1;
	int		screen;
	QString	displaystr;
	int		cnt=0;
	bool		reget=false;

	this->appName=name;

	display=XOpenDisplay(NULL);
	if(display!=NULL)
		this->isOnX11=true;

	workspace=getSIWorkSpace(display);
	screen=DefaultScreen(display);
	displaystr=DisplayString(display);

	if(suppliedkey==-1)
		{
			this->keystr=QString("%1%2%3%4").arg(this->appName).arg(workspace).arg(screen).arg(displaystr);
			this->key=hashFromKey(keystr);
		}
	else
		{
			this->key=suppliedkey;
			this->keystr=QString("%1%2").arg(this->appName).arg(this->key);
		}

	this->queueID=msgget(this->key,IPC_CREAT|0660);
	this->shmKey=hashFromKey(QString("%1%2").arg(keystr).arg("sharedmem"));
	this->shmQueueID=shmget(this->shmKey,SHAREDMEMSIZE,0);

//clean up defunct shared mem.
	if(this->shmQueueID!=-1)
		{
			int				maxid;
			struct shmid_ds	shmseg;
	
			maxid=shmctl(0,SHM_INFO,&shmseg);
			for(int j=0;j<=maxid;j++)
				{
					int				shmid;
					struct ipc_perm	*ipcp=&shmseg.shm_perm;

					shmid=shmctl(j,SHM_STAT,&shmseg);
					if((shmid<0)  || (ipcp->__key==0))
						continue;

					//printf("Key=0x%x UID=%i Perms=%o PID=%i\n",ipcp->__key,ipcp->uid,ipcp->mode,shmseg.shm_cpid);
					if(kill(shmseg.shm_cpid,0)!=0)
						{
							int id=shmget(ipcp->__key,0,0);
							int ret=shmctl(id,IPC_RMID,NULL);
						
							id=msgget(ipcp->__key,0);
							ret=msgctl(id,IPC_RMID,NULL);
							reget=true;
						}
				}
		}

	if(reget==true)
		{
			this->queueID=msgget(this->key,IPC_CREAT|0660);
			this->shmKey=hashFromKey(QString("%1%2").arg(keystr).arg("sharedmem"));
			this->shmQueueID=shmget(this->shmKey,SHAREDMEMSIZE,0);
		}

	if(this->shmQueueID==-1)
		{
			this->semid=sem_open(this->keystr.toStdString().c_str(),O_CREAT,0600,0);
			this->shmQueueID=shmget(this->shmKey,SHAREDMEMSIZE,IPC_CREAT|0600);
			this->queueAddr=(char*)shmat(this->shmQueueID,NULL,SHM_W);
			char *ptr=this->queueAddr;
			cnt=sprintf(ptr,"%i\n",getpid());
			cnt=sprintf(ptr+=cnt,"%s\n",keystr.toStdString().c_str());
			cnt=sprintf(ptr+=cnt,"0x%x\n",this->key);
			cnt=sprintf(ptr+=cnt,"%s\n",QString("%1%2").arg(keystr).arg("sharedmem").toStdString().c_str());
			sem_post(this->semid);
		}
	else
		{
			this->running=true;
			this->queueAddr=(char*)shmat(this->shmQueueID,NULL,SHM_W);
			this->semid=sem_open(this->keystr.toStdString().c_str(),0);
		}
	XCloseDisplay(display);
}

SingleInstanceClass::~SingleInstanceClass()
{
	if((this->isMulti==true) || (this->running==false))
		{
			shmdt(this->queueAddr);
			shmctl(this->shmQueueID,IPC_RMID,NULL);
			msgctl(this->queueID,IPC_RMID,NULL);
			sem_close(this->semid);
			sem_unlink(this->keystr.toStdString().c_str());
		}
}
