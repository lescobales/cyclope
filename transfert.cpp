#include "transfert.h"
#include "ui_transfert.h"

#include <QDebug>
#include <QProcess>

Transfert::Transfert(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Transfert)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0,0,800,470);

    row = -1;

    MENU = 3;


}

Transfert::~Transfert()
{
    delete ui;
}

void Transfert::getKey(QString key)
{
    if(key == "Bf")
        ui->bt_Transfert->click();
    else if(key == "Bd")
        ui->bt_UP->click();
    else if(key == "Be")
        ui->bt_DOWN->click();
}

void Transfert::rechercheForage()
{

    QStringList listeChantiers, listeForages;

    QFile file("/Geoid/Chantier/chantier.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    while (!stream.atEnd()){
        listeChantiers.append(stream.readLine());
    } ;
    QStringList ligneActuelle;
    for(int i = 0; i < listeChantiers.length(); i++)
    {
            ligneActuelle = listeChantiers.at(i).split(";");
            pathChantier << "/Geoid/Chantier/" + ligneActuelle.at(0);
            listPathChantiers << pathChantier;
            pathChantier.clear();
            nameChantier << ligneActuelle.at(1);
            listChantiers << nameChantier;
            nameChantier.clear();
            QFile file("/Geoid/Chantier/" +ligneActuelle.at(0)+"/forage.txt");

            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            while (!stream.atEnd()){
                listeForages.append(stream.readLine());
            } ;

            QStringList ligneActuelle2;
            for(int j = 0; j < listeForages.length(); j++)
            {
                    ligneActuelle2 = listeForages.at(j).split(";");
                    nameForage << ligneActuelle2.at(1);
                    pathForage << "/" + ligneActuelle2.at(0);
            }
            listPathForages << pathForage;
            pathForage.clear();
            listeForages.clear();
            listForages << nameForage;
            nameForage.clear();
    }
    listeChantiers.clear();

}

void Transfert::creationLabel()
{
    int dy = 120;
    ///Label des chantiers
    for(int i = 0; i < listChantiers.length(); i++)
    {
        myLabel* lb_Chantier = new myLabel(this);
        lb_Chantier->setObjectName("lb_Chantier"+ QString::number(i));
        lb_Chantier->setGeometry(300,dy,300,30);
        lb_Chantier->setStyleSheet("color : rgb(96,160,192)");
        lb_Chantier->setFont(QFont("Arial", 15, 63, false));
        lb_Chantier->setText(listChantiers.at(i).at(0));
        lb_Chantier->setVisible(true);
        listLabelChantiers << lb_Chantier;
        dy += 35;
        connect(lb_Chantier,SIGNAL(clicked(QLabel*)),this,SLOT(rowChange(QLabel*)));
    }


}


void Transfert::on_bt_Transfert_clicked()
{
if(this->row != -1)
{
    QProcess process;

    QStringList argument0, argument1, argument2, argument3, argument4;

    ///Montage de la cle
    QString mount("mount /dev/sda1 /media/usb");
    argument0 << "-c" << mount;
    process.start("/bin/sh", argument0);
    process.waitForFinished();


    ///Création du répertoire de copie si inexistant
    QDir folder("/media/usb/Geoid");
    if(!folder.exists())
    {
        QString dir("mkdir /media/usb/Geoid");
        folder.mkpath(dir);
        qDebug() << "folder Geoid";
    }
    QDir folder2("/media/usb/Geoid/Chantier");
    if(!folder2.exists())
    {
        QString dir("mkdir /media/usb/Geoid/Chantier");
        folder2.mkpath(dir);
        qDebug() << "folder Geoid/Chantier";
    }

    ///Copie du chantier
    QString cmde("cp -r "
                  + listPathChantiers[this->row].at(0) +" /media/usb/Geoid/Chantier");
    argument1 << "-c" << cmde;
    process.start("/bin/sh", argument1);
    process.waitForFinished();
    qDebug() << cmde;


    ///Changement du nom de chantier
    QString changementNomChantier("mv /media/usb" + listPathChantiers[this->row].at(0) + " /media/usb/Geoid/Chantier/"
            + listChantiers[this->row].at(0));
    argument2 << "-c" << changementNomChantier;
    process.start("/bin/sh", argument2);
    process.waitForFinished();
    qDebug() << changementNomChantier;


    ///Changement noms forages
    for(int i = 0; i < listForages.at(this->row).length(); i++)
    {
        QString changementNomForage("mv /media/usb/Geoid/Chantier/" + listChantiers[this->row].at(0) +
                listPathForages[this->row].at(i) + ".txt" + " /media/usb/Geoid/Chantier/"+ listChantiers[this->row].at(0) + "/"
                + listForages[this->row].at(i) + ".txt");
        qDebug() << changementNomForage;
        argument3 << "-c" << changementNomForage;
        process.start("/bin/sh", argument3);
        process.waitForFinished();
        argument3.clear();
    }


    ///Demontage de la cle
    QString umount("umount /media/usb");
    argument4 << "-c" << umount;
    process.start("/bin/sh", argument4);
    process.waitForFinished();



    //qDebug() << argument0 << "\r\n " << argument1 << "\r\n" << argument2 << "\r\n" << argument3 << "\r\n" << argument4;
}
for(int i = 0; i < listLabelChantiers.length(); i++)
{
    listLabelChantiers.at(i)->setStyleSheet("background : black");
}
emit menuActif(0); //Réactivation du menu principal
this->close();
}

void Transfert::rowChange(QLabel *lab)
{
    for(int i = 0; i < listLabelChantiers.length(); i++)
    {
        listLabelChantiers.at(i)->setStyleSheet("color : rgb(96,160,192)");
    }
   lab->setStyleSheet("background: rgb(96,32,64)");
   for(int i = 0; i < listLabelChantiers.length(); i++)
   {
       if(listLabelChantiers.at(i) == lab)
           this->row = i;
   }


}

void Transfert::show()
{
    emit menuActif(MENU);
    listChantiers.clear();
    listForages.clear();
    listLabelChantiers.clear();
    listPathForages.clear();
    listPathChantiers.clear();
    rechercheForage();
    creationLabel();
    QWidget::show();
}

void Transfert::on_bt_UP_clicked()
{
    if(!listLabelChantiers.isEmpty())
    {
        if(listLabelChantiers.last()->y() != 435)
        {
            int x, y, w, h;
            for(int i = 0; i < listLabelChantiers.length(); i++)
            {
                x = listLabelChantiers.at(i)->x();
                y = listLabelChantiers.at(i)->y();
                w = listLabelChantiers.at(i)->width();
                h = listLabelChantiers.at(i)->height();
                listLabelChantiers.at(i)->setGeometry(x, y - 35, w, h);
                if(y < 121 || y > 470)
                    listLabelChantiers.at(i)->setVisible(false);
                else
                    listLabelChantiers.at(i)->setVisible(true);
            }
        }
    }
}

void Transfert::on_bt_DOWN_clicked()
{
    if(!listLabelChantiers.isEmpty())
    {
        if(listLabelChantiers.at(0)->y() != 120)
        {
            int x, y, w, h;
            for(int i = 0; i < listLabelChantiers.length(); i++)
            {
                x = listLabelChantiers.at(i)->x();
                y = listLabelChantiers.at(i)->y();
                w = listLabelChantiers.at(i)->width();
                h = listLabelChantiers.at(i)->height();
                listLabelChantiers.at(i)->setGeometry(x, y + 35, w, h);
                if(y < 436 && y > 84)
                    listLabelChantiers.at(i)->setVisible(true);
                else
                    listLabelChantiers.at(i)->setVisible(false);
            }
        }
    }
}
