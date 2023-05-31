#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class myLabel : public QLabel
{
    Q_OBJECT

public:

    myLabel(QWidget *parent);

signals:
    void clicked(QLabel* lab);

private:
    void mousePressEvent(QMouseEvent *);
};

#endif // MYLABEL_H
