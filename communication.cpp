#include "communication.h"
#include <QFile>

Communication::Communication(QObject *parent) : QObject(parent)
{

}

Communication::Communication(QList<Affichage*> aff, Graphique *g, Gestion *ge)
{
    graphique = g;
    affichage = aff;
    gestion = ge;
    paused = true;
    init = false;
    offset = 0;
    profondeurForage = 0;
    valeurCourantePosition = 0;
    vitesseActuelle = 0;
    valPressInj = valPressOut = valPressRot = 0;
    listenAddress.setAddress("192.168.3.11");
    sendAddress.setAddress("192.168.3.14");
    dataSend = &data;
    newData = false;
    timerAffichage = new QTimer(this);

    connect(timerAffichage,SIGNAL(timeout()),this,SLOT(affichageGraphique()));
}
void Communication::ConnectReceiver()
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(listenAddress, 1470);
    udpSocket->open(QIODevice::ReadWrite);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void Communication::affichageData(QString affichageData)
{

    if(!affichageData.isEmpty())
        graphique->setData(affichageData);
}

void Communication::affichageGraphique()
{
    if(newData)
    {
        for(int i = 0; i < affichage.length() - 9; i++)
        {
            QStringList list = data.split(";");
            list.replace(1, QString::number(list[1].toFloat() / 10));
            QString dataAffichage = list.join(";");
            affichage[i]->setData(dataAffichage);
        }
        newData = false;
    }
}

void Communication::getPaused(bool pause)
{
    paused = pause;
}

void Communication::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QString posVit;
        QString pression;
        QString donnees = datagram.data();
        QStringList listeDonnees = donnees.split(";");

        /*-----------------------------traitement codeur et pressions ------------------------------------------*/
        if(listeDonnees.at(0).contains('G') && listeDonnees.at(3).contains('T'))
        {

            valeurCourantePosition = listeDonnees.at(1).toInt();
            vitesseActuelle = listeDonnees.at(2).toInt();
            valPressOut = listeDonnees.at(4).toInt();
            valPressRot = listeDonnees.at(5).toInt();
            valPressInj = listeDonnees.at(6).toInt();

            /** Traitement de la position pendant pause forage*/

            if(!graphique->getPause())
            {
                if (!init)
                {
                    init=true;
                    offset = profondeurForage - valeurCourantePosition + 1;
                    timerAffichage->start(210);// 1000/210 != valeur entière
                    //pathCurrentFile = gestion->getPathForageCourant();
                    //currentFile.setFileName(pathCurrentFile);
                    //currentFile.open(QIODevice::WriteOnly | QIODevice::Append);
                }

                profondeurOutils = valeurCourantePosition + offset;

                listeDonnees.removeFirst();//enleve G;
                if (profondeurForage < profondeurOutils)//nouvelle profondeur positive
                {

                    profondeurForage = profondeurOutils;
                    posVit = QString::number(profondeurForage) + ";" + QString::number(vitesseActuelle) + ";";
                    pression = QString::number(valPressOut) + ";" + QString::number(valPressRot) + ";"
                                + QString::number(valPressInj);
                    data = posVit + pression;
                    ecrireForageCourant(data);
                    data += ";" + QString::number(profondeurOutils); //Ajout de la cote outils
                    graphique->setData(data);
                    newData = true;

                }
                else
                {
                    posVit = QString::number(profondeurForage) + ";" + QString::number(vitesseActuelle) + ";";
                    pression = QString::number(valPressOut) + ";" + QString::number(valPressRot) + ";"
                                + QString::number(valPressInj) + ";" + QString::number(profondeurOutils);
                    data = posVit + pression;
                    graphique->setData(data);
                }

            }
            if (graphique->getPause())
            {
                offset = profondeurForage - valeurCourantePosition;

                /*posVit = QString::number(profondeurForage) + ";" + QString::number((float)vitesseActuelle / 10) + ";";
                pression = QString::number(valPressOut) + ";" + QString::number(valPressRot) + ";"
                            + QString::number(valPressInj) + ";" + QString::number(profondeurOutils);
                data = posVit + pression;

                //dataList.append(data);
                graphique->setData(data);*/
            }
        /*----------------------------------traitement boutons physiques -------------------------*/


        }

        if(listeDonnees.at(0).contains('B'))
        {
            if(listeDonnees.at(1).contains('a')) //listeDonnees.at(1)
                emit keyPressed("Ba");
            else if(listeDonnees.at(1).contains('b'))
                emit keyPressed("Bb");
            else if(listeDonnees.at(1).contains('c'))
                emit keyPressed("Bc");
            else if(listeDonnees.at(1).contains('d'))
                emit keyPressed("Bd");
            else if(listeDonnees.at(1).contains('e'))
                emit keyPressed("Be");
            else if(listeDonnees.at(1).contains('f'))
                emit keyPressed("Bf");
        }

        if(listeDonnees.at(0).contains('T'))
        {
            if(listeDonnees.at(4).toInt() < 360) //Si vitesse < 360 dm/h(vitesse pour 1cm/s)
            {
                QStringList dataAffichage;
                if(data.isEmpty())
                {
                    dataAffichage << QString::number(profondeurForage) << "0" << "0" << "0" << "0" << QString::number(profondeurForage);
                }

                else
                {
                    dataAffichage = data.split(";");
                }

                float vitesse = listeDonnees.at(4).toFloat() / 10;
                dataAffichage.replace(1,QString::number(vitesse));//vitesse
                dataAffichage.replace(2,listeDonnees.at(1));//PO
                dataAffichage.replace(3,listeDonnees.at(2));//CR
                dataAffichage.replace(4,listeDonnees.at(3));//PI
                QString donneesAffichage;
                donneesAffichage = dataAffichage.join(";");
                affichageData(donneesAffichage);
            }
        }

    }
}
void Communication::doBS()
{

    offset = profondeurForage - valeurCourantePosition;

    if(!data.isEmpty())
    {
        QStringList listData;
        listData = data.split(";");
        listData.replace(5, listData.first());
        data = listData.join(";");
    }
}

void Communication::getFilePath(QString path)
{
    chemin = path;
}

void Communication::getProfondeurReprise(int prof)
{
    profondeurForage = prof;
}

void Communication::finForage()
{
    profondeurForage = 0;
}

void Communication::ecrireForageCourant(QString texte)
{
        QString fichier(gestion->getPathForageCourant());
        qDebug() << fichier;
        QFile file(fichier);
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream stream(&file);
        stream.setCodec("UTF-8");

        stream << texte << "\r\n";
        file.close();

}

void Communication::debutMoufflage()
{
    QByteArray datagram = "D";
    udpSocket->writeDatagram(datagram.data(), datagram.size(),sendAddress, 1470);
}

void Communication::finMoufflage()
{
    QByteArray datagram = "F";
    udpSocket->writeDatagram(datagram.data(), datagram.size(),sendAddress, 1470);
}
