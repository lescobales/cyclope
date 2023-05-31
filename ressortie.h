#ifndef RESSORTIE_H
#define RESSORTIE_H

#include <QWidget>
#include <QTextStream>
#include <QPaintEvent>

#include "affichage.h"
#include "gestion.h"
#include "mylabel.h"


namespace Ui {
class Ressortie;
}

class Ressortie : public QWidget
{
    Q_OBJECT

public:
    explicit Ressortie(QWidget *parent = 0);
    ~Ressortie();

    void setEchelle(QList<int> ech, int typeGraph);
    int getEchelle(int typeGraph);

    void setGraduation(QList<int> grad, int typeGraph);
    int getGraduation(int typeGraph);


    void rechercheForages();

    void getKey(QString key);

    void creationLabel();

    void affichageGraphique();

    void setVisualisation(bool visu);
    bool getVisualisation();

private:
    Ui::Ressortie *ui;

    int MENU;

    Affichage   *affRessortieVia, *affRessortiePO, *affRessortieCR, *affRessortiePI,
                *affRessortieVAxe, *affRessortieHAxeVia, *affRessortieHAxePO,
                *affRessortieHAxeCR, *affRessortieHAxePI;

    Gestion* gestion;

    QList<Affichage*> listAff;

    QPushKey* pageUP;
    QPushKey* pageDOWN;

    QPushButton* zoomPlus;
    QPushButton* zoomMoins;
    QPushButton* selectGraph;

    QList<int> echelle;
    QList<int> graduation;

    int rowChantier;
    int rowForage;
    QStringList pathChantier;
    QStringList pathForage;

    QStringList nameForage;
    QStringList nameChantier;

    QList<QStringList> listChantiers;
    QList<QStringList> listForages;
    QList<QStringList> listPathForages;
    QList<QStringList> listPathChantiers;

    QString path;

    QList<myLabel*> listLabelChantiers;
    QList<myLabel*> listLabelForages;

    QList<QList<myLabel*> > listLabel;

    void infoUtilisateur(bool visu);

    bool visualisation;

    bool chantierSelect;

    int decX;
    int rectWidth;
    int choixGraph;

    void updateEchelle();
    void updateGraduation();
protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

public slots:
    void show();

private slots:
    void rowChantierChange(QLabel *lab);
    void rowForageChange(QLabel* lab);


    void on_bt_visualiser_clicked();
    void on_bt_Quitter_clicked();

    void on_bt_UP_clicked();

    void on_bt_DOWN_clicked();

    void on_bt_RIGHT_clicked();

    void on_bt_LEFT_clicked();

    void onBtZoomPlus();
    void onBtZoomMoins();
    void onBtSelectGraph();

signals:
    void menuActif(int menu);

};

#endif // RESSORTIE_H
