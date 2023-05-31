#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QPaintEvent>

#include "renseignement.h"
#include "graphique.h"
#include "transfert.h"
#include "ressortie.h"
#include "moufflage.h"
#include "communication.h"
#include "parametre.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    MainWidget(Renseignement* r, Graphique* g, Transfert* t, Ressortie *re, Communication* c, Parametre *p);
    ~MainWidget();
    void getKey(QString key);


private slots:
    void on_bt_Forage_clicked();

    void on_bt_usb_clicked();

    void on_bt_quitter_clicked();

    void on_bt_Reglage_clicked();

    void on_bt_moufflage_clicked();

    void on_bt_ressortie_clicked();

    void debutMoufflage();

    void finMoufflage();

private:
    Ui::MainWidget *ui;
    Renseignement* renseignement;
    Graphique* graphique;

    Transfert* transfert;

    Ressortie* ressortie;

    Communication* communication;

    Moufflage *moufflage;

    Parametre *parametre;

    int MENU;

    void paintEvent(QPaintEvent *);


signals:
    void menuActif(int menu);


public slots:
    void show();
};

#endif // MAINWIDGET_H
