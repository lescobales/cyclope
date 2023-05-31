#include "qpushkey.h"

#include <QFontDatabase>
QPushKey::QPushKey(QWidget *parent) : QPushButton(parent)
{
    //this->setStyleSheet("color : white");
    this->setFlat(true);
    //this->setBackgroundRole(QPalette::NoRole);


    this->setFont(QFont("Arial",15,63,false));

}

void QPushKey::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clique(this);
    }

}

void QPushKey::mouseReleaseEvent(QMouseEvent *)
{
    emit released();
}
