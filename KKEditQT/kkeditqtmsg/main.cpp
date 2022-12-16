/*
 *
 * ©K. D. Hedger. Sun 20 Mar 19:10:57 GMT 2022 keithdhedger@gmail.com

 * This file (main.cpp) is part of KKEditQT.

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

#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <getopt.h>

#include "../../config.h"
#include "../src/kkedit-includes.h"

#define APPNAME "kkedtqtmsg"
#define MSGVERSION "0.7.0"

#define ALLOK 0
#define UNKNOWNARG 1
#define NOMAKEQUEUE 2
#define WAITFORMSG 0

#define GETMSG 0x1000

struct option long_options[] =
{
	{"command",1,0,'c'},
	{"data",1,0,'d'},
	{"key",1,0,'k'},
	{"info",0,0,'i'},
	{"flush",0,0,'f'},
	{"autowait",0,0,'a'},
	{"help",0,0,'?'},
	{0, 0, 0, 0}
};

struct msgBuffer
{
	long mType;
	char mText[MAXMSGSIZE];
};

int			queueID;
msgBuffer	buffer;
bool		flushQueue=false;
int			sinkReturn;
bool		waitForMsg=false;
bool		waitContinue=false;

const char	*commandList[]={"activate","openfile","savefile","quit","restoresession","gotoline","searchdefine","selecttab","selecttabbyname","selecttabbypath","bookmark","closetab","closealltabs","setusermark","unsetusermark","moveto","paste","copy","cut","inserttext","insertnl","insertfile","printfile","runtool","activatemenubylabel","openindocview",NULL};

const char	*infoList[]={"selectbetween","sendposdata","sendselectedtext","sendcurrenturl",NULL};

void printHelp()
{
	unsigned cnt=0;
	printf("Usage: %s-%s [OPTION] [TEXT]\n"
	       "A CLI application to send messages to kkeditqt\n"
	       " -c, --command	Send command [TEXT]\n"
	       " -d, --data	Data to send [TEXT]\n"
	       " -f, --flush	Flush message queue quietly\n"
	       " -k, --key	Use key [INTEGER] instead of generated one\n"
	       " -i, --info	Send information request and wait for reply to arrive, then print to stdout (blocking)\n"
	       " -a, --autowait	Wait for command to complete (blocking, no output)\n"
	       " -h, -?, --help	print this help\n\n"
	       "Report bugs to keithdhedger@gmail.com\n"
	       ,APPNAME,MSGVERSION);

	printf("\nCommands recognised by KKEditQT:\n");
	while(commandList[cnt]!=NULL)
		{
			printf("%s ",commandList[cnt]);
			cnt++;
		}
	cnt=0;
	printf("\n\nInformation resquests recognised by KKEditQT:\n");
	while(infoList[cnt]!=NULL)
		{
			printf("%s ",infoList[cnt]);
			cnt++;
		}
	printf("\n");
	printf("\nCommands that dont require a parameter:\n");
	printf("activate,quit,bookmark,closetab,closealltabs,setusermark,unsetusermark,paste,copy,cut,insertnl,printfile\n");
}

void sendMsg()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
	int	retcode;

	if(waitContinue==true)
		buffer.mType|=CONTINUEMSG;

	if((msgsnd(queueID,&buffer,strlen(buffer.mText)+1,0))==-1)
		{
			fprintf(stderr,"Can't send message :(\n");
			exit(NOSENDMSG);
		}

	if(waitContinue==true)
		retcode=msgrcv(queueID,&buffer,MAXMSGSIZE,CONTINUEMSG,WAITFORMSG);
#pragma GCC diagnostic pop
}

void waitMsg()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
	int retcode;

	buffer.mText[0]=0;
	if((msgsnd(queueID,&buffer,0,0))==-1)
		{
			fprintf(stderr,"Can't send message :(\n");
			exit(NOSENDMSG);
		}
	retcode=msgrcv(queueID,&buffer,MAXMSGSIZE,GETMSG,WAITFORMSG);

	printf("%s",buffer.mText);
#pragma GCC diagnostic pop
}

int messageToType(const char *msgstring)
{
	int cnt=0;
	int	cnt2=0;

	while(commandList[cnt]!=NULL)
		{
			if(strcasecmp(msgstring,commandList[cnt])==0)
				return(cnt+ACTIVATEAPPMSG);
			cnt++;
		}

	while(infoList[cnt2]!=NULL)
		{
			if(strcasecmp(msgstring,infoList[cnt2])==0)
				return(cnt+cnt2+ACTIVATEAPPMSG);
			cnt2++;
		}
	return(ACTIVATEAPPMSG);
}

int main(int argc, char **argv)
{
	int	c;
	int	key;
	int	retcode;

	buffer.mType=ACTIVATEAPPMSG;
	buffer.mText[0]=0;
	key=0xdeadbeef;

	while (1)
		{
			int option_index = 0;
			c = getopt_long (argc, argv, "?hfai:d:k:c:",long_options, &option_index);
			if (c == -1)
				break;

			switch (c)
				{
					case 'c':
						buffer.mType=messageToType(optarg);
						break;

					case 'd':
						strcpy(buffer.mText,optarg);
						break;

					case 'f':
						flushQueue=true;
						break;

					case 'i':
						buffer.mType=messageToType(optarg);
						waitForMsg=true;
						break;

					case 'a':
						waitContinue=true;
						break;

					case 'k':
						key=strtol(optarg,NULL,0);
						if((queueID=msgget(key,IPC_CREAT|0660))==-1)
							{
								fprintf(stderr,"Can't create message queue\n");
								exit(NOMAKEQUEUE);
							}
						break;

					case '?':
					case 'h':
						printHelp();
						return ALLOK;
						break;

					default:
						fprintf(stderr,"?? Unknown argument %c ??\n",c);
						printHelp();
						return UNKNOWNARG;
						break;
				}
		}

	if((queueID=msgget(key,IPC_CREAT|0660))==-1)
		{
			fprintf(stderr,"Can't create message queue\n");
			exit(NOMAKEQUEUE);
		}

	if (flushQueue==true)
		{
			bool flag=false;
			while(flag==false)
				{
					retcode=msgrcv(queueID,&buffer,MAXMSGSIZE,MSGANY,IPC_NOWAIT);
					if(retcode<=1)
						flag=true;
				}
			return(ALLOK);
		}

	if(waitForMsg==true)
		{
			waitContinue=false;
			waitMsg();
			return(ALLOK);
		}
	else
		sendMsg();

	return(ALLOK);

}


