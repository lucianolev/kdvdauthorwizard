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

#include <QAbstractTableModel>
#include <QList>

#include "playlistmodel.h"
#include "movie.h"

PlaylistModel::PlaylistModel(QObject* parent) : QAbstractTableModel(parent) {
	m_movies = new QList<Movie*>();
};

PlaylistModel::~PlaylistModel() {
	delete m_movies;
};

void PlaylistModel::addMovies(KUrl::List &selected_movies) {
	int counter = rowCount();
	insertRows(counter, selected_movies.size(), QModelIndex());

	foreach(KUrl moviefile, selected_movies)
	{
		setData(index(counter, 1, QModelIndex()), moviefile.url());
		counter++;
	}
	return;
}

void PlaylistModel::removeAllMovies() {
	removeRows(0, rowCount(), QModelIndex());
	return;
}

int PlaylistModel::rowCount( const QModelIndex &parent  ) const { 
	Q_UNUSED(parent);
	return m_movies->size();
};

int PlaylistModel::columnCount( const QModelIndex &parent  ) const { 
	Q_UNUSED(parent);
	return 2;
};

QVariant PlaylistModel::data( const QModelIndex &index, int role ) const {

	if ( !index.isValid() || index.row() >= m_movies->size() )
		return QVariant();

	if ( role == Qt::DisplayRole || role == Qt::EditRole )
	{
		switch ( index.column() )
		{
			case 0:
				return m_movies->at(index.row())->getTitle();
			case 1:
				return m_movies->at(index.row())->getFilename();
			default:
				break;
		}
	}
	return QVariant();
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		switch (section) {
			case 0:
				return tr("Movie Title");

			case 1:
				return tr("Filename");

			default:
				return QVariant();
		}
	} else if (orientation == Qt::Vertical)
		return section+1;

	return QVariant();
}

bool PlaylistModel::insertRows(int position, int rows, const QModelIndex &index) {
	Q_UNUSED(index);
	beginInsertRows(QModelIndex(), position, position+rows-1);

	for (int row = 0; row < rows; row++) {
		Movie *movie = new Movie(this);
		m_movies->insert(position, movie);
	}

	endInsertRows();
	return true;
}

bool PlaylistModel::removeRows(int position, int rows, const QModelIndex &index) {
	Q_UNUSED(index);
	beginRemoveRows(QModelIndex(), position, position+rows-1);

	for (int row = 0; row < rows; ++row) {
		delete m_movies->at(position);
		m_movies->removeAt(position); //FIXME: Pierde memoria?
	}

	endRemoveRows();
	return true;
}

bool PlaylistModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
	if( index.isValid() && role == Qt::EditRole )
	{
		switch ( index.column() )
		{
			case 0:
				if( value.toString() == m_movies->at(index.row())->getTitle() )
					return false;
				m_movies->at(index.row())->setTitle(value.toString());
				break;
			case 1:
				m_movies->at(index.row())->setMovieFile(value.toString());
				break;
			default:
				return false;
		}
		emit dataChanged( index, index );
		return true;
	}
		return false;
}

Qt::ItemFlags PlaylistModel::flags( const QModelIndex &index ) const
{
	if(!index.isValid())
		return Qt::ItemIsEnabled;
	if(index.column() == 0)
		return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	else
		return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
}
