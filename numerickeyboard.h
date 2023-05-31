#ifndef NUMERICKEYBOARD_H
#define NUMERICKEYBOARD_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>

#include "qpushkey.h"

namespace Ui {
class numericKeyboard;
}

class numericKeyboard : public QWidget
{
    Q_OBJECT

public:
    explicit numericKeyboard(QWidget *parent = 0);
    ~numericKeyboard();
    void setLine(QLineEdit *line);
    QLineEdit* getLine();

    QString selectionNomChamp(QString d);

    void affichage(QStringList list);

    void paintRect(QPainter *painterGraph);

private:
    Ui::numericKeyboard *ui;
    QList<QPushKey*> listKey;
    QLineEdit *Line;

    QLabel *lb_visu;
    QLabel* lb_nomChamp;

    QStringList listChar;
    int decX, decY;
    QRect rectKey;

    QPen penRect;

public slots:
    void show();
    void buttonClicked(QPushButton* w);
private slots:
    void on_bt_UP_clicked();
    void on_bt_LEFT_clicked();
    void on_bt_DOWN_clicked();
    void on_bt_RIGHT_clicked();
    void on_bt_SELECT_clicked();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // NUMERICKEYBOARD_H
