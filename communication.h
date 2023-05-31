#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

#include "affichage.h"
#include "graphique.h"
#include "gestion.h"

class Communication : public QObject
{
    Q_OBJECT
private:

    bool paused, init;
    int valeurCourantePosition;
    int offset;
    int profondeurForage;
    int vitesseActuelle;
    int valPressOut, valPressRot, valPressInj;
    int profondeurOutils;
    QHostAddress listenAddress;
    QHostAddress sendAddress;
    QString data;
    bool newData;
    QString *dataSend;
    QStringList dataList;
    QString chemin;
    Graphique *graphique;
    QList<Affichage*> affichage;
    Gestion* gestion;
    QTimer *timerAffichage;
    QString pathCurrentFile;
    QFile currentFile;

public:
    explicit Communication(QObject *parent = 0);
    Communication(QList<Affichage*> aff, Graphique* g, Gestion* ge);
    QUdpSocket *udpSocket;
    void ecrireForageCourant(QString data);

    void debutMoufflage();
    void finMoufflage();

    void affichageData(QString affichageData);

public slots:
    void ConnectReceiver();
    void getPaused(bool pause);
    void readPendingDatagrams();
    void doBS();
    void getFilePath(QString path);
    void getProfondeurReprise(int prof);
    void finForage();

signals:
    void keyPressed(QString key);

private slots:
    void affichageGraphique();
};

#endif // COMMUNICATION_H
