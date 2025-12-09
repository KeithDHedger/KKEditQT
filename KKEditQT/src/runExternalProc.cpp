/*
 *
 * ©K. D. Hedger. Thu  2 Oct 13:06:22 BST 2025 keithdhedger@gmail.com
 *
 * This file (QT_runExternalProc.cpp) is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This file is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "runExternalProc.h"

runExternalProcClass::~runExternalProcClass()
{
}

/**
* Main class constructor.
*
*/
runExternalProcClass::runExternalProcClass()
{
}

/**
* Run External Commands.
* \param std::string str Command line to run.
* \param bool capture Capture OP to std::string ( optional default=false ).
*
* \param std::string tofile Divert stdout to file at tofile ( optional ).
* \param bool append Append stdout to file at tofile ( optional ).
*
* \return Return std::string Captured stdout or "".
*
* \note No redirect  or line break symbols ( >; except in quotes )
*/
std::string runExternalProcClass::runExternalCommands(std::string str,bool capture,std::string tofile,bool append)
{
	std::string	dataout="";
	int			slen=0;
	char			*data=NULL;
	int			comnum=0;
	int			wordexpret=0;
	bool			inquote=false;
	char			found=0;
	int			wstatus=-1;
	int			filefd=-1;

	if(this->showCli==true)
		fprintf(stderr,"\n%s\n",str.c_str());

	slen=str.length();
	data=(char*)alloca(slen*8);
	for(int j=0;j<slen*8;j++)
		data[j]=0;

	strcpy(data,str.c_str());
	if(tofile.empty()==false)
		{
			if(append==true)
				filefd=open(tofile.c_str(), O_WRONLY|O_APPEND, 0666);
			else
				filefd=open(tofile.c_str(), O_WRONLY|O_CREAT, 0666);
		}

	while(strlen(data)>0)
		{
			int pipe_position=-1;
			int i=0;
			for(i=0;data[i]!=0;i++)
				{
					if(data[i]=='\'')
						i++;
					if(data[i]=='|')
						{
							inquote=false;
							found=0;
							for(int j=0;j<i;j++)
								{
									if(data[j]=='\\')
										j+=2;
									if((found==0) && ((data[j]=='"') || (data[j]=='\'')) )
										{
											found=data[j];
											inquote=true;
										}
									else if((found!=0) && (data[j]==found))
										{
											found=0;
											inquote=false;
										}
								}

							if(inquote==false)
								{
									pipe_position=i;
									break;
								}
						}
				}

			if(pipe_position!=-1)
				data[pipe_position]='\0';
			if((wordexpret=wordexp(data,&this->commandsVar[comnum],0))==0)
				comnum++;
			else
				{
					fprintf(stderr,"command parse fail error: %i\n",wordexpret);
					fprintf(stderr,"%s\n",data);
					exit(wordexpret);
				}

			if(pipe_position!=-1)
				data+=pipe_position+1;
			else
				data+=strlen(data);
		}

	int pipe_fds[2*(comnum)];
	int pipefdslast[2];
	pid_t pid[comnum];
	if(capture==true)
		{
			if(pipe(pipefdslast)==-1)
				{
					perror("pipe");
					exit(EXIT_FAILURE);
				}
		}

	for(int i=0;i<comnum;i++)
		{
			if(pipe(pipe_fds+i*2)==-1)
				{
					perror("pipe");
					exit(EXIT_FAILURE);
				}
		}

	for(int i=0;i<comnum;i++)
		{
			pid[comnum]=fork();
			if(pid[comnum]==-1)
				{
					perror("fork");
					exit(EXIT_FAILURE);
				}

			if(pid[comnum]==0)
				{
					if(i>0)
						dup2(pipe_fds[(i-1)*2],0);
					if(i<comnum-1)
						dup2(pipe_fds[i*2+1],1);
					else if(capture==true)
						dup2(pipefdslast[1],STDOUT_FILENO);
					else if(tofile.empty()==false)
						{
							close(STDOUT_FILENO);
							dup(filefd);
						}

					for(int j=0;j<2*(comnum);j++)
						close(pipe_fds[j]);

					execvp(this->commandsVar[i].we_wordv[0],this->commandsVar[i].we_wordv);
					//extern char **environ;
//
					//execvpe(this->commandsVar[i].we_wordv[0],this->commandsVar[i].we_wordv,environ);
					perror("execlp");
					exit(EXIT_FAILURE);
				}
		}

	for(int i=0;i<2*(comnum);i++)
		close(pipe_fds[i]);

	if(capture==true)
		{
			close(pipefdslast[1]);
			FILE *file=fdopen(pipefdslast[0],"r");
			if(file!=NULL)
				{
					while(fgets(this->buffer,BUFFER_SIZE-1,file))
						{
							dataout.append(this->buffer);
							if(this->realTimeCB!=NULL)
								this->realTimeCB(this,this->buffer);
							this->buffer[0]=0;
						}
					fclose(file);
				}
			close(pipefdslast[0]);
		}

	waitpid(pid[comnum],&wstatus,0);
	this->exitStatus=WEXITSTATUS(wstatus);

	for(int j=0;j<comnum;j++)
		wordfree(&this->commandsVar[j]);

	if(filefd>0)
		close(filefd);

	if(capture==true)
		{
			if(this->trimOP==true)
				return(trimmed(dataout));
			else
				return(dataout);
		}

	return("");
}

