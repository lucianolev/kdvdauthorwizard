/* Widget: Page Playlist */

#include <KEditListBox>
#include <KFileDialog>

#include "wpageplaylist.h"

WPagePlaylist::WPagePlaylist(QWidget* parent, DVDVideo* dvdvideo) : QWidget (parent) {
    
    ui.setupUi(this);
    m_dvdvideo = dvdvideo;

    m_fileselector = new KUrlRequester(this);
    m_fileselector->setFilter("*.mpg *.mpeg|MPEG Movie Files (*.mpg, *.mpeg) ");
    m_fileselector->fileDialog()->setUrl(QDir::homePath());

    ui.playlist->setCustomEditor(m_fileselector->customEditor());

    connect(ui.playlist, SIGNAL(added(const QString)), m_dvdvideo, SLOT(addMovie(const QString)));
    connect(ui.playlist, SIGNAL(removed(const QString)), m_dvdvideo, SLOT(removeMovie(const QString)));

    //TODO:
    //connect(playlist, SIGNAL()
}
