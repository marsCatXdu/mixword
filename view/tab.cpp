#include "tab.h"
#include "fileencryptpage/fileencryptpage.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QFrame>

Tab::Tab(QWidget *parent) : QWidget(parent)
{
    initUI();
}

Tab::~Tab()
{

}

void Tab::initUI()
{
    QHBoxLayout *tabLayout = new QHBoxLayout(this);
    tabLayout->setMargin(0);
    tabLayout->setSpacing(0);

    QFrame *listFrame = new QFrame();
    listFrame->setFixedWidth(200);
    listFrame->setStyleSheet("QFrame {background-color: white}");
    QVBoxLayout *listLayout = new QVBoxLayout(listFrame);

    QStackedWidget *pageContainer = new QStackedWidget(this);

    FileEncryptPage *fileEncryptPage = new FileEncryptPage();
    pageContainer->addWidget(fileEncryptPage);
    pageContainer->setCurrentIndex(0);

    QLabel *menuTitle = new QLabel(tr("Functions"));

    QPushButton *page1Btn = new QPushButton(tr("File Encryption"));
    listLayout->addWidget(menuTitle);
    listLayout->addWidget(page1Btn);
    listLayout->addStretch();

    tabLayout->addWidget(listFrame);
    tabLayout->addWidget(pageContainer);

}
