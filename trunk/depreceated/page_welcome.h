/* Page 1: Welcome Page */

#ifndef PAGE_WELCOME_H
#define PAGE_WELCOME_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class Page_Welcome : public QWidget {
Q_OBJECT

    public:
	Page_Welcome(QWidget* parent = 0);

    private:
	QVBoxLayout* m_layout;
	QLabel* m_description;

};

#endif /* PAGE_WELCOME_H */
