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
K DVD Author Wizard GUI class 

Represents the graphical interface (the assistant) which interacts with the core (the DVDVideo class)

**/

#ifndef DVDAUTHORWIZARD_H
#define DVDAUTHORWIZARD_H

//KDE
#include <KAssistantDialog>
//#include <KUrl>

//Local
#include "dvdvideo.h"
#include "playlistmodel.h"
#include "build/ui_pagewelcome.h"
#include "build/ui_pageintromovie.h"
#include "build/ui_pagestoragefolder.h"
#include "build/ui_pageplaylist.h"
#include "build/ui_pageauthoring.h"

class DVDAuthorWizard : public KAssistantDialog {
Q_OBJECT

public:
	DVDAuthorWizard(QWidget* parent = 0);

protected slots:
	virtual void next();

private slots:
	void selectMoviesDialog();
	void setButtons(bool enabled);

	void appendNewLine(QString newline);
// 	void storageFolderSelected(const KUrl&);

signals:
	void selectionChanged(const QItemSelection &selected);

private:
	DVDVideo *m_dvdvideo;
	Ui::PageWelcome ui_pagewelcome;
	Ui::PageIntroMovie ui_pageintromovie;
	Ui::PageStorageFolder ui_pagestoragefolder;
  Ui::PagePlaylist ui_pageplaylist;
	Ui::PageAuthoring ui_pageauthoring;
	PlaylistModel *m_playlist;
	KPageWidgetItem *m_pageWelcome;
	KPageWidgetItem *m_pageIntroMovie;
	KPageWidgetItem *m_pagePlaylist;
	KPageWidgetItem *m_pageStorageFolder;
	KPageWidgetItem *m_pageReadyToAuthor;
	KPageWidgetItem *m_pageAuthoring;
	KPageWidgetItem *m_pageFinish;
};

#endif /* DVDAUTHORWIZARD_H */
