#include "gestion.h"

#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

Gestion::Gestion()
{
    chantier_vil = " ";
    chantier_adr = " ";
    forage_out = " ";
    renseignementBoot();

    nouveauChantier = nouveauForage = true;
    
    coteDebutForage = "0";
}

Gestion::~Gestion()
{

}

/******************************************************************************/
QStringList Gestion::getRenseignement()
{
    QStringList list;
    QString donnee;
    int length = listeOpen.length();
    for(int i = length; (i > 4); i--)
    {
        donnee = listeOpen.at(i - 1);
        list = donnee.split(";");
        if(list.length() > 1)
        {
        if((list.at(1) == "ED"))
            listeOpen.removeAt(i - 1);
        }
        if(list.at(0) == "")
            listeOpen.removeAt(i - 1);
    }
    return listeOpen;
}

void Gestion::setBootRenseignement(QStringList rens)
{
    listeOpen = rens;
}

void Gestion::setRenseignement(QStringList rens)
{
    listRenseignementIn = rens;
}

void Gestion::renseignementBoot()
{
    listeChantiers = lectureFichier("/Geoid/Chantier/chantier.txt");
    findOpen();

}


/***************************************************************************************************************/
void Gestion::findOpen()
{
    QStringList listeOpen;
    QStringList ligneActuelle;
    QStringListIterator i(listeChantiers);
    while(i.hasNext()) //cherche dans chantier.txt puis dans forage.txt les precedants forages marques par PR
    {
        QString next = i.next();
        if (!next.isEmpty())
        {
                ligneActuelle = next.split(";");
            if (ligneActuelle.last() == "PR")
            {
                chantier_ref = ligneActuelle.at(1);
                chantier_vil = ligneActuelle.at(2);
                chantier_adr = ligneActuelle.at(3);
                dossier = "/Geoid/Chantier/" + ligneActuelle.at(0);
                qDebug() << "Dossier = " << dossier;
                listeOpen << dossier << chantier_ref << chantier_vil << chantier_adr;
                listeForages = lectureFichier(dossier, "forage");
                QStringList ligneActuelle2;
                QStringListIterator i2(listeForages);
                while(i2.hasNext())
                {
                    QString next2 = i2.next();
                    if (!next2.isEmpty())
                    {
                            ligneActuelle2 = next2.split(";");
                        if (ligneActuelle2.last() == "PR")
                        {

                            forage_ref = ligneActuelle2.at(1);
                            //forage_typ = ligneActuelle2.at(2);
                            forage_out = ligneActuelle2.at(3);
                            fichier = ligneActuelle2.at(0);
                            listeOpen << forage_ref + ";PR" << forage_typ << forage_out;
                        }
                        else if(ligneActuelle2.last() == "ED")
                        {
                            listeOpen << ligneActuelle2.at(1) + ";ED";
                            fichier = "";
                        }
                    }
                }
            }
            else
            {
                dossier = "";
                fichier = "";
            }
        }
    }
    setBootRenseignement(listeOpen);
}

/****************************************************************************************************/
void Gestion::findClose()//Recherche les chantiers et forages existants
{
    listeChantiers = lectureFichier("/Geoid/Chantier/chantier.txt");
    QStringList ligneActuelle;
    QStringListIterator i(listeChantiers);
    while(i.hasNext()) //cherche dans chantier.txt puis dans forage.txt les fichiers existants
    {
        QString next = i.next();
        if (!next.isEmpty())
        {
            ligneActuelle = next.split(";");
            if (ligneActuelle.at(2) == "ED")
            {
                listChantiersClose << ligneActuelle.at(1);
                QString path(ligneActuelle.at(0));
                listeForages = lectureFichier(path, "forage");
                QStringList ligneActuelle2;
                QStringListIterator i2(listeForages);
                while(i2.hasNext())
                {
                    QString next2 = i2.next();
                    if (!next2.isEmpty())
                    {
                        ligneActuelle2 = next2.split(";");
                        if (ligneActuelle2.at(2) == "ED")
                        {
                            listForagesClose << ligneActuelle2.at(1);
                        }
                    }
                }
            }
            listExistForagesClose << listForagesClose;
            listForagesClose.clear();
            listExistChantiersClose << listChantiersClose;
            listChantiersClose.clear();

        }
    }
}

/**********************************************************************/
QStringList Gestion::lectureFichier(QString chantier, QString forage)
{
    QStringList data;
    QFile file(chantier.append("/").append(forage).append(".txt"));
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    while(!stream.atEnd())
    {
        data.append(stream.readLine());
    }

    return data;

}

/********************************************************************/
void Gestion::newChantier(QStringList rens)
{
    QStringList rensChantier;
    rensChantier = rens;
    rensChantier.removeAt(1);
    rensChantier.removeAt(3);
    rensChantier.removeAt(3);
    rensChantier << "PR";

    QDateTime heure = getHeure();
    rensChantier.prepend(QString::number(heure.toMSecsSinceEpoch()));//en premier
    ecrireFichier("/Geoid/Chantier/chantier", rensChantier);

    setDirForageCourant("/Geoid/Chantier/" + QString::number(heure.toMSecsSinceEpoch()));
    QDir().mkpath(dossier);
}

/********************************************************************/
void Gestion::newForage(QStringList rens)
{
    QStringList rensForage;
    forage_ref = rens.at(1);
    forage_out = rens.at(4);
    rensForage << forage_ref << forage_out << "PR";
    setForageRef(rensForage.at(0));
    QDateTime heure = getHeure();
    rensForage.prepend(QString::number(heure.toMSecsSinceEpoch()));//en premier
    ecrireFichier(dossier + "/forage", rensForage);
    setFileForageCourant(QString::number(heure.toMSecsSinceEpoch()));
    setCoteDebut(rens.at(5));
    float cote = rens.at(5).toFloat()*100;
    QString debut = QString::number(cote)+";0;0;0;0\r\n";
    ecrireForageCourant(&debut);
}

/********************************************************************/
void Gestion::forageTermine()
{
    QFile file(dossier+"/forage.txt");
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString data;
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            QString line = stream.readLine();

            if(line.split(";").at(1) == forage_ref)
            {
                line.chop(2);
                data.append(line + "ED\n");
            }
            else
            {
                data.append(line + "\n");
            }
        }
        file.resize(0);
        stream << data;
        file.close();
    }
}

/********************************************************************/
void Gestion::chantierTermine()
{
    QFile file("/Geoid/Chantier/chantier.txt");
    if(file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString data;
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            QString line = stream.readLine();

            if(line.split(";").at(1) == chantier_ref)
            {
                line.chop(2);
                data.append(line + "ED\n");
            }
            else
            {
                data.append(line + "\n");
            }
        }
        file.resize(0);
        stream << data;
        file.close();
    }

}

/***************************************************************************************/
void Gestion::ecrireParametre(QString param, QStringList data, QStringList listParam)
{
    QFile file("parametres.txt");
    QString donnee;
    file.open(QIODevice::ReadWrite);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    int i = 0;
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        if(line.split(";").at(0) == param) //Condition de modification du parametre
        {
            donnee = param + ";" + listParam.join(";"); //Modification du parametre
            data.replace(i, donnee); //Modification pour réécriture dans fichier
        }

        i++;

    }
    file.resize(0);

    for(int i = 0; i < data.length(); i++) //Réécriture dans le fichier
    {
        stream << data.at(i) + "\r\n";
    }

    file.close();
}

/********************************************************************/
QStringList Gestion::lectureParametre(QString parametre)
{
    QStringList listParametre = lectureFichier("parametres.txt");
    QString param;
    QStringList paramIntermediaire, returnList;
    for(int i = 0; i < listParametre.length(); i++)
    {
        param = listParametre.at(i);
        paramIntermediaire = param.split(";");
        if(paramIntermediaire.at(i) == parametre)
        {
            paramIntermediaire.removeFirst();
            returnList = paramIntermediaire;
        }
    }
    return returnList;
}

/*********************************************************************/
QString Gestion::getForageRef()
{
    return forage_ref;
}

/********************************************************************/
void Gestion::setForageRef(QString forageRef)
{
    forage_ref = forageRef;
}

/********************************************************************/
void Gestion::setCoteDebut(QString cote)
{
    QStringList listCote;
    if(cote.split(".").length() == 1)
        cote = cote + ".00";

    else if(cote.split(".").length() == 2)
    {
        listCote = cote.split(".");
        if(listCote.at(1).length() == 1)
            listCote[1] = listCote[1] + "0";
        cote = listCote.join(".");
    }

    coteDebutForage = cote;
    qDebug() << coteDebutForage;
}

/*******************************************************************/
QString Gestion::getCoteDebut()
{
    return coteDebutForage;
}

/********************************************************************/
QString Gestion::verifDonneeChantier()
{
    findOpen();
    findClose();
    QString message;

    ///Données des chantiers et forages ouverts
    if(!listeOpen.isEmpty())
    {
        if(listeOpen.isEmpty())
        {
            nouveauChantier = nouveauForage = true;
        }
        else if(listeOpen.at(1) == listRenseignementIn.at(0))//si chantier en cours
        {
            nouveauChantier = false;
            if(listeOpen.length() > 4)
            {
                QStringList donnee;
                for(int i = 2; i < listeOpen.length(); i++)
                {
                    donnee = listeOpen[i].split(";");

                    if(donnee.at(0) == listRenseignementIn.at(1))//meme forage
                    {
                        if(donnee.at(1) == "PR")
                        {
                            //forage_ref = donnee.at(0);
                            forageTermine();
                        }
                        message = "Changer le nom du forage";
                        return message;
                    }
                }
                nouveauForage = true;
            }
        }

        else
        {
            nouveauChantier = true;
            nouveauForage = false;
        }
    }

    if(!listeChantiers.isEmpty())
    {
        QStringList listDonnee;
        for(int i = 0; i < listeChantiers.length(); i++)
        {
            listDonnee = listeChantiers.at(i).split(";");
            if(listDonnee.at(1) == listRenseignementIn.at(0) && listDonnee.last() != "PR")
            {
                nouveauChantier = false;
                nouveauForage = false;
                message = "Changer le nom du Chantier";
                return message;
            }
        }
    }
}

/********************************************************************/
void Gestion::createChantierForage()
{
    if(nouveauChantier && nouveauForage) //Chantier.txt n'existe pas
    {
        newChantier(listRenseignementIn);
        newForage(listRenseignementIn);
    }
    else if(!nouveauChantier && nouveauForage) //Nouveau forage dans chantier en cours
        newForage(listRenseignementIn);

    else if(nouveauChantier && !nouveauForage) //Nouveau chantier et forage et chantier.txt existe
    {
        chantierTermine();
        newChantier(listRenseignementIn);
        newForage(listRenseignementIn);
    }
    listRenseignementIn.clear();
}

/********************************************************************/
QStringList Gestion::lectureFichier(QString readFile)
{
    QFile file(readFile);
    QStringList data;
    bool open = file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    while (!stream.atEnd()){
        data.append(stream.readLine());
    } ;

    return data;

}

/******************************************************************/
void Gestion::ecrireForageCourant(QString* texte)
{
    QString path = getPathForageCourant();
    if(path != "error")
    {
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");

    /*if (*texte == "0;0;0;0;0")
        stream << *texte << "\r\n";*/

    if(*texte == "BS")
        stream << ";BS";
    else
        stream << *texte;
    file.close();
    }
}

/******************************************************************/
void Gestion::ecrireFichier(QString fichier, QStringList donnee)
{
    QFile file(fichier+".txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    for(int i = 0; i < donnee.length(); i++)
    {
        if(i + 1 < donnee.length())
            stream << donnee[i] << ";";
        else if(i + 1 == donnee.length())
            stream << donnee[i];
    }
    stream << "\r\n";
    file.close();
}

/******************************************************************/
void Gestion::setFileForageCourant(QString file)
{
    fichier = file;
}

/******************************************************************/
void Gestion::setDirForageCourant(QString dir)
{
    dossier = dir;
}

/******************************************************************/
QString Gestion::getPathForageCourant()
{
    if(dossier != "" && fichier !="")
    {
        QString path;
        path = dossier + "/" + fichier + ".txt";
        //qDebug() << path;
        return path;
    }
    else
        return "error";
}

QDateTime Gestion::getHeure()
{
    QDateTime heure = QDateTime::currentDateTime();
    return heure;
}
