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

#define MAX_MSG_SIZE 4096

#define ALLOK 0
#define UNKNOWNARG 1
#define NOMAKEQUEUE 2
#define NOSENDMSG 3
#define WAIT_MSG 0

#define MSGANY 0
#define LISTENMSG 50

struct option long_options[] =
{
	{"command",1,0,'c'},
	{"data",1,0,'d'},
	{"key",1,0,'k'},
	{"wait",0,0,'w'},
	{"flush",0,0,'f'},
	{"help",0,0,'?'},
	{0, 0, 0, 0}
};

struct msgBuffer
{
	long mType;
	char mText[MAX_MSG_SIZE];
};

int			queueID;
msgBuffer	buffer;
bool		flushQueue=false;
int			sinkReturn;
bool		waitForMsg=false;

const char	*commandList[]={"activate","openfile","savefile","quit","restoresession","gotoline","searchdefine","selecttab","selecttabbyname","selecttabbypath","bookmark","closetab","closealltabs","setusermark","unsetusermark","moveto","selectbetween","paste","copy","cut","inserttext","insertnl","insertfile","printfile","waitforkkeditqt","showcontinue","runtool","activatemenubylabel","sendposdata","sendselectedtext",NULL};

void printHelp()
{
	unsigned cnt=0;
	printf("Usage: %s-%s [OPTION] [TEXT]\n"
	       "A CLI application to send messages to kkeditqt\n"
	       " -c, --command	Send command [TEXT]\n"
	       " -d, --data	Data to send [TEXT]\n"
	       " -f, --flush	Flush message queue quietly\n"
	       " -k, --key	Use key [INTEGER] instead of generated one\n"
	       " -w, --wait	Wait for message's to arrive (blocking)\n"
	       " -h, -?, --help	print this help\n\n"
	       "Report bugs to keithdhedger@gmail.com\n"
	       ,APPNAME,MSGVERSION);

	printf("\nCommands recognised by KKEditQT:\n");
	while(commandList[cnt]!=NULL)
		{
			printf("%s ",commandList[cnt]);
			cnt++;
		}
	printf("\n");
}

void sendMsg()
{
	if((msgsnd(queueID,&buffer,strlen(buffer.mText)+1,0))==-1)
		{
			fprintf(stderr,"Can't send message :(\n");
			exit(NOSENDMSG);
		}
}

void readMsg()
{
	int retcode;
	retcode=msgrcv(queueID,&buffer,MAX_MSG_SIZE,LISTENMSG,WAIT_MSG);

	printf("%s",buffer.mText);
}

int messageToType(const char *msgstring)
{
	int cnt=0;
	if(strcasecmp(msgstring,"sendmsg")==0)
		return(LISTENMSG);

	while(commandList[cnt]!=NULL)
		{
			if(strcasecmp(msgstring,commandList[cnt])==0)
				return(cnt+ACTIVATEAPPMSG);
			cnt++;
		}
	return(ACTIVATEAPPMSG);
}

int main(int argc, char **argv)
{
	int	c;
	int	key;
	int	retcode;
	const char *gg="G";

	buffer.mType=ACTIVATEAPPMSG;
	buffer.mText[0]=0;
	key=0xdeadbeef;

	while (1)
		{
			int option_index = 0;
			c = getopt_long (argc, argv, "?hwfad:k:c:",long_options, &option_index);
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

					case 'w':
						waitForMsg=true;
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
					retcode=msgrcv(queueID,&buffer,MAX_MSG_SIZE,MSGANY,IPC_NOWAIT);
					if(retcode<=1)
						flag=true;
				}
			return(ALLOK);
		}

	if (waitForMsg==true)
		{
			readMsg();
			return(ALLOK);
		}

	sendMsg();

	return(ALLOK);

}

