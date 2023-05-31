#ifndef PARAMETRE_H
#define PARAMETRE_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QProcess>
#include <QDir>

#include "echelles.h"

namespace Ui {
class Parametre;
}

class Parametre : public QWidget
{
    Q_OBJECT

public:
    explicit Parametre(QWidget *parent = 0);
    ~Parametre();
    void paintRect(QPainter *painterGraph);

private slots:
    void on_bt_Echelle_clicked();

    void on_bt_Quitter_clicked();

    void on_bt_NAV_clicked();

    void on_bt_SELECT_clicked();

    void on_bt_Date_clicked();

    void on_bt_Update_clicked();

private:
    Ui::Parametre *ui;
    //QWidget *parentWidget;
    int MENU;
    QRect rectButton;
    QPen penRect;
    int decX,decY;
    QList<QPushButton*> listButton;
    int pos; //choix du bouton dans la liste de bouutons(listButton)

signals:
    void menuActif(int menu);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
};

#endif // PARAMETRE_H
