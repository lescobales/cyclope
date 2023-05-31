#ifndef MOUFFLAGE_H
#define MOUFFLAGE_H

#include <QWidget>

namespace Ui {
class Moufflage;
}

class Moufflage : public QWidget
{
    Q_OBJECT

public:
    explicit Moufflage(QWidget *parent = 0);
    ~Moufflage();

private:
    Ui::Moufflage *ui;

signals:
    void debutMoufflage();
    void finMoufflage();
private slots:
    void on_bt_Debut_clicked();
    void on_bt_Fin_clicked();
};

#endif // MOUFFLAGE_H
