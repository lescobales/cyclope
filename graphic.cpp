#include "graphic.h"
#include "ui_widget.h"

void graphic::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        qDebug() << "click";
    }
}
