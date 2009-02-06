/* Widget: Playlist */

#include <KPushButton>
#include <KUrl>
#include <KFileDialog>

#include <QTableWidget>
#include <QVBoxLayout>
#include <QList>
#include <QtDebug>

#include "playlistpage.h"
#include "../movie.h"

PlaylistPage::PlaylistPage(QWidget* parent) : QWidget(parent) {
	
	KPushButton* addmovies_button = new KPushButton("Add Movie(s)", this);

	m_playlist = new QTableWidget(0, 3, this);
	m_playlist->setHorizontalHeaderLabels(QStringList() << "#" << "Filename" << "Movie Title");

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(addmovies_button);
	layout->addWidget(m_playlist);	

	connect(addmovies_button, SIGNAL(clicked()), this, SLOT(addMovies()));
}

void PlaylistPage::addMovies() {
	KUrl::List selected_movies = 
		KFileDialog::getOpenUrls(QDir::homePath(), "*.mpg *.mpeg|MPEG Movie Files (*.mpg, *.mpeg)", this);
	
	QList<Movie*> some_movies;

	int moviecounter = m_playlist->rowCount()+1;
	foreach(KUrl moviefile, selected_movies)
	{
		Movie* amovie = new Movie(moviefile);
		some_movies.append(amovie);

		qDebug() << "Aca entra" << m_playlist->rowCount();

		m_playlist->insertRow(moviecounter);
		
		QTableWidgetItem* cmovienumber = new QTableWidgetItem(moviecounter);
		m_playlist->setItem(moviecounter, 0, cmovienumber);

		QTableWidgetItem* cmoviefilename = new QTableWidgetItem(moviefile.pathOrUrl());
		m_playlist->setItem(moviecounter, 1, cmoviefilename);

		QTableWidgetItem* cmovietitle = new QTableWidgetItem(moviefile.fileName());
		m_playlist->setItem(moviecounter, 2, cmovietitle);
		
		moviecounter++;
	}
	
	qDebug() << some_movies.size();

	emit moviesAdded(some_movies);
}
