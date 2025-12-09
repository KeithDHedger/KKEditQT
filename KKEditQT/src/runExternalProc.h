/*
 *
 * ©K. D. Hedger. Thu  2 Oct 13:06:22 BST 2025 keithdhedger@gmail.com
 *
 * This file (QT_runExternalProc.h) is free software: you can redistribute it and/or modify
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

#ifndef _RUNEXTERNALPROC_
#define _RUNEXTERNALPROC_

/**
* \brief class for Run External Proc Class.
*/

#define MAXARGS 1024
#define BUFFER_SIZE 4096

static const char *ws=" \t\n\r\f\v";

/**
* Trim from both ends of string (right then left).
* \param std::string str		String to trim.
* \param const char *t		Characters to trim.
* \return Return std::string	Trimmed str.
*
* \note String is trimmed inline.
* \note Defaut chars to trim are " \t\n\r\f\v".
*/
inline std::string& trimmed(std::string& s,const char *t=ws)
{
	s.erase(0,s.find_first_not_of(t));
	s.erase(s.find_last_not_of(t)+1);
	return(s);
}

class runExternalProcClass
{
	public:
		runExternalProcClass();
		~runExternalProcClass();

//vars
		int			exitStatus=-1;
/**
* Show commandline passed to function.
*/
		bool			showCli=false;
/**
* Trim output when capturing.
*/
		bool			trimOP=false;
//funcs
		std::string	runExternalCommands(std::string str,bool capture=false,std::string tofile="",bool append=false);
/**
* Real time callback.
* \param runExternalProcClass *theclass This class.
* \param std::string str String from stdout.
* \note If realTimeCB!=NULL AND capture=true, the callback realTimeCB will be run for every line received from stdout.
*/
		void			(*realTimeCB)(runExternalProcClass *theclass,std::string str)=NULL;
	private:
//vars
		wordexp_t	commandsVar[MAXARGS];
		char			buffer[BUFFER_SIZE];
};

#endif
