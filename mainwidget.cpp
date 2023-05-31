#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QProcess>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)

{

}

MainWidget::MainWidget(Renseignement *r, Graphique *g, Transfert *t, Ressortie *re, Communication *c, Parametre *p) : ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    MENU = 0;
    renseignement = r;
    graphique = g;
    transfert = t;
    ressortie = re;
    communication = c;
    parametre = p;
    this->setGeometry(0,0,800,460);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::getKey(QString key)
{
    if(key == "Ba")
        ui->bt_Reglage->click();
    else if(key == "Bb")
        ui->bt_moufflage->click();
    else if(key == "Bc")
        ui->bt_usb->click();
    else if(key == "Bd")
        ui->bt_Forage->click();
    else if(key == "Be")
        ui->bt_ressortie->click();
    else if(key == "Bf")
        ui->bt_quitter->click();
}


void MainWidget::show()
{
    emit menuActif(MENU);
    QWidget::show();

}


void MainWidget::on_bt_Forage_clicked()
{
    if(!graphique->getEnCoursForage())//Pas en cours de forage
    {
        this->setVisible(false);
        renseignement->setWindowFlags(Qt::FramelessWindowHint);
        renseignement->setGeometry(this->x(),this->y(),800,460);
        renseignement->show();
    }
    else
    {
        emit menuActif(2);//Menu des graphiques actifs
        graphique->updateEchelle();
        graphique->updateGraduation();
        graphique->setAffichageVisible(true);
        this->setVisible(false);
    }
}

void MainWidget::on_bt_usb_clicked()
{
    transfert->show();
}


void MainWidget::on_bt_quitter_clicked()
{
    qApp->quit();
    system("halt");

}

void MainWidget::on_bt_Reglage_clicked()
{
    parametre->setWindowFlags(Qt::FramelessWindowHint);
    parametre->setGeometry(this->geometry());
    parametre->menuActif(6);
    parametre->show();
}

void MainWidget::on_bt_moufflage_clicked()
{
    moufflage = new Moufflage;
    moufflage->setWindowFlags(Qt::FramelessWindowHint);
    moufflage->setGeometry(this->geometry());

    connect(moufflage,SIGNAL(debutMoufflage()),this,SLOT(debutMoufflage()));
    connect(moufflage,SIGNAL(finMoufflage()),this,SLOT(finMoufflage()));

    moufflage->show();

}

void MainWidget::on_bt_ressortie_clicked()
{
    ressortie->setWindowFlags(Qt::FramelessWindowHint);
    ressortie->setGeometry(this->x(),this->y(),800,460);
    ressortie->show();
}

void MainWidget::debutMoufflage()
{
    communication->debutMoufflage();
}

void MainWidget::finMoufflage()
{
    communication->finMoufflage();
}

void MainWidget::paintEvent(QPaintEvent *)
{
    QRectF target(150.0, 200.0, 500.0, 200.0);
     QImage image(":/prefix/logoGeoid.png");

     QPainter painter;
     painter.begin(this);
     //painter.setRenderHint(QPainter::Antialiasing);
     painter.drawImage(target, image);
     painter.end();
}
