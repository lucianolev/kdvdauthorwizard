/* Page 1: Welcome Page */

#include "page_welcome.h"

Page_Welcome::Page_Welcome(QWidget* parent) : QWidget ( parent ) {

    m_layout = new QVBoxLayout();
    m_description = new QLabel(tr("This wizard will help you create DVD disks from MPEG2 movie files, a process called DVD Authoring. You need one or more DVD compatible MPEG2 files to use this wizard"));
    m_description->setWordWrap(true);

    m_layout->addWidget(m_description);
    setLayout(m_layout);
}
