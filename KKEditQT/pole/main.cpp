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
#include <QFile>
#include <unistd.h>

enum labelEnums {WINDOWTITLE=1,BODYLABEL,CANCELLABEL,NUMBEROFITEMS,CONTROLFILE,NOMOREARGS};

#define MAXMESSAGELEN 96

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
	QFile			control(argv[CONTROLFILE]);
	QString			result;

/*
QT_STYLE_OVERRIDE=fusion
Hack if using gtk2 style as pulse progress bar doesn't work.
*/
	QApplication::setStyle("fusion");

	if(control.open(QFile::WriteOnly|QFile::Truncate))
		{
			QTextStream out(&control);
			out<<argv[BODYLABEL]<<"\n0";
			control.close();
		}
	else
		{
			qCritical()<<"Can't open control file ...";
			return(1);
		}

	cancellabel=argv[CANCELLABEL];

	if(cancellabel.isEmpty()==true)
		cancellabel=QString();
	QProgressDialog progress(argv[BODYLABEL],cancellabel,0,QString(argv[NUMBEROFITEMS]).toInt(),nullptr,Qt::Dialog);

	progress.setMinimumWidth(400);
	progress.setWindowModality(Qt::WindowModal);
	progress.setWindowTitle(argv[WINDOWTITLE]);
	progress.setWindowFlags(progress.windowFlags() | Qt::WindowStaysOnTopHint);
	progress.setAutoClose(false);
	progress.setAutoReset(false);
	progress.setValue(0);
	progress.show();

	while(flag==true)
		{
			app.processEvents();
			if (progress.wasCanceled())
				flag=false;
			usleep(500);
			if(control.open(QFile::ReadOnly | QFile::Text))
				{
					QTextStream in(&control);
					result=in.readLine();
					if(result.compare("quit")==0)
						{
							progress.setValue(progress.maximum());
							progress.update();
							app.processEvents();
							flag=false;
							control.close();
							continue;
						}

					progress.setLabelText(truncateWithElipses(result));
					result=in.readLine();
					progress.setValue(result.toInt());
					control.close();
				}
		}

	QFile::remove(argv[CONTROLFILE]);
	return 0;
}