/* Widget: Playlist */

#include <KPushButton>
#include <KUrl>
#include <KFileDialog>

#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QtDebug>

#include "playlistwidget.h"
#include "playlistmodel.h"

PlaylistWidget::PlaylistWidget(QWidget* parent) : QWidget(parent) {
	m_playlist = new PlaylistModel(this);
	m_tableView = new QTableView(this);

	QVBoxLayout* layout = new QVBoxLayout(this);
	KPushButton* addmovies_button = new KPushButton("Add Movie(s)", this);
	
	m_tableView->setModel(m_playlist);
	m_tableView->verticalHeader()->hide();
	m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_tableView->horizontalHeader()->setStretchLastSection(true);
	m_tableView->verticalHeader()->hide();
	m_tableView->setDragEnabled(true);
	m_tableView->setAcceptDrops(true);
	m_tableView->setDropIndicatorShown(true);
	m_tableView->setShowGrid(false);
	m_tableView->setDragDropMode(QAbstractItemView::InternalMove);
	m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	connect(m_tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
		this, SIGNAL(selectionChanged(const QItemSelection &)));

	layout->addWidget(addmovies_button);
	layout->addWidget(m_tableView);

	connect(addmovies_button, SIGNAL(clicked()), this, SLOT(addMovies()));
}

void PlaylistWidget::addMovies() {
	KUrl::List selected_movies = 
		KFileDialog::getOpenUrls(QDir::homePath(), "*.mpg *.mpeg|MPEG Movie Files (*.mpg, *.mpeg)", this);

	int counter = m_playlist->rowCount();
	m_playlist->insertRows(counter, selected_movies.size(), QModelIndex());

	foreach(KUrl moviefile, selected_movies)
	{
		QModelIndex index = m_playlist->index(counter, 2, QModelIndex());
		m_playlist->setData(index, moviefile.url());
		counter++;
	}
}

