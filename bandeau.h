#ifndef BANDEAU_H
#define BANDEAU_H

#include <QWidget>
#include <QProcess>
#include <QTimer>
#include <QLabel>

#include "mainwidget.h"
#include "renseignement.h"
#include "graphique.h"
#include "keyboard.h"
#include "transfert.h"
#include "ressortie.h"
#include "parametre.h"

namespace Ui {
class Bandeau;
}

class Bandeau : public QWidget
{
    Q_OBJECT

public:
    explicit Bandeau(QWidget *parent = 0);
    Bandeau(MainWidget* m, Renseignement* r, Graphique* g, Keyboard* k,  Transfert* t, Ressortie* re, Parametre* p);
    ~Bandeau();

private:
    Ui::Bandeau *ui;
    void traitementEnvoie();

    MainWidget* mainWidget;
    Renseignement* renseignement;
    Graphique* graphique;
    Keyboard* keyboard;
    Transfert* transfert;
    Ressortie* ressortie;
    Parametre* parametre;

    QString key;

    QPixmap logo;

    int menu;

    typedef enum
    {
        mainMenu = 0,
        rens,
        graph,
        trans,
        keyb,
        resso,
        param
    }Menu;

    QList<QWidget*> listFenetre;
private slots:
    void presenceUSB();

public slots:
    void setMenuActif(int menu);
    void getPress(QString key);
signals:
    void sendKeyPress(QString key);
};

#endif // BANDEAU_H
