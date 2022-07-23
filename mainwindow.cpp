#include "mainwindow.h"
#include "view/tab.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QApplication>

#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    const QDesktopWidget *const desktop = QApplication::desktop();
    setFixedSize(800, 450);
    move((desktop->width() - this->width()) / 2, (desktop->height() - this->height()) / 2);

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

    QFile file(":/default.qss");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    setStyleSheet(in.readAll());
}
