#include "echelles.h"
#include "ui_echelles.h"

#include <QDebug>

Echelles::Echelles(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Echelles)
{
    ui->setupUi(this);

    echelle = 0;

    typeGraphe = -1;

    QFile file("parametres.txt");
    QString dataIntermediaire;
    QStringList listIntermediaire;
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    while (!stream.atEnd()){
        data.append(stream.readLine());

    } ;
    file.close();
    if(!data.isEmpty())
    {
        for(int i = 0; i < data.length(); i++)
        {
            dataIntermediaire = data.at(i);
            listIntermediaire = dataIntermediaire.split(";");
            if(listIntermediaire.at(0)  == "echelle")
            {
                for(int j = 1; j < 10; j++)
                {
                    listEchelle << listIntermediaire.at(j);
                }
            }
        }
    }

    ui->bt_MOINS->setVisible(false);
    ui->bt_PLUS->setVisible(false);

    ui->lb_Via->setText(listEchelle.at(0));
    ui->lb_PO->setText(listEchelle.at(1));
    ui->lb_CR->setText(listEchelle.at(2));
    ui->lb_PI->setText(listEchelle.at(3));
    int x, y, w, h;
    listLabel << ui->lb_TextVia << ui->lb_TextPO << ui->lb_TextCR << ui->lb_TextPI;
    int lenght = listLabel.length();
    for(int i = 0; i < lenght; i++)
    {
        x = listLabel.at(i)->x();
        y = listLabel.at(i)->y();
        w = listLabel.at(i)->width();
        if(i == 0)
            h = listLabel.at(i)->height() + ui->lb_CR->height();
        myLabel* lab = new myLabel(this);
        lab->setGeometry(x, y, w, h);
        listLabel << lab;
    }

    pos = 4;

    penRect = QPen(QColor(96,160,192));
    penRect.setWidth(3);

    connect(ui->lb_TextVia,SIGNAL(clicked(QLabel*)),this,SLOT(updateEchelle(QLabel*)));
    connect(ui->lb_TextPO,SIGNAL(clicked(QLabel*)),this,SLOT(updateEchelle(QLabel*)));
    connect(ui->lb_TextCR,SIGNAL(clicked(QLabel*)),this,SLOT(updateEchelle(QLabel*)));
    connect(ui->lb_TextPI,SIGNAL(clicked(QLabel*)),this,SLOT(updateEchelle(QLabel*)));
}

Echelles::~Echelles()
{
    delete ui;

}

void Echelles::paintRect(QPainter *painterGraph)
{
    painterGraph->setPen(penRect);
    int x, y, w, h;
    x = listLabel.at(pos)->x();
    y = listLabel.at(pos)->y();
    w = listLabel.at(pos)->width();
    h = listLabel.at(pos)->height();

    rectLabel.setRect(x - 3, y - 3, w + 6, h + 6);
    painterGraph->drawRect(rectLabel);
}

void Echelles::plus()
{
    if(typeGraphe == 0)
    {
        if(echelle < 2000)
            echelle += 100;

        ui->lb_Via->setText(QString::number(echelle));
    }
    else if(typeGraphe == 1)
    {
        if(echelle < 250)
            echelle += 50;

        ui->lb_PO->setText(QString::number(echelle));
    }
    else if(typeGraphe == 2)
    {
        if(echelle < 250)
            echelle += 50;

        ui->lb_CR->setText(QString::number(echelle));
    }
    else if(typeGraphe == 3)
    {
        if(echelle < 50)
            echelle += 10;

        ui->lb_PI->setText(QString::number(echelle));
    }

}

void Echelles::moins()
{
    if(typeGraphe == 0)
    {
        if(echelle > 0)
            echelle -= 100;

        ui->lb_Via->setText(QString::number(echelle));
    }
    else if(typeGraphe == 1)
    {
        if(echelle > 0)
            echelle -= 50;

        ui->lb_PO->setText(QString::number(echelle));
    }
    else if(typeGraphe == 2)
    {
        if(echelle > 0)
            echelle -= 50;

        ui->lb_CR->setText(QString::number(echelle));
    }
    else if(typeGraphe == 3)
    {
        if(echelle > 0)
            echelle -= 10;

        ui->lb_PI->setText(QString::number(echelle));
    }
}

void Echelles::quitterEchelle(QPushButton *but)
{
    if(but->objectName().contains("TextVia"))
    {
        ///Echelle en cours de forage et de visualisation
        listEchelle.replace(0,QString::number(echelle));
        listEchelle.replace(5,QString::number(echelle));
        Gestion g;
        g.ecrireParametre("echelle", data, listEchelle);
        ui->lb_TextVia->setVisible(true);
    }
    else if(but->objectName().contains("TextPO"))
    {
        ///Echelle en cours de forage et de visualisation
        listEchelle.replace(1,QString::number(echelle));
        listEchelle.replace(6,QString::number(echelle));
        Gestion g;
        g.ecrireParametre("echelle", data, listEchelle);
        ui->lb_TextPO->setVisible(true);
    }
    else if(but->objectName().contains("TextCR"))
    {
        ///Echelle en cours de forage et de visualisation
        listEchelle.replace(2,QString::number(echelle));
        listEchelle.replace(7,QString::number(echelle));
        Gestion g;
        g.ecrireParametre("echelle", data, listEchelle);
        ui->lb_TextCR->setVisible(true);
    }
    else if(but->objectName().contains("TextPI"))
    {
        ///Echelle en cours de forage et de visualisation
        listEchelle.replace(3,QString::number(echelle));
        listEchelle.replace(8,QString::number(echelle));
        Gestion g;
        g.ecrireParametre("echelle", data, listEchelle);
        ui->lb_TextPI->setVisible(true);
    }
    ui->bt_Quitter->setEnabled(true);
    typeGraphe = -1;
    w->~QWidget();
}

void Echelles::updateEchelle(QLabel *lab)
{
    if(typeGraphe == -1)
    {
        if(lab->objectName().contains("TextVia"))
        {
            typeGraphe = 0;
            echelle = listEchelle.at(0).toInt();
        }
        else if(lab->objectName().contains("TextPO"))
        {
            typeGraphe = 1;
            echelle = listEchelle.at(1).toInt();
        }
        else if(lab->objectName().contains("TextCR"))
        {
            typeGraphe = 2;
            echelle = listEchelle.at(2).toInt();
        }
        else if(lab->objectName().contains("TextPI"))
        {
            typeGraphe = 3;
            echelle = listEchelle.at(3).toInt();
        }

        lab->setVisible(false);
        w = new QWidget(this);
        w->setGeometry(lab->geometry());

        plusKey = new QPushButton(w);
        plusKey->setGeometry(w->width() - 30, w->height() - 30, 30, 30);
        plusKey->setText("+");
        connect(plusKey,SIGNAL(clicked(bool)),this,SLOT(plus()));

        moinsKey = new QPushButton(w);
        moinsKey->setGeometry(0, w->height() - 30, 30, 30);
        moinsKey->setText("-");
        connect(moinsKey,SIGNAL(clicked(bool)),this,SLOT(moins()));

        quitterKey = new QPushKey(w);
        quitterKey->setGeometry(0, 0, w->width(), w->height() - 30);
        quitterKey->setObjectName("bt_Quitter" + lab->objectName());
        quitterKey->setText("Sauvegarder");
        connect(quitterKey,SIGNAL(clique(QPushButton*)),this,SLOT(quitterEchelle(QPushButton*)));

        ui->bt_Quitter->setEnabled(false);
        w->show();
    }
}

void Echelles::on_bt_Quitter_clicked()
{
    this->~Echelles();
}

void Echelles::paintEvent(QPaintEvent *)
{
    QPainter painterGraph;
    painterGraph.begin(this);
    painterGraph.setRenderHint(QPainter::Antialiasing);
    this->paintRect(&painterGraph);
    painterGraph.end();
}

void Echelles::on_bt_NAV_clicked()
{
    pos++;
    if(pos > listLabel.length() - 1)
        pos = 4;
    update();
}

void Echelles::on_bt_SELECT_clicked()
{
    if(ui->bt_Quitter->isEnabled())
    {
        for(int i = 0; i < listLabel.length() / 2; i++)
        {
            if(rectLabel.contains(listLabel.at(i)->x(), listLabel.at(i)->y()))
            {
                listLabel.at(i)->clicked(listLabel.at(i));
                ui->bt_MOINS->setVisible(true);
                ui->bt_PLUS->setVisible(true);
            }
        }
    }

    else
    {
        quitterKey->clique(quitterKey);
    }
}

void Echelles::on_bt_MOINS_clicked()
{
    moinsKey->clicked(true);
}

void Echelles::on_bt_PLUS_clicked()
{
    plusKey->clicked(true);
}
