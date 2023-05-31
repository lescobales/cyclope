#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QThread>
#include <QFontDatabase>

#include "communication.h"
#include "affichage.h"
#include "mainwidget.h"
#include "renseignement.h"
#include "graphique.h"
#include "keyboard.h"
#include "gestion.h"
#include "bandeau.h"
#include "transfert.h"
#include "ressortie.h"
#include "parametre.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ///Création du fichiers parametres.txt si il n'existe pas
    QFile file("parametres.txt");
    if(!file.exists())
    {
        file.open(QIODevice::ReadWrite);
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << "echelle;2000;250;250;50;1;2000;250;250;50";
        file.close();
    }
    file.~QFile();

    ///Création de l'arborescence des chantiers si elle n'existe pas
    if(!QDir("/Geoid/Chantier").exists())
        QDir().mkpath("/Geoid/Chantier");


    ///Installation de la font arial.ttf pour toute l'application
    int id = QFontDatabase::addApplicationFont(":/prefix/arial.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);
    a.setFont(font);

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);

    ///Gestion des dossiers et fichiers
    Gestion gestion;
    Gestion* pGestion;
    pGestion = &gestion;

    ///Fenetre de ressortie
    Ressortie ressortie;
    Ressortie* pRessortie;
    pRessortie = &ressortie;

    ///Fenetres d'affichage des graphiques
    QList<Affichage*> listAff;
    Affichage *affVia, *affPO, *affCR, *affPI, *affVAxe, *affHAxeVia, *affHAxePO, *affHAxeCR, *affHAxePI,
            *affVisuVia, *affVisuPO, *affVisuCR, *affVisuPI, *affVisuVAxe, *affVisuHAxeVia,
            *affVisuHAxePO, *affVisuHAxeCR, *affVisuHAxePI;

    Affichage affichageVia(0), affichagePO(1), affichageCR(2), affichagePI(3), affichageVAxe(4),
            affichageHAxeVia(5), affichageHAxePO(5), affichageHAxeCR(5), affichageHAxePI(5),
            affichageVisuVia(0), affichageVisuPO(1), affichageVisuCR(2), affichageVisuPI(3), affichageVisuVAxe(4),
                        affichageVisuHAxeVia(5), affichageVisuHAxePO(5), affichageVisuHAxeCR(5), affichageVisuHAxePI(5);

    ///Initialisation des pointeurs pour l'affichage du forage
    affVia = &affichageVia;
    affPO = &affichagePO;
    affCR = &affichageCR;
    affPI = &affichagePI;

    affVAxe = &affichageVAxe;

    affHAxeVia = &affichageHAxeVia;
    affHAxePO = &affichageHAxePO;
    affHAxeCR = &affichageHAxeCR;
    affHAxePI = &affichageHAxePI;



    ///Initialisation des pointeurs pour visualisation en cours de forage
    affVisuVia = &affichageVisuVia;
    affVisuPO = &affichageVisuPO;
    affVisuCR = &affichageVisuCR;
    affVisuPI = &affichageVisuPI;

    affVisuVAxe = &affichageVisuVAxe;

    affVisuHAxeVia = &affichageVisuHAxeVia;
    affVisuHAxePO = &affichageVisuHAxePO;
    affVisuHAxeCR = &affichageVisuHAxeCR;
    affVisuHAxePI = &affichageVisuHAxePI;


    listAff << affVia << affPO << affCR << affPI << affVAxe << affHAxeVia << affHAxePO << affHAxeCR << affHAxePI
            << affVisuVia << affVisuPO << affVisuCR << affVisuPI << affVisuVAxe << affVisuHAxeVia << affVisuHAxePO
            << affVisuHAxeCR << affVisuHAxePI;

    ///Fenetre de visualisation des données de forage durant un forage
    Graphique graph(listAff, pGestion);
    Graphique* pGraph;
    pGraph = &graph;

    ///Communication UDP(Thread)
    Communication comUDP(listAff, pGraph, pGestion);
    Communication* pComUDP;
    pComUDP = &comUDP;

    ///Clavier virtuel
    Keyboard keyboard(NULL);
    Keyboard *pKeyboard;
    pKeyboard = &keyboard;

    ///Fenetre de renseignement des chantiers
    Renseignement rens(pGestion, pGraph, listAff, pKeyboard);
    Renseignement* pRens;
    pRens = &rens;

    ///Fenetre de transfert
    Transfert transfert;
    Transfert* pTransfert;
    pTransfert = &transfert;

    ///Parametrage de l'application
    Parametre param;
    Parametre* pParam = &param;

    ///Menu principal
    MainWidget mainWidget(pRens, pGraph, pTransfert, pRessortie, pComUDP, pParam);
    MainWidget* pMainWidget;
    pMainWidget = &mainWidget;
    //mainWidget.setWindowFlags(Qt::FramelessWindowHint);

    ///Déclaration du thread
    QThread comThread;

    ///Bandeau d'information et de gestion des boutons physiques
    Bandeau bandeau(pMainWidget, pRens, pGraph, pKeyboard, pTransfert, pRessortie, pParam);
    Bandeau* pBandeau;
    pBandeau = &bandeau;
    //pBandeau->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);


    ///SIGNAUX et SLOT
    QObject::connect(&comThread,SIGNAL(started()),pComUDP,SLOT(ConnectReceiver()));
    QObject::connect(pGraph,SIGNAL(BS()),pComUDP,SLOT(doBS()));
    QObject::connect(affVia, SIGNAL(profondeurReprise(int)),&comUDP,SLOT(getProfondeurReprise(int)));
    QObject::connect(pGraph,SIGNAL(finForage()),pRens,SLOT(show()));
    QObject::connect(pGraph,SIGNAL(finForage()),pComUDP,SLOT(finForage()));
    QObject::connect(pRens,SIGNAL(menuShow()),pMainWidget,SLOT(show()));
    QObject::connect(pGraph, SIGNAL(menuShow()),pMainWidget,SLOT(show()));
    QObject::connect(pGraph, SIGNAL(coteForage(int)),pComUDP,SLOT(getProfondeurReprise(int)));

    QObject::connect(pMainWidget,SIGNAL(menuActif(int)),pBandeau,SLOT(setMenuActif(int)));
    QObject::connect(pRens,SIGNAL(menuActif(int)),pBandeau,SLOT(setMenuActif(int)));
    QObject::connect(pGraph,SIGNAL(menuActif(int)),pBandeau,SLOT(setMenuActif(int)));
    QObject::connect(pTransfert,SIGNAL(menuActif(int)),pBandeau,SLOT(setMenuActif(int)));
    QObject::connect(pKeyboard,SIGNAL(menuActif(int)),pBandeau,SLOT(setMenuActif(int)));
    QObject::connect(pRessortie,SIGNAL(menuActif(int)),pBandeau,SLOT(setMenuActif(int)));
    QObject::connect(pParam,SIGNAL(menuActif(int)),pBandeau,SLOT(setMenuActif(int)));
    QObject::connect(pComUDP,SIGNAL(keyPressed(QString)),pBandeau,SLOT(getPress(QString)));

    ///Lancement du thread de communication
    comUDP.moveToThread(&comThread);
    comThread.start();

    ///Suivant l'état des chantiers et forages (fini ou non),
    ///on lance une fenetre différente

    if(pGestion->getRenseignement().isEmpty())          //Pas de chantier en cours
        pMainWidget->show();
    else if(pGestion->getRenseignement().length() > 4)  //Chantier et forage en cours
        pGraph->show();
    else                                                //Forage seulement en cours
        pRens->show();

    pBandeau->show();

    return a.exec();
}

/*Gestion des menus pour les boutons physiques
 * FENETRE       numéro
 *
 * mainWidget       0
 * Renseignement    1
 * Graphique        2
 * Transfert        3
 * Keyboard         4
 * Ressortie        5
 * */
