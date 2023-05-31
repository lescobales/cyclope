#ifndef QPUSHKEY_H
#define QPUSHKEY_H

#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>

class QPushKey : public QPushButton
{
    Q_OBJECT
public:
    explicit QPushKey(QWidget *parent = 0);

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

signals:
    void clique(QPushButton* w);
public slots:
};

#endif // QPUSHKEY_H
