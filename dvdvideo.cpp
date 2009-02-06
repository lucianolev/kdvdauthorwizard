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

#include <KUrl>
#include <KDebug>

#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QString>
#include <QByteArray>
#include <QTextStream>

#include <QtXml>

#include "dvdvideo.h"
#include "movie.h"
#include "dvdauthorprocess.h"

DVDVideo::DVDVideo(QObject* parent) : QObject( parent ) {
	m_intro = NULL;
	m_movies = NULL;
	m_outputdir = NULL;
	m_tempdir = NULL;
	m_moviesprocessed = 0;
};

DVDVideo::~DVDVideo() {
	delete m_intro;
}

void DVDVideo::setOutputDir(KUrl output_dir) {
	m_outputdir = new QDir(output_dir.path());
	m_tempdir = new QDir(output_dir.path()+"/temp");
}

void DVDVideo::setIntro(KUrl introfile) { 
	//Only set the intro if there was none or it has changed
	if (m_intro == NULL || m_intro->getFilename() != introfile.url())
	{
		delete m_intro;
		m_intro = new Movie(introfile);
	}
};

void DVDVideo::removeIntro()
{
	delete m_intro;
	m_intro = NULL;
}

void DVDVideo::authorDVD() {
	QString processinfo;

	if (m_tempdir != NULL) {
		m_outputdir->mkpath(m_tempdir->path());
		processinfo = "Temp folder created: " + m_tempdir->path();
		kDebug() << processinfo;
		//emit newProcessInfo(processinfo);
	} else {
		kDebug() << "No storage folder selected! Exiting..."; //Cannot happen
		return;
	}

	//Generates the XML file for dvdauthor
	bool xmlgenerated = generateXML();

	if (!xmlgenerated) {
		processinfo = "DVD authoring process fail: No XML file!";
		kDebug() << processinfo;	
		emit newProcessInfo(processinfo);
		return;
	}

	m_dvdauthorprocess = new DVDAuthorProcess(this, m_outputdir);

	connect(m_dvdauthorprocess, SIGNAL(statusChanged(DVDAuthorProcess::Status_t)), 
					this, SLOT(updateProcessInfo(DVDAuthorProcess::Status_t)));
// 	connect(dvdauthorprocess, SIGNAL(progressUpdated(int)), 
// 					this, SLOT(sendCurrentProgress(int)));

	m_dvdauthorprocess->run(m_xmlfile->fileName());

	return;
}

int DVDVideo::getNumberOfMovies() {

	int totalmovies = 0;

	if (m_movies != NULL) {
		totalmovies = m_movies->size();
	}

	if (m_intro != NULL) {
		totalmovies++;
	}

	return totalmovies;
}

void DVDVideo::updateProcessInfo(DVDAuthorProcess::Status_t dvdauthorstatus) {

	QString output;

	switch (dvdauthorstatus)
	{
		case DVDAuthorProcess::PROGRAM_NOT_FOUND :
			output = "'dvdauthor' not found! Please make sure you have 'dvdauthor' installed.";
			emit processRunning(false);
			break;
		case DVDAuthorProcess::STARTED :
			output = "Starting DVD Authoring process: " + m_dvdauthorprocess->getVersion();
			m_moviesprocessed = 0;
			emit processRunning(true);
			break;
		case DVDAuthorProcess::MOVIE_PROCESSED :
			output = "Movie processed!";
			m_moviesprocessed++;
			emit progressUpdated(m_moviesprocessed);
			break;
		case DVDAuthorProcess::FINISHED :
			output = "The authoring process has finished succesfully!";
			emit processRunning(false);
			break;
		case DVDAuthorProcess::CRASHED :
			output = "The dvdauthor program has crashed!";
			emit processRunning(false);
			break;
	}

	kDebug() << "Process info updated!";

	emit newProcessInfo(output);
}

void DVDVideo::abortProcess() {

	m_dvdauthorprocess->terminateProcess(); //Terminate the dvdauthor's process
	
}

bool DVDVideo::generateXML() {
	kDebug() << "Generating XML data for dvdauthor...";
	QDomDocument xmloutput(""); //New XML with no DOCTYPE
	QDomElement root = xmloutput.createElement("dvdauthor"); //Root element
	QDomElement vmgm = xmloutput.createElement("vmgm"); //VMGM element, always present
	xmloutput.appendChild(root);
	root.setAttribute("dest", m_outputdir->path());
	root.appendChild(vmgm);

	//Generate XML data for the Intro
	if (m_intro != NULL) {
		QDomElement menus = xmloutput.createElement("menus");
		QDomElement pgc = xmloutput.createElement("pgc");
		QDomElement vob = xmloutput.createElement("vob");
		QDomElement post = xmloutput.createElement("post");
		QDomText introjumptext = xmloutput.createTextNode("jump vmgm title 1;");
		vmgm.appendChild(menus);
		menus.appendChild(pgc);
		vob.setAttribute("file", m_intro->getFilename());
		pgc.appendChild(vob);
		pgc.appendChild(post);
		post.appendChild(introjumptext);
	}

	QDomElement titleset = xmloutput.createElement("titleset"); //Titleset element, always present
	root.appendChild(titleset);
    
	//Generate XML data for the menu
	// if (menu != NULL) {
	// 	//TODO: It has to change the post.appendChild(introjumptext);!
	// 	introjumptext.setNodeValue("jump titleset 1 menu;");
	//TODO: It has to change the post.appendChild(titlejumptext); from the LAST movie!
	// 	titlejumptext.setNodeValue("call menu entry root;");
	// 	//TODO: Everthing else!
	//}

	QDomElement titles = xmloutput.createElement("titles"); //Titles element, always present
	QDomElement audio = xmloutput.createElement("audio");
	titleset.appendChild(titles);
	audio.setAttribute("lang","en"); //Set audio to english language always
	titles.appendChild(audio);

	//Generate XML data for every movie
	foreach (Movie* movie, *m_movies) {
		QDomElement pgc = xmloutput.createElement("pgc");
		QDomElement vob = xmloutput.createElement("vob");
		QDomElement post = xmloutput.createElement("post");
		titles.appendChild(pgc);
		pgc.appendChild(vob);
		vob.setAttribute("file",movie->getFilename());
		vob.setAttribute("chapters","0:0"); //No chapters
		vob.setAttribute("pause",movie->getEndPause()); //Adds custom pause to each movie
		//IMPORTANT: In original scripts last movie has a 5 sec pause, is it necessary?
	
		//Jump to title (post info) for every movie except the last one
		pgc.appendChild(post);
		if (movie != m_movies->last()) {
			QString nexttitle = QString::number(m_movies->indexOf(movie));
			QDomText titlejumptext = xmloutput.createTextNode("jump title "+nexttitle+";");
			post.appendChild(titlejumptext);
		}
	}
    
	m_xmlfile = new QFile(m_tempdir->path()+"/dvdauthor.xml"); //check
	QFileInfo xmlfileinfo(*m_xmlfile);
	QString filepath = xmlfileinfo.absoluteFilePath();
	kDebug() << "Saving XML to file:" << filepath;
	if (!m_xmlfile->open(QIODevice::WriteOnly)) {
		kDebug() << "Could not open file!";
		return false;
	} else {
		QByteArray data = xmloutput.toByteArray(2);
		m_xmlfile->write(data);
		m_xmlfile->close();
		kDebug() << "XML file saved succesfully!";
		m_xmlfilepath = filepath;
		return true;
	}

}

