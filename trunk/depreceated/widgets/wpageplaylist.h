/* Widget: Page Playlist */

#ifndef WPAGEPLAYLIST_H
#define WPAGEPLAYLIST_H

#include <KUrlRequester>

#include "../dvdvideo.h"

#include "../build/ui_pageplaylist2.h"

class WPagePlaylist : public QWidget  {
Q_OBJECT

    public:
	WPagePlaylist(QWidget* parent = 0, DVDVideo* dvdvideo = 0);

    private:
	DVDVideo* m_dvdvideo;
	KUrlRequester* m_fileselector;
	Ui::PagePlaylist ui;

};

#endif /* WPAGEPLAYLIST_H */
