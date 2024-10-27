/*
 *
 * ©K. D. Hedger. Fri 25 Oct 13:33:08 BST 2024 keithdhedger@gmail.com

 * This file (SingleInstanceClass.h) is part of KKEditQT.

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

#ifndef _SINGLEINSTANCECLASS_
#define _SINGLEINSTANCECLASS_

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#define SHAREDMEMSIZE 64

class SingleInstanceClass
{
	public:
		SingleInstanceClass(QString name,int suppliedkey=-1);
		~SingleInstanceClass();

		long				getSIWorkSpace(Display *display);
		void				*getX11Prop(Display	*display,Window w,Atom prop,Atom type,int fmt,unsigned long *rcountp);
		unsigned long	hashFromKey(QString key);

		bool				isOnX11=false;
		int				key;
		int				queueID=-1;
		int				shmQueueID=-1;
		unsigned long	shmKey=0;
		char				*queueAddr=NULL;
		bool				running=false;
		bool				isMulti=false;
		sem_t			*semid=NULL;
		QString			keystr;

	private:
		QString			appName;
};

#endif


