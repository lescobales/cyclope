#include "bandeau.h"
#include "ui_bandeau.h"

Bandeau::Bandeau(QWidget *parent) :
    QWidget(parent)

{



}

Bandeau::Bandeau(MainWidget *m, Renseignement *r, Graphique *g, Keyboard *k, Transfert *t, Ressortie *re, Parametre *p) : ui(new Ui::Bandeau)
{
    ui->setupUi(this);

    this->setGeometry(0,460,800,20);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    QPalette pal( this->palette() );
    pal.setColor( QPalette::Window, Qt::black );
    this->setPalette( pal );

    mainWidget = m;
    renseignement = r;
    graphique = g;
    keyboard = k;
    transfert = t;
    ressortie = re;
    parametre = p;

    listFenetre << mainWidget << renseignement << graphique << keyboard << transfert << ressortie << parametre;

                  /* mainWidget       0
                   * Renseignement    1
                   * Graphique        2
                   * Transfert        3
                   * Keyboard         4
                   * Ressortie        5
                   * Parametre        6
                   * */
    QTimer *presenceUSB = new QTimer(this);
    presenceUSB->start(1000);

    ui->lb_USB->setGeometry(750,0,50,20);
    ui->lb_USB->setText("USB");
    ui->lb_USB->setAlignment(Qt::AlignCenter);

    logo.load(":/prefix/USB.png");
    logo.setDevicePixelRatio(1);

    this->menu = -1;

    connect(presenceUSB,SIGNAL(timeout()),this,SLOT(presenceUSB()));
}

Bandeau::~Bandeau()
{
    delete ui;
}

void Bandeau::traitementEnvoie()
{
    if(!key.isEmpty())
    {
        switch(menu)
        {
        case mainMenu:
            mainWidget->getKey(key);
            break;
        case rens:
            renseignement->getKey(key);
            break;
        case graph:
            graphique->getKey(key);
            break;
        case trans:
            transfert->getKey(key);
            break;
        case keyb:
            keyboard->getKey(key);
            break;
        case resso:
            ressortie->getKey(key);
        }
    }
}

void Bandeau::presenceUSB()
{
    ///Fonctionnel
    QStringList argument;
    argument << "-c" << "fdisk -l | grep /dev/sda1";
    QProcess process;
    process.start("/bin/sh", argument);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();

    if(!output.isEmpty())
    {
        ui->lb_USB->setStyleSheet("background : green");
        //ui->lb_USB->setPixmap(logo);
    }
    else
    {
        //logo.load(":/prefix/NO_USB.png");
        ui->lb_USB->setStyleSheet("background : red");
        //ui->lb_USB->setPixmap(logo);
    }
}

void Bandeau::setMenuActif(int menu)
{
    if(this->menu != -1)
        listFenetre.at(this->menu)->setVisible(false);

    switch(menu)
    {
    case mainMenu:
        mainWidget->setVisible(true);
        break;
    case rens:
        renseignement->setVisible(true);
        break;
    case graph:
        graphique->setVisible(true);
        break;
    case trans:
        transfert->setVisible(true);
        break;
    case resso:
        ressortie->setVisible(true);
        break;
    case param:
        parametre->setVisible(true);
        break;
    }
    this->menu = menu;

}

void Bandeau::getPress(QString key)
{
    this->key = key;
    traitementEnvoie();
}
