
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QWidget>
#include <QLineEdit>
#include <QDebug>
#include <QPushButton>
#include <QMouseEvent>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>

#include "qpushkey.h"


namespace Ui {
class Keyboard;
}

class Keyboard : public QWidget
{
    Q_OBJECT

public:
    explicit Keyboard(QWidget *parent = 0);
    ~Keyboard();
    void affichage(QStringList list);
    QString selectionNomChamp(QString d);

    void setLine(QLineEdit *line);
    QLineEdit* getLine();

    void getKey(QString key);

    void paintRect(QPainter *painterGraph);

    void setShowKeyboard(bool show);
    bool getShowKeyboard();

private:
    Ui::Keyboard *ui;

    QStringList listChar;

    QLabel *lb_visu;
    QLabel* lb_nomChamp;

    QLineEdit* Line;

    int MENU;

    int decX, decY;

    QList<QPushKey*> listKey;

    QRect rectKey;

    QPen penRect;

    bool showKeyboard;
public slots:
    void buttonClicked(QPushButton* w);
    void show();

signals:
    void menuActif(int menu);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void on_bt_down_clicked();
    void on_bt_up_clicked();
    void on_bt_left_clicked();
    void on_bt_right_clicked();
    void on_bt_valide_clicked();
};

#endif // KEYBOARD_H
