#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <QWidget>
#include <QPushButton>

#include "affichage.h"
#include "qpushkey.h"
#include "gestion.h"


namespace Ui {
class Graphique;
}

class Graphique : public QWidget
{
    Q_OBJECT

public:
    explicit Graphique(QWidget *parent = 0);
    Graphique(QList<Affichage*> aff, Gestion* g);
    ~Graphique();
    
    bool getPause();
    void setPause(bool pause);

    QStringList getData();
    void setData(QString donnee);
    void updateData();

    void infoUtilisateur();

    void setEchelle(QList<int> ech, int typeGraph);
    int getEchelle(int typeGraph);

    void setGraduation(QList<int> grad, int typeGraph);
    int getGraduation(int typeGraph);

    void clearAffichage();

    void getKey(QString key);

    void setEnCoursForage(bool enCours);
    bool getEnCoursForage();

    void setAffichageVisible(bool visible);

    void updateEchelle();
    void updateGraduation();

    int getProfondeurForage();

private:
    Ui::Graphique *ui;

    ///Affichage durant le forage
    QList<Affichage*> affichage;

    ///Visualisation du forage courant
    QList<Affichage*> affVisu;

    Gestion* gestion;

    bool paused;
    bool initAffEnCours;
    bool enCoursForage;
    bool continuerForage;

    QStringList data;

    QList<int> echelle;
    QList<int> graduation;

    QPushKey* pageUP;
    QPushKey* pageDOWN;

    int MENU;

    QString profondeurForage;
public slots:
    void show();
private slots:
    void on_bt_pause_clicked();
    void on_bt_BS_clicked();

    void on_bt_Visu_clicked();

    void on_bt_Fin_clicked();

    void on_bt_Menu_clicked();

signals:
    void BS();
    void finForage();
    void menuActif(int menu);
    void menuShow();
    void coteForage(int cote);
};

#endif // GRAPHIQUE_H
