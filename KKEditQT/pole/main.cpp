/*
 *
 * ©K. D. Hedger. Mon 29 Nov 13:21:04 GMT 2021 keithdhedger@gmail.com

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

#include <QtGui>
#include <QApplication>
#include <QProgressDialog>
#include <unistd.h>

enum labelEnums {WINDOWTITLE=1,BODYLABEL,CANCELLABEL,NUMBEROFITEMS,CONTROLFILE,NOMOREARGS};

//TODO//
//use normal window and add widgets.

int main(int argc, char **argv)
{
	FILE				*fp;
	char				line[256];
	char				command[256];
	bool				flag=true; 
	QApplication		app(argc, argv);
	QString			dialoglabel="Building docs ...";
	QString			windowtitle="Please wait ...";
	QString			cancellabel="Abort opertation";
	int				maxitems=0;

	sprintf(command,"echo \"%s\" > '%s'",argv[BODYLABEL],argv[CONTROLFILE]);
	system(command);

	windowtitle=argv[WINDOWTITLE];
	dialoglabel=argv[BODYLABEL];
	cancellabel=argv[CANCELLABEL];
	maxitems=atoi(argv[NUMBEROFITEMS]);

	if(cancellabel.isEmpty()==true)
		cancellabel=QString();
	QProgressDialog progress("",cancellabel,1,maxitems,nullptr,Qt::Dialog);
	progress.setWindowModality(Qt::WindowModal);
	progress.setWindowTitle(windowtitle);
	progress.setWindowFlags(progress.windowFlags() | Qt::WindowStaysOnTopHint);
	progress.setAutoClose(false);
	progress.setAutoReset(false);
	progress.setValue(1);
	progress.show();

	sprintf(command,"cat '%s' 2>/dev/null",argv[CONTROLFILE]);

	while(flag==true)
		{
			app.processEvents();
			if (progress.wasCanceled())
				flag=false;
			usleep(500);
			fp=popen(command, "r");
			while(fgets(line,256,fp))
				{
					if(strncasecmp(line,"quit",4)==0)
						{
							progress.setValue(progress.maximum());
							progress.update();
							app.processEvents();
							flag=false;
							continue;
						}
					else
						progress.setLabelText(line);
					fgets(line,256,fp);
					progress.setValue(atoi(line));
				}
			pclose(fp);
		}

	sprintf(command,"rm '%s' &>/dev/null",argv[CONTROLFILE]);
	system(command);
	return 0;
}