#include "ressortie.h"
#include "ui_ressortie.h"

#include <QDebug>

Ressortie::Ressortie(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ressortie)

{
    ui->setupUi(this);

    MENU = 5;

    gestion = new Gestion;

    this->rowChantier = this->rowForage = 0;

    visualisation = false;

    chantierSelect = true; //Zone des chantiers selectionnée par défaut

    decX = 119;
    rectWidth = 301;

    choixGraph = 0;

    pageUP = new QPushKey(this);
    pageUP->setVisible(false);

    pageDOWN = new QPushKey(this);
    pageDOWN->setVisible(false);

    zoomPlus = new QPushButton(this);
    zoomPlus->setObjectName("zoomPlus");
    zoomPlus->setGeometry(ui->bt_RIGHT->geometry());
    zoomPlus->setFont(QFont("Arial", 12));
    zoomPlus->setText("ZOOM +");
    zoomPlus->setVisible(false);
    connect(zoomPlus,SIGNAL(clicked()),this,SLOT(onBtZoomPlus()));

    zoomMoins = new QPushButton(this);
    zoomMoins->setVisible(false);
    zoomMoins->setObjectName("zoomMoins");

    selectGraph = new QPushButton(this);
    selectGraph->setVisible(false);
    selectGraph->setObjectName("selectGraph");

}

Ressortie::~Ressortie()
{
    delete ui;
}

void Ressortie::setEchelle(QList<int> ech, int typeGraph)
{
    echelle[typeGraph] =  ech[typeGraph];
}

int Ressortie::getEchelle(int typeGraph)
{
    return echelle[typeGraph];
}

void Ressortie::setGraduation(QList<int> grad, int typeGraph)
{
    graduation[typeGraph] =  grad[typeGraph];
}

int Ressortie::getGraduation(int typeGraph)
{
    return graduation[typeGraph];
}

void Ressortie::affichageGraphique()
{
    updateEchelle();
    updateGraduation();

    //pageUP = new QPushKey(this);
    pageUP->setObjectName("UP");
    pageUP->setFont(QFont("Arial",12));
    pageUP->setFlat(false);
    pageUP->setGeometry(ui->bt_UP->geometry());
    pageUP->setText("HAUT");
    pageUP->show();

    //pageDOWN = new QPushKey(this);
    pageDOWN->setObjectName("DOWN");
    pageDOWN->setFont(QFont("Arial",12));
    pageDOWN->setFlat(false);
    pageDOWN->setGeometry(ui->bt_DOWN->geometry());
    pageDOWN->setText("BAS");
    pageDOWN->show();


    ///Initialisation des pointeurs pour visualisation
    affRessortieVia = new Affichage(0);
    affRessortiePO = new Affichage(1);
    affRessortieCR = new Affichage(2);
    affRessortiePI = new Affichage(3);

    affRessortieVAxe = new Affichage(4);

    affRessortieHAxeVia = new Affichage(5);
    affRessortieHAxePO = new Affichage(5);
    affRessortieHAxeCR = new Affichage(5);
    affRessortieHAxePI = new Affichage(5);

    ///Initialisation de la liste des ressorties
    listAff << affRessortieVia << affRessortiePO << affRessortieCR << affRessortiePI << affRessortieVAxe << affRessortieHAxeVia << affRessortieHAxePO
            << affRessortieHAxeCR << affRessortieHAxePI;

    ///Création du polygon à visualiser
    QStringList dataForage(gestion->lectureFichier(path));
    QPolygonF polyVia, polyPO, polyCR, polyPI;
    QList<QPolygonF> listPoly;
    QList<QPolygonF> *pListPoly;
    pListPoly = &listPoly;
    QStringList listPoint;
    QString donnee;

    for(int i = 0; i < dataForage.length(); i++)
    {
       donnee = dataForage[i];
       listPoint = donnee.split(";");
       QPointF point(listPoint.at(1).toFloat() / 10, listPoint.at(0).toFloat());
       polyVia << point;

       point.setX(listPoint.at(2).toFloat());
       polyPO << point;

       point.setX(listPoint.at(3).toFloat());
       polyCR << point;

       point.setX(listPoint.at(4).toFloat());
       polyPI << point;
    }
    listPoly << polyVia << polyPO << polyCR << polyPI;

    ///Calcul du nombre de pages
    int nbrePage = 0;
    for(float i = 0; i < dataForage.length(); i += 400) //Calcul du nombre de pages
    {
        if(i / dataForage.length() <= 1)
            nbrePage++;
    }

    int dx = 120;
    int dy = 20;

    for(int i = 0; i < listAff.length(); i++) //
    {
        ///Affichage des vue des graphiques au 1er plan
        listAff[i]->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
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
        listAff[i]->setLargeurGraphique(135);
        listAff[i]->setParentPosition(dx, dy);
        listAff[i]->setEchelle(echelle[i]);
        listAff[i]->setGraduation(graduation[i]);
        listAff[i]->setRessortie(true);

        listAff[i]->setNumPage(nbrePage);
        if(i < 4)
        {
            listAff[i]->setRessortiePolygon(pListPoly->at(i));
            listAff[i]->verifPolygon(listPoly.at(i));
        }

        connect(pageUP,SIGNAL(clique(QPushButton*)),listAff[i],SLOT(navigationPage(QPushButton*)));
        connect(pageDOWN,SIGNAL(clique(QPushButton*)),listAff[i],SLOT(navigationPage(QPushButton*)));
        listAff[i]->show();
        dx += 150;
    }

}

void Ressortie::setVisualisation(bool visu)
{
    visualisation = visu;
}

bool Ressortie::getVisualisation()
{
    return visualisation;
}

void Ressortie::infoUtilisateur(bool visu)
{
    if(visu)
    {
        ///Enleve les boutons non utilisés pour l'affichage de la ressortie
        ui->bt_Quitter->setEnabled(false);
        ui->bt_Quitter->setVisible(false);

        ui->bt_LEFT->setEnabled(false);
        ui->bt_LEFT->setVisible(false);

        ui->bt_RIGHT->setEnabled(false);
        ui->bt_RIGHT->setVisible(false);

        ui->bt_UP->setEnabled(false);
        ui->bt_UP->setVisible(false);

        ui->bt_DOWN->setEnabled(false);
        ui->bt_DOWN->setVisible(false);

        ///Cache les labels du bas de page
        for(int i = 0; i < listLabelChantiers.length(); i++)
        {
            if(listLabelChantiers.at(i)->y() >= 400)
                listLabelChantiers.at(i)->setVisible(false);
        }

        for(int i = 0; i < listLabel.length(); i++)
        {
            for(int j = 0; j < listLabel.at(i).length(); j++)
            {
                if(listLabel.at(i).at(j)->y() >= 400)
                    listLabel.at(i).at(j)->setVisible(false);
            }
        }


        ///Affichage des boutons de zoom

        zoomPlus->setVisible(true);


        //zoomMoins = new QPushButton(this);
        zoomMoins->setGeometry(ui->bt_LEFT->geometry());
        zoomMoins->setFont(QFont("Arial", 12));
        zoomMoins->setText("ZOOM -");
        zoomMoins->setVisible(true);
        connect(zoomMoins,SIGNAL(clicked()),this,SLOT(onBtZoomMoins()));

        ///Affichage du bouton de sélection de graphique
        //selectGraph = new QPushButton(this);
        selectGraph->setObjectName("SELECT");
        selectGraph->setGeometry(ui->bt_Quitter->geometry());
        selectGraph->setFont(QFont("Arial", 12));
        selectGraph->setText("Sélection\r\n\nGraphique");
        selectGraph->setVisible(true);
        connect(selectGraph,SIGNAL(clicked()),this,SLOT(onBtSelectGraph()));
    }

    else
    {
        ///rétabli les boutons non utilisés pour l'affichage de la ressortie

        zoomMoins->setVisible(false);
        zoomMoins->setEnabled(false);

        zoomPlus->setVisible(false);
        zoomPlus->setEnabled(false);

        selectGraph->setVisible(false);
        selectGraph->setEnabled(false);

        ui->bt_Quitter->setEnabled(true);
        ui->bt_Quitter->setVisible(true);

        ui->bt_LEFT->setEnabled(true);
        ui->bt_LEFT->setVisible(true);

        ui->bt_RIGHT->setEnabled(true);
        ui->bt_RIGHT->setVisible(true);

        ui->bt_UP->setEnabled(true);
        ui->bt_UP->setVisible(true);

        ui->bt_DOWN->setEnabled(true);
        ui->bt_DOWN->setVisible(true);

        ///Rétabli les labels du bas de page
        for(int i = 0; i < listLabelChantiers.length(); i++)
        {
            if(listLabelChantiers.at(i)->y() >= 400 && listLabelChantiers.at(i)->y() < 471)
                listLabelChantiers.at(i)->setVisible(true);
        }

        for(int i = 0; i < listLabel.length(); i++)
        {
            for(int j = 0; j < listLabel.at(i).length(); j++)
            {
                if(listLabel.at(i).at(j)->y() >= 400 && listLabel.at(i).at(j)->y() < 471)
                    listLabel.at(i).at(j)->setVisible(true);
            }
        }
    }
}

void Ressortie::paintEvent(QPaintEvent *)
{
    QPainter painterRect;
    painterRect.begin(this);
    QRect chantierRect(decX,119,rectWidth,339);
    painterRect.setPen(QColor(255,255,255));
    if(!getVisualisation())
        painterRect.drawRect(chantierRect);
    painterRect.end();
}

void Ressortie::rechercheForages()
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
            //listForages << listeForages;
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

void Ressortie::getKey(QString key)
{
    if(key == "Ba")
        ui->bt_visualiser->click();
    else if(key == "Bc")
        ui->bt_Quitter->click();

    else if(key == "Bd")
    {
        if(getVisualisation())
        {
            pageUP->clique(pageUP);
        }
        else
            ui->bt_UP->click();
    }
    else if(key == "Be")
        ui->bt_RIGHT->click();

    else if(key == "Bf")
    {
        if(visualisation)
        {
            pageDOWN->clique(pageDOWN);
        }
        else
            ui->bt_DOWN->click();
    }
}

void Ressortie::creationLabel()
{
    int dy = 120;
    ///Label des chantiers
    for(int i = 0; i < listChantiers.length(); i++)
    {
        myLabel* lb_Chantier = new myLabel(this);
        lb_Chantier->setObjectName("lb_Chantier"+ QString::number(i));
        lb_Chantier->setGeometry(120,dy,300,20);
        lb_Chantier->setStyleSheet("color : rgb(96,160,192)");
        lb_Chantier->setFont(QFont("Arial", 15, 63, false));
        lb_Chantier->setText(listChantiers.at(i).at(0));
        lb_Chantier->setVisible(true);
        listLabelChantiers << lb_Chantier;
        dy += 35;
        connect(lb_Chantier,SIGNAL(clicked(QLabel*)),this,SLOT(rowChantierChange(QLabel*)));
    }

    ///Label des Forages
    for(int i = 0; i < listForages.length(); i++)
    {
        for(int j = 0; j < listForages.at(i).length(); j++)
        {
        myLabel* lb_Forage = new myLabel(this);
        lb_Forage->setObjectName("lb_Chantier"+ QString::number(i));
        lb_Forage->setStyleSheet("color : rgb(96,160,192)");
        lb_Forage->setFont(QFont("Arial", 15, 63, false));
        lb_Forage->setText(listForages.at(i).at(j));
        lb_Forage->setVisible(false);
        listLabelForages << lb_Forage;
        connect(lb_Forage,SIGNAL(clicked(QLabel*)),this,SLOT(rowForageChange(QLabel*)));
        }
        listLabel << listLabelForages;
        listLabelForages.clear();
    }
}

void Ressortie::show()
{
    emit menuActif(MENU);
    rechercheForages();
    creationLabel();
    QWidget::show();
}

void Ressortie::rowChantierChange(QLabel* lab)
{
    for(int i = 0; i < listLabel.at(rowChantier).length(); i++)
    {
        listLabel.at(rowChantier).at(i)->setVisible(false);
    }

    for(int i = 0; i < listLabelChantiers.length(); i++)
    {
        listLabelChantiers.at(i)->setStyleSheet("background : black; color : rgba(96,160,192)");
    }


   lab->setStyleSheet("background: rgba(96,32,64)");

    for(int i = 0; i < listLabelChantiers.length(); i++)
    {
        if(listLabelChantiers.at(i) == lab)
            rowChantier = i;
    }

    int dy = 120;
    ///Affichage des labels forage
    for(int i = 0; i < listLabel.at(rowChantier).length(); i++)
    {
        listLabel.at(rowChantier).at(i)->setGeometry(425,dy,230,20);
        listLabel.at(rowChantier).at(i)->setVisible(true);
        dy += 35;
    }
}

void Ressortie::rowForageChange(QLabel *lab)
{

    listLabel.at(this->rowChantier).at(this->rowForage)->setStyleSheet("background : black; color : rgba(96,160,192)");

    lab->setStyleSheet("background: rgba(96,32,64)");

    for(int i = 0; i < listLabel.at(rowChantier).length(); i++)
    {
        if(listLabel.at(rowChantier).at(i) == lab)
            rowForage = i;
    }
}

void Ressortie::updateEchelle()
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

void Ressortie::updateGraduation()
{
    graduation.clear();
    for(int i = 0; i < echelle.length(); i++)
    {
        graduation << echelle[i] / 5;
    }
    graduation[4] = 1;
}

void Ressortie::on_bt_visualiser_clicked()
{
    if(ui->bt_visualiser->objectName() == "bt_visualiser")
    {
        setVisualisation(true);
        infoUtilisateur(getVisualisation());
        path = listPathChantiers[rowChantier].at(0) + listPathForages.at(rowChantier).at(rowForage) + ".txt";
        qDebug() << path;
        ui->bt_visualiser->setObjectName("bt_VisuFin");
        //ui->bt_visualiser->setStyleSheet("background : black;qproperty-icon: url(:/prefix/RESSORTIE_FIN.png);");
        ui->bt_visualiser->setText("Visu Fin");
        update();
        affichageGraphique();
    }
    else if(ui->bt_visualiser->objectName() == "bt_VisuFin")
    {

        setVisualisation(false);
        infoUtilisateur(getVisualisation());
        update();
        ui->bt_visualiser->setObjectName("bt_visualiser");
        //ui->bt_visualiser->setStyleSheet("background : black;qproperty-icon: url(:/prefix/VOIR.png);");
        ui->bt_visualiser->setText("Visu");
        for(int i = 0; i < listAff.length(); i++)
        {
            listAff[i]->~Affichage();
        }
        listAff.clear();
        pageDOWN->close();
        pageUP->close();
        zoomMoins->close();
        zoomPlus->close();
        selectGraph->close();

    }
}


void Ressortie::on_bt_Quitter_clicked()
{
    ///rétabli les backgrounds à noir
    for(int i = 0; i < listLabel.length(); i++)
    {
        for(int j = 0; j < listLabel.at(i).length(); j++)
        {
            listLabel.at(i).at(j)->setStyleSheet("background : black");
        }
    }

    for(int i = 0; i < listLabelChantiers.length(); i++)
    {
        listLabelChantiers.at(i)->setStyleSheet("background : black");
    }

    ///Vide les listes
    listChantiers.clear();
    listForages.clear();
    listLabelChantiers.clear();
    listLabelForages.clear();
    listLabel.clear();
    listPathForages.clear();
    listPathChantiers.clear();
    emit menuActif(0);//MainWidget active
    this->close();
}



void Ressortie::on_bt_UP_clicked()
{

    if(chantierSelect)
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
    else
    {
        if(!listLabel.isEmpty())
        {
            if(listLabel.at(rowChantier).last()->y() != 435)
            {
                int x, y, w, h;
                for(int i = 0; i < listLabel.at(rowChantier).length(); i++)
                {

                    x = listLabel.at(rowChantier).at(i)->x();
                    y = listLabel.at(rowChantier).at(i)->y();
                    w = listLabel.at(rowChantier).at(i)->width();
                    h = listLabel.at(rowChantier).at(i)->height();
                    listLabel.at(rowChantier).at(i)->setGeometry(x, y - 35, w, h);
                    if(y < 121 || y > 470)
                        listLabel.at(rowChantier).at(i)->setVisible(false);
                    else
                        listLabel.at(rowChantier).at(i)->setVisible(true);
                }
            }
        }
    }
}

void Ressortie::on_bt_DOWN_clicked()
{
    if(chantierSelect)
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
    else
    {
        if(!listLabel.isEmpty())
        {
            if(listLabel.at(rowChantier).at(0)->y() != 120)
            {
                int x, y, w, h;
                for(int i = 0; i < listLabel.at(rowChantier).length(); i++)
                {

                    x = listLabel.at(rowChantier).at(i)->x();
                    y = listLabel.at(rowChantier).at(i)->y();
                    w = listLabel.at(rowChantier).at(i)->width();
                    h = listLabel.at(rowChantier).at(i)->height();
                    listLabel.at(rowChantier).at(i)->setGeometry(x, y + 35, w, h);
                    if(y < 436 && y > 84)
                        listLabel.at(rowChantier).at(i)->setVisible(true);
                    else
                        listLabel.at(rowChantier).at(i)->setVisible(false);
                }
            }
        }
    }
}

void Ressortie::on_bt_RIGHT_clicked()
{
    if(chantierSelect)
    {
        decX = 424;
        rectWidth = 231;
        chantierSelect = false;
    }
    else
    {
        decX = 119;
        rectWidth = 301;
        chantierSelect = true;
    }

    update();
}

void Ressortie::on_bt_LEFT_clicked()
{
    if(chantierSelect)
    {
        decX = 424;
        rectWidth = 231;
        chantierSelect = false;
    }
    else
    {
        decX = 119;
        rectWidth = 301;
        chantierSelect = true;
    }

    update();
}

void Ressortie::onBtZoomPlus()
{

    int incrementationEchelle = 0;
    int echelleMax;
    switch (choixGraph)
    {
    case 0:
        incrementationEchelle = 100;
        echelleMax = 2000;
        break;
    case 1:
        incrementationEchelle = 50;
        echelleMax = 250;
        break;
    case 2:
        incrementationEchelle = 50;
        echelleMax = 250;
        break;
    case 3:
        incrementationEchelle = 10;
        echelleMax = 50;
        break;
    }

    if(echelle[choixGraph] < echelleMax)
        echelle[choixGraph] = echelle[choixGraph] + incrementationEchelle;

    listAff[choixGraph]->setEchelle(echelle[choixGraph]);
    listAff[choixGraph + 5]->setEchelle(echelle[choixGraph]);

    listAff[choixGraph]->setGraduation(echelle[choixGraph] / 5);
    listAff[choixGraph + 5]->setGraduation(echelle[choixGraph] / 5);
}

void Ressortie::onBtZoomMoins()
{
    int incrementationEchelle = 0;
    int echelleMin;
    switch (choixGraph)
    {
    case 0:
        incrementationEchelle = 100;
        echelleMin = 100;
        break;
    case 1:
        incrementationEchelle = 50;
        echelleMin = 50;
        break;
    case 2:
        incrementationEchelle = 50;
        echelleMin = 50;
        break;
    case 3:
        incrementationEchelle = 10;
        echelleMin = 10;
        break;
    }

    if(echelle[choixGraph] > echelleMin)
        echelle[choixGraph] = echelle[choixGraph] - incrementationEchelle;

    listAff[choixGraph]->setEchelle(echelle[choixGraph]);
    listAff[choixGraph + 5]->setEchelle(echelle[choixGraph]);

    listAff[choixGraph]->setGraduation(echelle[choixGraph] / 5);
    listAff[choixGraph + 5]->setGraduation(echelle[choixGraph] / 5);
}

void Ressortie::onBtSelectGraph()
{
    if(choixGraph < 3)
        choixGraph += 1;
    else
        choixGraph = 0;

    for(int i = 0; i < 4; i++)
    {
        listAff.at(i)->setChoixGraphique(choixGraph);
    }
}
