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
Movie class

This class represents a movie of a DVD. It provides ways of preview the file, checking if the movie is valid, set movie propierties and possible future conversion methods.

**/

#ifndef MOVIE_H
#define MOVIE_H

#include <KUrl>

#include <QFile>
#include <QString>
#include <QtGlobal>

class Movie : public QObject {
Q_OBJECT

public:
	Movie(QObject* parent = 0);
	Movie(KUrl mpgurl, QObject* parent = 0);
	~Movie();
	QString getFilename();
	QString getTitle() { return m_title; };
	qint64 getSize() { return m_file->size(); };
	int getEndPause() { return m_endPause; };

public slots:
	void setMovieFile(KUrl mpgurl);
	void setTitle(QString mtitle) { m_title = mtitle; };
	void setEndPause(int endPause) { m_endPause = endPause; };
	//bool checkValid(); //TODO: Implementation
	//void playMovie(); //TODO: Implementation

private:
	QFile* m_file;
	QString m_title;
	int m_endPause;
};

#endif /* MOVIE_H */
