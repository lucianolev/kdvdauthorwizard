/* Widget: Playlist */
/* Es la interfaz grafica dedicada a manejar las peliculas, es decir la PlaylistModel */

#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QTableView>

#include "playlistmodel.h"

class PlaylistWidget : public QWidget  {
	Q_OBJECT

public:
	PlaylistWidget(QWidget* parent = 0);
	QList<Movie*>& getAddedMovies() { return m_playlist->getMovies(); };

public slots:
	void addMovies();
	//void editMovieItem();
	//void removeMovieItem();

signals:
	void selectionChanged(const QItemSelection &selected);

private:
	PlaylistModel* m_playlist;
	QTableView* m_tableView;
};

#endif /* PLAYLISTWIDGET_H */
