#ifndef MYMESSAGEBOX_H
#define MYMESSAGEBOX_H

#include <QMessageBox>
#include <QFont>
#include <QDebug>

class myMessageBox : public QMessageBox
{
    Q_OBJECT

public:
    myMessageBox(QWidget *parent =0);
};

#endif // MYMESSAGEBOX_H
