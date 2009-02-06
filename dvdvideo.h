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
DVD Video class (aka The Core)

This class represents a DVD disc as a whole as well as the processes involved in authoring it. It serves as the core to the GUI interface. It provides signals to inform the GUI about the core status. This is where the Movie class and the Menu class converge. It also provides managment for the dvdauthor process and all the other (future) processes.

**/

#ifndef DVDVIDEO_H
#define DVDVIDEO_H

#include <KUrl>

#include <QList>
#include <QDir>
#include <QString>

#include "movie.h"
#include "dvdauthorprocess.h"

//TODO: Como conecto las movies del playlistmodel con esta clase para podes usar authorDVD, etc??

class DVDVideo : public QObject {
Q_OBJECT

public:
	DVDVideo(QObject* parent = 0);
	~DVDVideo();
	QDir& getOutputDir() { return *m_outputdir; };

	int getNumberOfMovies();

public slots:
	void setOutputDir(KUrl outputdir);
	void setIntro(KUrl introfile); //An (optional) intro movie before menu
	void removeIntro(); //Removes the intro movie
	void authorDVD(); //Generates the menu MPGs, an XML and executes dvdauthor.
	void setMovies(QList<Movie*>* playlist) { m_movies = playlist; }; //Sync movies with the playlistmodel
	/*	void setMenu(); //A full featured menu*/

	void updateProcessInfo(DVDAuthorProcess::Status_t status); //Updates the current process information
	void abortProcess(); //Terminate the current running process

signals:
	//void progressUpdated(DVDAuthorProcess::Status_t);
	void newProcessInfo(QString); //Sends a string with information on the current running process
	void processRunning(bool); //Sends true when a process starts, sends false when all process finish
	void progressUpdated(int);

	/* TODO: 
		-No space left on device signal
		-DVD size exceeded signal
	*/

private:
	bool generateXML(); //Generates the XML file for dvdauthor.

	QList<Movie*>* m_movies;
	Movie* m_intro;
	QDir* m_outputdir;
	QDir* m_tempdir;
	QFile* m_xmlfile;
	QString m_xmlfilepath;

	DVDAuthorProcess* m_dvdauthorprocess;
	int m_moviesprocessed;
	/*	Menu* m_menu;*/

};

#endif /* DVDVIDEO_H */
