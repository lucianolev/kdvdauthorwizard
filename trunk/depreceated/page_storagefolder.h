/* Page 2: Storage Folder */

#ifndef PAGE_STORAGEFOLDER_H
#define PAGE_STORAGEFOLDER_H

#include <QLabel>
#include <QVBoxLayout>
#include <QWizardPage>

#include <KUrlRequester>

class Page_StorageFolder : public QWidget {
Q_OBJECT

    public:
	Page_StorageFolder(QWidget* parent = 0);

    private:
	QVBoxLayout* m_layout;
	QLabel* m_description;
	QLabel* m_fileselector_label;
	KUrlRequester* m_fileselector;
};

#endif /* PAGE_STORAGEFOLDER_H */
