#ifndef ECHELLES_H
#define ECHELLES_H

#include <QWidget>
#include <QPen>
#include <QLabel>
#include <QPushButton>
#include <QTextStream>
#include <QPainter>
#include <QPaintEvent>

#include "mylabel.h"
#include "qpushkey.h"
#include "gestion.h"
#include "mylabel.h"

namespace Ui {
class Echelles;
}

class Echelles : public QWidget
{
    Q_OBJECT

public:
    explicit Echelles(QWidget *parent = 0);
    ~Echelles();
    void paintRect(QPainter *painterGraph);

private:
    Ui::Echelles *ui;
    int echelle;
    int typeGraphe;
    QStringList listEchelle;
    QStringList data;
    QWidget* w;

    QRect rectLabel;
    QPen penRect;
    int decX,decY;
    QList<myLabel*> listLabel;
    int pos; //choix du bouton dans la liste de bouutons(listLabel)
    void ecrireParametre(QString param);
    QPushKey* quitterKey;
    QPushButton* plusKey;
    QPushButton* moinsKey;

private slots:
    void updateEchelle(QLabel* lab);

    void plus();
    void moins();

    void quitterEchelle(QPushButton* but);
    void on_bt_Quitter_clicked();

    void on_bt_NAV_clicked();

    void on_bt_SELECT_clicked();

    void on_bt_MOINS_clicked();

    void on_bt_PLUS_clicked();

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
};

#endif // ECHELLES_H
