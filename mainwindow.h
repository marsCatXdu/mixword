#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initUI();

    QStackedWidget *tabs = nullptr;

    QWidget *centralWidget = nullptr;
    QHBoxLayout *mainLayout = nullptr;

};

#endif // MAINWINDOW_H
