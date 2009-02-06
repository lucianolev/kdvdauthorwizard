/* Widget: Playlist */

#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QTableWidget>
#include <QList>

#include "../movie.h"

class PlaylistWidget : public QWidget  {
Q_OBJECT

public:
	PlaylistPage(QWidget* parent = 0);

private slots:
	void addMovies();

signals:
	void moviesAdded(QList<Movie*>);

   private:
	QTableWidget* m_playlist;
//	QList<Movie*> m_movies;

};

#endif /* PLAYLISTWIDGET_H */
