#include "ddestylemenubutton.h"

#include <QLabel>
#include <QVBoxLayout>

DDEStyleMenuButton::DDEStyleMenuButton(const QString & text, const int & id, QWidget *parent):
    QFrame(parent),
    m_id(id)
{
    QVBoxLayout* mainlayout = new QVBoxLayout(this);
    mainlayout->setContentsMargins(6, 4, 6, 4);
    mainlayout->setSpacing(0);
    btnText = new QLabel(QString(text));
    mainlayout->addWidget(btnText);
    setSelected(false);
}

void DDEStyleMenuButton::setSelected(bool isSelected)
{
    if(isSelected) {
        setStyleSheet("DDEStyleMenuButton { background-color: #0080FF; border-radius: 8px; padding: 5px} DDEStyleMenuButton:hover{background-color: #339CFF; }");
        btnText->setStyleSheet("background-color: transparent; color: white");
    } else {
        setStyleSheet("DDEStyleMenuButton { background-color: #FFFFFF; border-radius: 8px; padding: 5px} DDEStyleMenuButton:hover{background-color: #E3E3E3; }");
        btnText->setStyleSheet("background-color: transparent; color: #414D68");
    }
}

void DDEStyleMenuButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setSelected(true);
    emit clicked(m_id);
}
