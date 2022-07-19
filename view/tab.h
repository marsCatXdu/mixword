#ifndef TAB_H
#define TAB_H

#include <QWidget>

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = nullptr);
    ~Tab();

private:
    void initUI();

signals:

public slots:
};

#endif // TAB_H
