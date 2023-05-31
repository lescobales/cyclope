#include "parametre.h"
#include "ui_parametre.h"

Parametre::Parametre(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Parametre)
{
    ui->setupUi(this);
    ui->bt_Quitter->setGeometry(5,380,80,80);
    penRect = QPen(QColor(96,160,192));
    penRect.setWidth(3);

    decX = decY = pos = 0;
    listButton << ui->bt_Echelle << ui->bt_Date << ui->bt_Update;
}

Parametre::~Parametre()
{
    delete ui;
}

void Parametre::paintRect(QPainter *painterGraph)
{
    painterGraph->setPen(penRect);
    int x, y, w, h;
    x = listButton.at(pos)->x();
    y = listButton.at(pos)->y();
    w = listButton.at(pos)->width();
    h = listButton.at(pos)->height();

    rectButton.setRect(x - 3, y - 3, w + 6, h + 6);
    painterGraph->drawRect(rectButton);
}

void Parametre::on_bt_Echelle_clicked()
{
    Echelles* e = new Echelles;
    e->setGeometry(this->geometry());
    e->setWindowFlags(Qt::FramelessWindowHint);
    e->setBackgroundRole(QPalette::NoRole);
    e->show();
}

void Parametre::on_bt_Quitter_clicked()
{
    emit menuActif(0); //MainWidget actif
}

void Parametre::paintEvent(QPaintEvent *)
{
    QPainter painterGraph;
    painterGraph.begin(this);
    painterGraph.setRenderHint(QPainter::Antialiasing);
    this->paintRect(&painterGraph);
    painterGraph.end();
}

void Parametre::on_bt_NAV_clicked()
{
    pos++;
    if(pos > listButton.length() - 1)
        pos = 0;
    update();
}

void Parametre::on_bt_SELECT_clicked()
{
    for(int i = 0; i < listButton.length(); i++)
    {
        if(rectButton.contains(listButton.at(i)->x(), listButton.at(i)->y()))
            listButton.at(i)->click();
    }
}

void Parametre::on_bt_Date_clicked()
{

}

void Parametre::on_bt_Update_clicked()
{
    QProcess process;

    QStringList argument0, argument1, argument2, argument3, argument4;

    ///Montage de la cle
    QString mount("mount /dev/sda1 /media/usb");
    argument0 << "-c" << mount;
    process.start("/bin/sh", argument0);
    process.waitForFinished();

    QFile updateFile("/media/usb/Geoid/Update/Cyclope-v2");
    if(updateFile.exists())
    {
        ///Suppression du fichier
        QString rmCyclope("rm /home/root/Cyclope-v2");
        argument1 << "-c" << rmCyclope;
        process.start("/bin/sh",argument1);
        process.waitForFinished();

        ///Copie de la mise à jour
        QString copyUpdate("cp /media/usb/Geoid/Update/Cyclope-v2 /home/root/Cyclope-v2" );
        argument2 << "-c" << copyUpdate;
        process.start("/bin/sh", argument2);
        process.waitForFinished();

        ///Demontage de la cle
        QString umount("umount /media/usb");
        argument3 << "-c" << umount;
        process.start("/bin/sh", argument3);
        process.waitForFinished();

        ///Redémarrage du système
        QString reboot("reboot");
        argument4 << "-c" << reboot;
        process.start("/bin/sh",argument4);
        process.waitForFinished();
    }
}
