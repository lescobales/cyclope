#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QWidget>


class graphic : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit graphic(QWidget *parent = 0);
    ~graphic();

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // GRAPHIC_H
