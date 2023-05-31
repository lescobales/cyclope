#ifndef RENSEIGNEMENT_H
#define RENSEIGNEMENT_H

#include <QWidget>
#include "keyboard.h"
#include "gestion.h"
#include "graphique.h"
#include "mylineedit.h"
#include "affichage.h"
#include "numerickeyboard.h"

#include <QPaintEvent>
#include <QPainter>

namespace Ui {
class Renseignement;
}

class Renseignement : public QWidget
{
    Q_OBJECT

public:
    explicit Renseignement(QWidget *parent = 0);
    Renseignement(Gestion* g, Graphique* gr, QList<Affichage*> a, Keyboard* k);
    ~Renseignement();
    void setOutRenseignement(QStringList listRens);
    void setInRenseignement();
    QStringList getRenseignement();
    void affichageDonnees();
    void paintRect(QPainter *painterGraph);
    void getKey(QString key);
    void initPosition(); //Positionne les champs à compléter aux valeur par défaut

private:
    Ui::Renseignement *ui;
    Gestion* gestion;
    Graphique* graphique;
    QList<Affichage*> affichage;
    Keyboard* keyboard;
    numericKeyboard* numKeyBoard;

    QList<QLineEdit*> listLine;
    QList<MyLineEdit*> listLineEdit;

    QList<QLabel*> listLabel;

    QStringList listRenseignement;

    MyLineEdit* myLine;

    int MENU;

    QRect rectLine;

    int decY;

    QPen penRect;
    int lePosX;
    int lePosY;
    int leWidth;
    int leHeight;

    bool comboSelected;
    int cbIndex;

private slots:
    void setLineEdit(MyLineEdit *line);
    void on_bt_valider_clicked();

    void on_bt_Menu_clicked();

    void on_bt_UP_clicked();

    void on_bt_DOWN_clicked();

    void on_bt_Selection_clicked();

public slots:
    void show();

signals:
    void menuShow();
    void menuActif(int menu);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // RENSEIGNEMENT_H
