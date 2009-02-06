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

/** Playlist Model

Represents the abstract model of a movie playlist of a DVD.
It's table based with position, title and filename as columns.
It is used as the model of the table view located on the Playlist page of the assistant (GUI class).

**/

#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "movie.h"

class PlaylistModel : public QAbstractTableModel  {
Q_OBJECT

public:
	PlaylistModel(QObject* parent = 0);
	~PlaylistModel();
	Qt::ItemFlags flags( const QModelIndex &index ) const;
	QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
	bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());
	bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );

	int rowCount( const QModelIndex &parent = QModelIndex() ) const;
	int columnCount( const QModelIndex &parent = QModelIndex() ) const;

	QList<Movie*>& getMovies() const { return *m_movies; };

public slots:
	void addMovies(KUrl::List &selected_movies);
	void removeAllMovies();

private:
	QList<Movie*>* m_movies;
};

#endif /* PLAYLISTMODEL_H */
