#ifndef DDESTYLEMENUBUTTON_H
#define DDESTYLEMENUBUTTON_H

#include <QLabel>
#include <QFrame>
#include <QWidget>

class DDEStyleMenuButton : public QFrame
{
    Q_OBJECT
public:
    DDEStyleMenuButton(const QString & text, const int & id, QWidget *parent = nullptr);

signals:
    void clicked(int id);

public slots:
    void setSelected(bool isSelected);      // 是否正选中该按钮

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int m_id;

    QLabel *btnText = nullptr;
};

#endif // DDESTYLEMENUBUTTON_H
