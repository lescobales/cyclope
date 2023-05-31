#include "mymessagebox.h"

myMessageBox::myMessageBox(QWidget *parent) : QMessageBox (parent)

{
    QFont font = this->font();
    font.setPixelSize(19);
    this->setFont(font);
    qDebug()<<this->button(QMessageBox::Yes);
}
