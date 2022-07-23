#ifndef TAB_H
#define TAB_H

#include "common/ddestylemenubutton.h"

#include <QStackedWidget>
#include <QWidget>

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = nullptr);
    ~Tab();

private:
    void initUI();

    QStackedWidget *pageContainer = nullptr;

    QList<DDEStyleMenuButton *> btnList;

signals:

public slots:
    void showPage(int id);
};

#endif // TAB_H
