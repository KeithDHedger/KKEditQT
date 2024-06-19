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

#include <QApplication>
#include <QProgressDialog>
#include <QFile>
#include <unistd.h>

enum labelEnums {WINDOWTITLE=1,BODYLABEL,CANCELLABEL,NUMBEROFITEMS,CONTROLFILE,NOMOREARGS};

#define MAXMESSAGELEN 64

QString truncateWithElipses(const QString str)
{
	QString newlabel;
	if(str.length()>MAXMESSAGELEN)
		newlabel=str.left((MAXMESSAGELEN-3)/2)+"..."+str.right((MAXMESSAGELEN-3)/2);
	else
		newlabel=str;
	return(newlabel);
}

int main(int argc, char **argv)
{
	bool				flag=true; 
	QApplication		app(argc, argv);
	QString			cancellabel;
	QString			result;
	bool				showing=false;
	QFile			control(argv[CONTROLFILE]);

/*
QT_STYLE_OVERRIDE=fusion
Hack if using gtk2 style as pulse progress bar doesn't work.
*/
	QApplication::setStyle("fusion");

	cancellabel=argv[CANCELLABEL];

	if(cancellabel.isEmpty()==true)
		cancellabel=QString();
	QProgressDialog progress(argv[BODYLABEL],cancellabel,0,QString(argv[NUMBEROFITEMS]).toInt(),nullptr,Qt::Dialog);

	progress.setMinimumWidth(600);
	progress.setWindowModality(Qt::WindowModal);
	progress.setWindowTitle(argv[WINDOWTITLE]);
	progress.setWindowFlags(progress.windowFlags() | Qt::WindowStaysOnTopHint);
	progress.setAutoClose(false);
	progress.setAutoReset(false);
	progress.setValue(0);
	progress.hide();

	while(flag==true)
		{
			if (progress.wasCanceled())
				{
					flag=false;
					continue;
				}

			if(control.open(QFile::ReadOnly | QFile::Text))
				{
					QTextStream in(&control);
					result=in.readLine();
					if((result.length()>1) && (showing==false))
						{
							progress.show();
							showing=true;
						}

					if(result.compare("quit")==0)
						{
							progress.setValue(progress.maximum());
							progress.update();
							app.processEvents();
							flag=false;
							control.close();
							continue;
						}

					if(result.compare("pulse")==0)
						{
							progress.setLabelText(truncateWithElipses(in.readLine()));
							progress.setValue(0);
							progress.setMaximum(0);
							progress.setMinimum(0);
							control.close();
							app.processEvents();
							continue;
						}

					if(result.compare("progress")==0)
						{
							progress.setLabelText(truncateWithElipses(in.readLine()));
							progress.setValue(in.readLine().toInt());
							progress.setMinimum(in.readLine().toInt());
							progress.setMaximum(in.readLine().toInt());
							control.close();
							app.processEvents();
							continue;
						}

					progress.setLabelText(truncateWithElipses(result));
					result=in.readLine();
					progress.setValue(result.toInt());
					control.close();
					app.processEvents();
				}
			usleep(500);
		}

	QFile::remove(argv[CONTROLFILE]);
	return 0;
}
