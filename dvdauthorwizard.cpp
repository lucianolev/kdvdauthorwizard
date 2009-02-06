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
#include <KAssistantDialog>
#include <KLocale>
#include <KIcon>
#include <KPageWidgetItem>
#include <KUrlRequester>
#include <KFileDialog>
#include <KUrl>
#include <KMessageBox>
#include <KDebug>

//Qt
#include <QWidget>
#include <QList>
#include <QHeaderView>

//Local
#include "dvdauthorwizard.h"
#include "playlistmodel.h"
#include "dvdvideo.h"
#include "movie.h"

DVDAuthorWizard::DVDAuthorWizard(QWidget* parent) : KAssistantDialog ( parent ) {

		setFaceType( Plain );
		this->resize(590,390);
		this->showButton(KDialog::Help, false); //TODO: Enable Help button when help is available

	//creates the dvdvideo resource
		m_dvdvideo = new DVDVideo(this);

	/**
		Welcome page
	*/
	//setup the page widget
		QWidget* pagewelcome = new QWidget(this);
		ui_pagewelcome.setupUi(pagewelcome);
	//creates the page
		m_pageWelcome = new KPageWidgetItem( pagewelcome, i18n( "Welcome" ) );
		m_pageWelcome->setHeader( i18n( "Welcome to K DVD Author Wizard" ) );
		addPage(m_pageWelcome);

	/**
		Intro Movie page
	*/
	//setup the page widget
		QWidget* pageintromovie = new QWidget(this);
		ui_pageintromovie.setupUi(pageintromovie);
		ui_pageintromovie.fileselector->setMode(KFile::File);
		ui_pageintromovie.fileselector->setFilter("*.mpg *.mpeg|MPEG Movie Files (*.mpg, *.mpeg) ");
		ui_pageintromovie.fileselector->fileDialog()->setUrl(QDir::homePath());
	//creates the page
		m_pageIntroMovie = new KPageWidgetItem( pageintromovie, i18n( "Intro Movie" ) );
		m_pageIntroMovie->setHeader( i18n( "Introduction Movie" ) );
		addPage(m_pageIntroMovie);

	/**
		Movie Playlist Page
	*/
	//setup the page widget
	QWidget* pageplaylist = new QWidget(this);
	m_playlist = new PlaylistModel(this);
	ui_pageplaylist.setupUi(pageplaylist);
	ui_pageplaylist.tableView->setModel(m_playlist);
	ui_pageplaylist.tableView->horizontalHeader()->setStretchLastSection(true);
	//ui_pageplaylist.tableView->setVerticalHeader();
	//ui_pageplaylist.tableView->verticalHeader()->hide();
	connect(ui_pageplaylist.tableView->selectionModel(), 
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, 
		SIGNAL(selectionChanged(const QItemSelection &)));
	connect(ui_pageplaylist.addmovies_button, SIGNAL(clicked()), this, SLOT(selectMoviesDialog()));
	connect(ui_pageplaylist.removemovies_button, SIGNAL(clicked()), m_playlist, SLOT(removeAllMovies()));
	//creates the page
	m_pagePlaylist = new KPageWidgetItem( pageplaylist, i18n( "Playlist" ) );
	m_pagePlaylist->setHeader( i18n( "DVD Movies: The Playlist" ) );
	addPage(m_pagePlaylist);

	/**
		Storage Folder Page
	*/
	//setup the page widget
		QWidget* pagestoragefolder = new QWidget(this);
		ui_pagestoragefolder.setupUi(pagestoragefolder);
		ui_pagestoragefolder.fileselector->setMode(KFile::Directory);
		ui_pagestoragefolder.fileselector->setPath(QDir::homePath());
// 	connect(ui_storagefolderpage.fileselector->lineEdit(), SIGNAL(textChanged(const QString&)), 
// 		this, SLOT(storageFolderSelected(const KUrl&)));
	//creates the page
		m_pageStorageFolder = new KPageWidgetItem( pagestoragefolder, i18n( "Storage Folder" ) );
		m_pageStorageFolder->setHeader( i18n( "Storage Folder" ) );
		addPage(m_pageStorageFolder);

	/**
		Ready to Author the DVD Page
	*/
	QLabel* readytoauthor = new QLabel("We are ready to create the DVD structure. Just click the Next button to begin the process. Take into account that this process can take several minutes or even hours.");
	//creates the page
	readytoauthor->setWordWrap(true);
	m_pageReadyToAuthor = new KPageWidgetItem( readytoauthor, i18n( "Ready to Author the DVD" ) );
	m_pageReadyToAuthor->setHeader( i18n( "Ready to Author the DVD" ) );
	addPage(m_pageReadyToAuthor);

	/**
		Authoring the DVD Page
	*/
	//setup the page widget
	QWidget* pageauthoring = new QWidget(this);
	ui_pageauthoring.setupUi(pageauthoring);
	//creates the page
	m_pageAuthoring = new KPageWidgetItem( pageauthoring, i18n( "Authoring the DVD" ) );
	m_pageAuthoring->setHeader( i18n( "Authoring the DVD" ) );
	ui_pageauthoring.abortButton->setEnabled(false);

	connect(m_dvdvideo, SIGNAL(progressUpdated(int)), ui_pageauthoring.progressBar, SLOT(setValue(int)));
	connect(m_dvdvideo, SIGNAL(processRunning (bool)), this, SLOT(setButtons(bool)));
	connect(m_dvdvideo, SIGNAL(newProcessInfo(QString)), this, SLOT(appendNewLine(QString)));
	connect(ui_pageauthoring.abortButton, SIGNAL(clicked()), m_dvdvideo, SLOT(abortProcess()));
	//connect(m_dvdvideo, SIGNAL(authoringFinished(bool)), this, SLOT(next()));
	//TODO: connection that disables the next button when the authoring process starts
	addPage(m_pageAuthoring);

	/**
		Finish Page
	*/
	QLabel* done = new QLabel("The DVD creation process ended successfully.");
	m_pageFinish = new KPageWidgetItem( done, i18n( "Finish" ) );
	m_pageFinish->setHeader( i18n( "Finish" ) );
	addPage(m_pageFinish);

}

void DVDAuthorWizard::next() {
	if (currentPage() == m_pageIntroMovie)
	{
		KUrl selectedintro = ui_pageintromovie.fileselector->url();
		//remove the movie if no intro movie was selected
		if(selectedintro.url().isEmpty())
			m_dvdvideo->removeIntro();
		else
			m_dvdvideo->setIntro(selectedintro);
	}
	else if (currentPage() == m_pagePlaylist)
	{
		QList<Movie*> *movies = &(m_playlist->getMovies());
		if (!movies->isEmpty()) {
			m_dvdvideo->setMovies(movies);
		}
		else {
			KMessageBox::queuedMessageBox(this, KMessageBox::Error,
				i18n("Please select at least one movie for your DVD"),
				i18n("No movie selected"));
			return;
		}
	}
	else if (currentPage() == m_pageStorageFolder)
	{
		KUrl selectedfolder = ui_pagestoragefolder.fileselector->url();
		if(!selectedfolder.url().isEmpty())
		{
			//TODO: Check if the storage folder is valid
			m_dvdvideo->setOutputDir(selectedfolder);
		} else
			return;
	}
	else if (currentPage() == m_pageReadyToAuthor)
	{
		ui_pageauthoring.progressBar->setMaximum(m_dvdvideo->getNumberOfMovies());
		m_dvdvideo->authorDVD();
	}
	KAssistantDialog::next(); //move to the next page
}

void DVDAuthorWizard::selectMoviesDialog() {
	KUrl::List selected_movies = 
		KFileDialog::getOpenUrls(QDir::homePath(), "*.mpg *.mpeg|MPEG Movie Files (*.mpg, *.mpeg)", this);
	m_playlist->addMovies(selected_movies);
	ui_pageplaylist.tableView->resizeColumnsToContents();
	return;
}

void DVDAuthorWizard::setButtons(bool enabled) {
	this->enableButton(KDialog::Cancel, !enabled); //Cancel button
	this->enableButton(KDialog::User2, !enabled); //Next button
	this->enableButton(KDialog::User3, !enabled); //Back button
	ui_pageauthoring.abortButton->setEnabled(enabled); //Abort button

	return;
}

void DVDAuthorWizard::appendNewLine(QString newline) {
	ui_pageauthoring.infoProcess->append(newline);
}

// void DVDAuthorWizard::storageFolderSelected(const KUrl &folder) {
// 	if (!folder.url().isEmpty() && !isValid(m_pageStorageFolder))
// 		setValid(m_pageStorageFolder, true);
// 	if (folder.url().isEmpty() && isValid(m_pageStorageFolder))
// 		setValid(m_pageStorageFolder, false);
// 	return;
// }
