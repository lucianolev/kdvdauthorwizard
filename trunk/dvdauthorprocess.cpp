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

//KDE
#include <KProcess>
#include <KDebug>

//Qt
#include <QDir>

//local
#include "dvdauthorprocess.h"

DVDAuthorProcess::DVDAuthorProcess(QObject* parent, QDir* outputdir) : QObject( parent ) {

	m_outputdir = outputdir;
	m_version = QString();

}

void DVDAuthorProcess::run(QString xmlfilename) {

	QString program = "dvdauthor";
	QStringList arguments;
	arguments << "-x" << xmlfilename << "-o" << m_outputdir->path();

	m_process = new KProcess(this);
	m_process->setOutputChannelMode(KProcess::MergedChannels);
	m_process->setProgram(program, arguments);

	connect(m_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleErrors(QProcess::ProcessError)));
	connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(handleReadOutput()));
	connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(handleFinish(int, QProcess::ExitStatus)));

	m_process->start();

}

void DVDAuthorProcess::handleErrors(QProcess::ProcessError error) {

	kDebug() << "The process returned the following error code: " << error;

	if (error == QProcess::FailedToStart) {
		m_currentstatus = PROGRAM_NOT_FOUND;
		emit statusChanged(m_currentstatus);
	}

	return;
}

void DVDAuthorProcess::handleReadOutput() {

	QString newline = m_process->readLine();

	if (newline.startsWith("DVDAuthor::dvdauthor")) {
		m_version = newline.remove("\n");
		m_currentstatus = STARTED;
		kDebug() << "DVD authoring proccess started succesfully";
		kDebug() << newline;
		emit statusChanged(m_currentstatus);
	}

	if (newline.startsWith("STAT: Processing")) {
		m_currentstatus = MOVIE_PROCESSED;
		kDebug() << "Movie processed!";
		emit statusChanged(m_currentstatus);
	}

	return;
}

void DVDAuthorProcess::handleFinish(int finishcode, QProcess::ExitStatus exitstatus) {

	switch( exitstatus )
	{
		case QProcess::NormalExit:
			m_currentstatus = FINISHED;
			kDebug() << "DVD authoring proccess has finished succesfully!";
			break;
		case QProcess::CrashExit:
			m_currentstatus = CRASHED;
			kDebug() << "DVD authoring proccess has crashed!";
			break;		
	}

	kDebug() << "Finish Code: " << finishcode;

	emit statusChanged(m_currentstatus);

	return;
}

void DVDAuthorProcess::terminateProcess() {
	m_process->terminate();
}
