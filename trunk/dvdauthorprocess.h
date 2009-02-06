/*
		This file is part of K DVD Author Wizard.
		Copyright (C) 2009 Luciano Leveroni <lucianolev at gmail.com>

		K DVD Author Wizard is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, either version 3 of the License, or
		(at your option) any later version.

		Foobar is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

/** 
dvdauthor's process manager class

It handles all the operations related to the dvdauthor program execution and output information. 
It provides signals to show the current process status.

**/

#ifndef DVDAUTHORPROCESS_H
#define DVDAUTHORPROCESS_H

//KDE
#include <KProcess>

//Qt
#include <QDir>

class DVDAuthorProcess : public QObject {
Q_OBJECT

public:
	DVDAuthorProcess(QObject* parent = 0, QDir* outputdir = 0);
	void run(QString xmlfilename);
	QString getVersion() { return m_version; };

	enum Status_t {
		PROGRAM_NOT_FOUND,
		STARTED,
		MOVIE_PROCESSED,
		FINISHED,
		CRASHED
	};

public slots:
	void handleErrors(QProcess::ProcessError error);
	void handleReadOutput();
	void handleFinish(int, QProcess::ExitStatus);
	void terminateProcess();

signals:
	void statusChanged(DVDAuthorProcess::Status_t);
	void newOutputLine(QString); //For debuging

private:
	KProcess* m_process;
	QString m_version;
	QDir* m_outputdir;

	Status_t m_currentstatus;

};

#endif /* DVDAUTHORPROCESS_H */