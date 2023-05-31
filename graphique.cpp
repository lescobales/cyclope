#include "graphique.h"
#include "ui_graphique.h"

#include <QDebug>

Graphique::Graphique(QWidget *parent) :
    QWidget(parent)

{

}

Graphique::Graphique(QList<Affichage *> aff, Gestion *g) : ui(new Ui::Graphique)
{
    ui->setupUi(this);

    MENU = 2;
    QPalette p( this->palette() );
    p.setColor( QPalette::Window, Qt::black );
    this->setPalette( p );

    affichage = aff;
    gestion = g;

    this->setMinimumWidth(800);

    paused = true;

    setEnCoursForage(false);

    initAffEnCours = true;

    updateEchelle();
    updateGraduation();

    for(int i = 0; i < graduation.length(); i++)
    {
        setGraduation(graduation, i);
    }

    setPause(paused);
    infoUtilisateur();

    ui->lb_TextVia->setGeometry(100,420,155,20);
    ui->lb_TextPO->setGeometry(255,420,155,20);
    ui->lb_TextCR->setGeometry(410,420,155,20);
    ui->lb_TextPI->setGeometry(565,420,155,20);

    ui->lb_vitesse->setGeometry(100,435,155,30);
    ui->lb_PO->setGeometry(255,435,155,30);
    ui->lb_CR->setGeometry(410,435,155,30);
    ui->lb_PI->setGeometry(565,435,155,30);

    ui->bt_Fin->setVisible(true);

    ui->bt_Visu->setText("Visu");


    ui->bt_pause->setText("Départ");

    pageUP = new QPushKey(this);
    pageUP->setVisible(false);

    pageDOWN = new QPushKey(this);
    pageDOWN->setVisible(false);

    profondeurForage = "0.00";

}

Graphique::~Graphique()
{
    delete ui;
}

bool Graphique::getPause()
{
    return paused;
}

void Graphique::setPause(bool pause)
{
    paused = pause;
}

QStringList Graphique::getData()
{
    return data;
}

void Graphique::setData(QString donnee)
{
    data = donnee.split(";");
    if(data.length() < 6 && data.length() > 2)
        data << "0";
    updateData();

}

void Graphique::updateData()
{


    ui->lb_profForage->setText(QString::number(data.at(0).toFloat() / 100,'f', 2));
    ui->lb_profOutil->setText(QString::number(data.at(5).toFloat() / 100, 'f', 2));
    ui->lb_vitesse->setText(QString::number(data.at(1).toFloat() / 100, 'f', 1));
    ui->lb_PO->setText(data.at(2));
    ui->lb_CR->setText(data.at(3));
    ui->lb_PI->setText(data.at(4));
}

void Graphique::infoUtilisateur()
{
    if(getPause())
    {
        ui->lb_profForage->setStyleSheet("color: red");
        ui->lb_profOutil->setStyleSheet("color: red");
        ui->lb_vitesse->setStyleSheet("color: red");
        ui->lb_PO->setStyleSheet("color: red");
        ui->lb_CR->setStyleSheet("color: red");
        ui->lb_PI->setStyleSheet("color: red");

        ui->lb_TextProf->setStyleSheet("color: red");
        ui->lb_TextOutil->setStyleSheet("color: red");
        ui->lb_TextVia->setStyleSheet("color: red");
        ui->lb_TextPO->setStyleSheet("color: red");
        ui->lb_TextCR->setStyleSheet("color: red");
        ui->lb_TextPI->setStyleSheet("color: red");
    }
    else
    {
        ui->lb_profForage->setStyleSheet("color: white");
        ui->lb_profOutil->setStyleSheet("color: white");
        ui->lb_vitesse->setStyleSheet("color: white");
        ui->lb_PO->setStyleSheet("color: white");
        ui->lb_CR->setStyleSheet("color: white");
        ui->lb_PI->setStyleSheet("color: white");

        ui->lb_TextProf->setStyleSheet("color: white");
        ui->lb_TextOutil->setStyleSheet("color: white");
        ui->lb_TextVia->setStyleSheet("color: white");
        ui->lb_TextPO->setStyleSheet("color: white");
        ui->lb_TextCR->setStyleSheet("color: white");
        ui->lb_TextPI->setStyleSheet("color: white");
    }

}

void Graphique::setEchelle(QList<int> ech, int typeGraph)
{

    echelle[typeGraph] =  ech[typeGraph];
}

int Graphique::getEchelle(int typeGraph)
{
    return echelle[typeGraph];
}

void Graphique::setGraduation(QList<int> grad, int typeGraph)
{
    graduation[typeGraph] =  grad[typeGraph];
}

int Graphique::getGraduation(int typeGraph)
{
    return graduation[typeGraph];
}

void Graphique::clearAffichage()
{
    
    ui->lb_profForage->setText(profondeurForage);
    ui->lb_profOutil->setText(profondeurForage);
    ui->lb_vitesse->setText("0");
    ui->lb_PO->setText("0");
    ui->lb_CR->setText("0");
    ui->lb_PI->setText("0");
}

void Graphique::getKey(QString key)
{
    if(key == "Ba")
        ui->bt_pause->click();
    else if(key == "Bb")
        qDebug() << "Do nothing";
    else if(key == "Bc")
        ui->bt_BS->click();
    else if(key == "Bd")
        ui->bt_Visu->click();
    else if(key == "Be")
    {
        if(affichage[9]->getVisualisation())
            pageUP->clique(pageUP);
        else if(!affichage[9]->getVisualisation())
            ui->bt_Menu->click();
    }
    else if(key == "Bf")
    {
        if(affichage[9]->getVisualisation())
            pageDOWN->clique(pageDOWN);
        else if(!affichage[9]->getVisualisation())
            ui->bt_Fin->click();
    }

}

void Graphique::setEnCoursForage(bool enCours)
{
    enCoursForage = enCours;
}

bool Graphique::getEnCoursForage()
{
    return enCoursForage;
}

void Graphique::setAffichageVisible(bool visible)
{
    for(int i = 0; i < affichage.length() - 9; i++)
    {
        affichage[i]->setEchelle(echelle[i]);
        affichage[i]->setGraduation(graduation[i]);
        affichage[i]->setVisible(visible);
    }
}

void Graphique::updateEchelle()
{
    echelle.clear();
    QStringList echelleParametre = gestion->lectureParametre("echelle");
    for(int i = 0; i < echelleParametre.length(); i++)
    {
        echelle << echelleParametre.at(i).toInt();
    }

    for(int i = 0; i < echelle.length(); i++)
    {
        setEchelle(echelle, i);
    }
}

void Graphique::updateGraduation()
{
    graduation.clear();
    for(int i = 0; i < echelle.length(); i++)
    {
        graduation << echelle[i] / 5;
    }
    graduation[4] = 1;
}

int Graphique::getProfondeurForage()
{
    float coteFloat = gestion->getCoteDebut().toFloat();
    int cote = coteFloat * 100;
    emit coteForage(cote);
    return cote;
}


void Graphique::show()
{

    emit menuActif(MENU);

    setEnCoursForage(true);

    ui->lb_TextNomForage->setStyleSheet("color : rgb(96,160,192)");
    ui->lb_TextNomForage->setText(gestion->getForageRef());

    ///Regarde si forage en cours ou non
    if(gestion->getRenseignement().length() < 5)
        continuerForage = false;
    else
        continuerForage = true;

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0,0,160,480);
    QWidget::show();


    if(!continuerForage)
    {
        profondeurForage = gestion->getCoteDebut();
        int prof = getProfondeurForage();//(int)(profondeurForage.toFloat() * 100);
        clearAffichage();
        int dx = 120;
        int dy = 20;
        ///Affichage des fenetres des graphs
        for(int i = 0; i < affichage.length() - 9; i++) //-9 pour l'affichge en cours de forage
        {
            ///Affichage des vue des graphiques au 1er plan
            affichage[i]->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            if(i == 5)
            {
                dx = 120;
                dy = 0;
            }
            if(i == 4)
            {
                dx = 80;
                dy = 0;
            }
            affichage[i]->setOffset(prof);
            affichage[i]->setLargeurGraphique(135);//On ajoute 15 dans affichage
            affichage[i]->setParentPosition(dx, dy);
            affichage[i]->setEchelle(echelle[i]);
            affichage[i]->setGraduation(graduation[i]);
            affichage[i]->setVisualisation(false);
            affichage[i]->setReprise(false);
            affichage[i]->show();
            dx += 150;
        }
    }

    else if(continuerForage)
    {
        QString path(gestion->getPathForageCourant());
        QStringList dataForage(gestion->lectureFichier(path));
	
        ///Vérification de la validité de la fin du forage en cours
        QString donnee = dataForage.last();

        while(donnee.split(";").length() == 1)
        {
            dataForage.removeLast();
            donnee = dataForage.last();
        }
        setData(dataForage.last());

        donnee = dataForage.first();
        QStringList list = donnee.split(";");
        int prof = list.at(0).toInt();

        int dx = 120;
        int dy = 20;
        ///Affichage des fenetres des graphs
        for(int i = 0; i < affichage.length() - 9; i++) //-9 pour l'affichge en cours de forage
        {
            ///Affichage des vue des graphiques au 1er plan
            affichage[i]->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            if(i == 5)
            {
                dx = 120;
                dy = 0;
            }
            if(i == 4)
            {
                dx = 80;
                dy = 0;
            }
            affichage[i]->setOffset(prof);
            affichage[i]->setLargeurGraphique(135);//On ajoute 15 dans affichage
            affichage[i]->setParentPosition(dx, dy);
            affichage[i]->setEchelle(echelle[i]);
            affichage[i]->setGraduation(graduation[i]);
            affichage[i]->setVisualisation(false);
            affichage[i]->setReprise(true);
            affichage[i]->setDataReprise(dataForage);

            dx += 150;
        }

            for(int i = 0; i < affichage.length() - 9; i++)
            {                
                affichage[i]->show();
            }

    }
}

void Graphique::on_bt_pause_clicked()
{

    if(paused)
    {
        setPause(false);
        ui->bt_pause->setText("Pause");
    }
    else
    {
        setPause(true);
        ui->bt_pause->setText("Départ");
    }

    infoUtilisateur();
}

void Graphique::on_bt_BS_clicked()
{

    emit BS();
    ui->lb_profOutil->setText(ui->lb_profForage->text());
}



void Graphique::on_bt_Visu_clicked()
{
    if(ui->bt_Visu->objectName() == "bt_Visu")
    {
        ui->bt_Visu->setVisible(true);
        ui->bt_Menu->setVisible(false);
        ui->bt_Fin->setVisible(false);
        ui->bt_Fin->setEnabled(false);
        ui->bt_Menu->setEnabled(false);

        pageUP->setFont(QFont("Arial",12));
        pageUP->setFlat(false);
        pageUP->setObjectName("UP");
        pageUP->setGeometry(ui->bt_Menu->geometry());
        pageUP->setText("HAUT");
        pageUP->show();

        pageDOWN->setFont(QFont("Arial",12));
        pageDOWN->setFlat(false);
        pageDOWN->setObjectName("DOWN");
        pageDOWN->setGeometry(ui->bt_Fin->geometry());
        pageDOWN->setText("BAS");
        pageDOWN->show();

        int dx = 120;
        int dy = 20;

        for(int i = 9; i < affichage.length(); i++) //
        {
            ///Affichage des vue des graphiques au 1er plan
            affichage[i]->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            if(i == 14)
            {
                dx = 120;
                dy = 0;
            }
            if(i == 13)
            {
                dx = 80;
                dy = 0;
            }
            affichage[i]->setOffset(affichage[0]->getOffset());
            affichage[i]->setLargeurGraphique(135);//on ajoute 15 dans affichage
            affichage[i]->setParentPosition(dx, dy);
            affichage[i]->setEchelle(echelle[i - 9]);
            affichage[i]->setGraduation(graduation[i - 9]);
            affichage[i]->setRatioEchelle(affichage[i-9]->getRatioEchelle());
            affichage[i]->setVisualisation(true);
            affichage[i]->setNumPage(affichage[i-9]->getNumPage());
            if(i < 13)
                affichage[i]->setVisuPolygon(affichage[i-9]->getPolygon());

            connect(pageUP,SIGNAL(clique(QPushButton*)),affichage[i],SLOT(navigationPage(QPushButton*)));
            connect(pageDOWN,SIGNAL(clique(QPushButton*)),affichage[i],SLOT(navigationPage(QPushButton*)));
            affichage[i]->show();
            dx += 150;
        }
        ui->bt_Visu->setObjectName("bt_VisuFin");
        ui->bt_Visu->setText("Visu Fin");
    }

    else if(ui->bt_Visu->objectName() == "bt_VisuFin")
    {
        pageUP->close();
        pageDOWN->close();

        ui->bt_Fin->setVisible(true);
        ui->bt_Menu->setVisible(true);
        ui->bt_Fin->setEnabled(true);
        ui->bt_Menu->setEnabled(true);


        for(int i = 9; i < affichage.length(); i++) //
        {
            ///Arret de la visualisation
            affichage[i]->setVisualisation(false);
            affichage[i]->close();

        }
        ui->bt_Visu->setObjectName("bt_Visu"); 
        ui->bt_Visu->setText("Visu");
    }
}

void Graphique::on_bt_Fin_clicked()
{
    setEnCoursForage(false);

    for(int i = 0; i < affichage.length() - 9; i++)
    {
        affichage[i]->clearAllData();
        affichage[i]->hide();
    }
    if(!getPause())
        ui->bt_pause->click();
    clearAffichage();
    gestion->renseignementBoot();
    gestion->forageTermine();
    emit finForage(); //Ouvre la fenetre de renseignement
    ui->bt_BS->click();
    this->close();
}


void Graphique::on_bt_Menu_clicked()
{
    if(getPause())//Si pause on peut aller dans le menu
    {
        setAffichageVisible(false);
        emit menuShow();
    }
}
