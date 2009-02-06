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

#include <QFile>
#include <QString>
#include <QtGlobal>

#include "movie.h"

Movie::Movie(QObject* parent) : QObject( parent ) {
	m_file = NULL;
	m_title = "Untitled";
	m_endPause = 2; //2 sec of pause by default
}

Movie::Movie(KUrl mpgurl, QObject* parent) : QObject( parent ) {
	m_file = new QFile(mpgurl.path());
	m_title = "Untitled";
	m_endPause = 2; //2 sec of pause by default
}

void Movie::setMovieFile(KUrl mpgurl) {
	delete m_file;
	m_file = new QFile(mpgurl.path());
}

Movie::~Movie() {
	delete m_file;
}

QString Movie::getFilename() {
	if (m_file != NULL)
		return m_file->fileName();
	else
		return tr("");
};
