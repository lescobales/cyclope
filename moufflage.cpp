#include "moufflage.h"
#include "ui_moufflage.h"

Moufflage::Moufflage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Moufflage)
{
    ui->setupUi(this);

    ui->lb_Info->setText("Appuyer sur le boutton Début pour\nCommencer le Moufflage");
}

Moufflage::~Moufflage()
{
    delete ui;
}

void Moufflage::on_bt_Debut_clicked()
{
    ui->bt_Debut->setEnabled(false);
    emit debutMoufflage();
    ui->lb_Info->setText("Descendre la tête de 1 mètre, puis\n appuyer sur le boutton Fin");
}

void Moufflage::on_bt_Fin_clicked()
{
    emit finMoufflage();
    this->close();
}
