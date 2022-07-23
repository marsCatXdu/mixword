#include "tab.h"
#include "fileencryptpage/fileencryptpage.h"
#include "textencryptpage/textencryptpage.h"
#include "common/ddestylemenubutton.h"

#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>

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
    listFrame->setStyleSheet("QFrame {background-color: #FEFEFE}");
    QVBoxLayout *listLayout = new QVBoxLayout(listFrame);

    DDEStyleMenuButton *btn1 = new DDEStyleMenuButton(tr("File Encryption"), 0);
    DDEStyleMenuButton *btn2 = new DDEStyleMenuButton(tr("Text Encryption"), 1);
    btnList.push_back(btn1);
    btnList.push_back(btn2);
    connect(btn1, &DDEStyleMenuButton::clicked, this, &Tab::showPage);
    connect(btn2, &DDEStyleMenuButton::clicked, this, &Tab::showPage);
    listLayout->addWidget(btn1);
    listLayout->addWidget(btn2);
    listLayout->addStretch();

    pageContainer = new QStackedWidget(this);
    pageContainer->setStyleSheet("QStackedWidget {padding: 12px 12px 12px 12px; background-color: #F8F8F8}");
    FileEncryptPage *fileEncryptPage = new FileEncryptPage();
    TextEncryptPage *textEncryptPage = new TextEncryptPage();
    pageContainer->addWidget(fileEncryptPage);
    pageContainer->addWidget(textEncryptPage);

    tabLayout->addWidget(listFrame);
    tabLayout->addWidget(pageContainer);

    showPage(0);
}

void Tab::showPage(int id)
{
    pageContainer->setCurrentIndex(id);
    btnList.at(id)->setSelected(true);
    for(int i=0; i<pageContainer->count(); i++) {
        if(i!=id) {
            btnList.at(i)->setSelected(false);
        }
    }
}
