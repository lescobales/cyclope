#ifndef TRANSFERT_H
#define TRANSFERT_H

#include <QWidget>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>
#include <QDir>

#include "mylabel.h"


namespace Ui {
class Transfert;
}

class Transfert : public QWidget
{
    Q_OBJECT

public:
    explicit Transfert(QWidget *parent = 0);
    ~Transfert();
    void getKey(QString key);
    void rechercheForage();
    void creationLabel();

private slots:
    void on_bt_Transfert_clicked();
    void rowChange(QLabel* lab);

    void on_bt_UP_clicked();

    void on_bt_DOWN_clicked();

private:
    Ui::Transfert *ui;
    QStringList pathChantier;
    QStringList pathForage;

    QStringList nameForage;
    QStringList nameChantier;

    QList<QStringList> listChantiers;
    QList<QStringList> listForages;
    QList<QStringList> listPathForages;
    QList<QStringList> listPathChantiers;

    QList<myLabel*> listLabelChantiers;

    int MENU;

    int row;

public slots:
    void show();

signals:
    void menuActif(int menu);

};

#endif // TRANSFERT_H
