#include "mainwindow.h"
#include "view/tab.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QScreen>
#include <QApplication>

#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    const QDesktopWidget *const desktop = QApplication::desktop();
    setMinimumSize(900, 550);
    move((desktop->width() - this->width()) / 2, (desktop->height() - this->height()) / 2);

    qDebug("%d", desktop->logicalDpiX());

    initUI();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initUI()
{
    tabs = new QStackedWidget(this);
    setCentralWidget(tabs);

    Tab *tab1 = new Tab();
    tabs->addWidget(tab1);
    tabs->setCurrentIndex(0);

    QScreen *screen = QApplication::screens()[0];
    connect(screen, &QScreen::logicalDotsPerInchChanged, this, &MainWindow::onLogicalDPIChanged);
    qDebug("%f", screen->logicalDotsPerInch());

    if(QApplication::desktop()->logicalDpiX()>=144) {
        QFile file(":/hdpi_fit.qss");
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        setStyleSheet(in.readAll());
    } else {
        QFile file(":/default.qss");
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        setStyleSheet(in.readAll());
    }

}

void MainWindow::onLogicalDPIChanged(qreal dpi)
{
    qDebug("%f", dpi);
}
