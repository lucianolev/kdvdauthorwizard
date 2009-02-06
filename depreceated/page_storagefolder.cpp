/* Page 2: Storage Folder */

#include "page_storagefolder.h"

Page_StorageFolder::Page_StorageFolder(QWidget* parent) : QWidget ( parent ) {

    m_layout = new QVBoxLayout();
    m_description = new QLabel(tr("Choose which folder should be used to store the DVD. There should be plenty of space free (around 10GB for a full dvd disc) in the storage folder"));
    m_fileselector_label = new QLabel(tr("Storage folder:"));
    m_fileselector = new KUrlRequester();
    m_fileselector->setMode(KFile::Directory);
    m_fileselector->setPath(QDir::homePath());

    m_description->setWordWrap(true);
    m_layout->addWidget(m_description);
    m_layout->addWidget(m_fileselector_label);
    m_layout->addWidget(m_fileselector);
    setLayout(m_layout);
    //connect(fileselector, SIGNAL(textChanged(const &QString)), this->dvdvideo, SLOT(setOutputDir(const &QString)));
}
