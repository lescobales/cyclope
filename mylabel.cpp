#include "mylabel.h"

#include <QFontDatabase>

myLabel::myLabel(QWidget *parent) : QLabel(parent)
{

}

void myLabel::mousePressEvent ( QMouseEvent * )
{
        emit clicked(this);
}
