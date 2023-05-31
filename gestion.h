#ifndef GESTION_H
#define GESTION_H

#include <QStringList>
#include <QDateTime>

class Gestion
{
public:
    Gestion();
    ~Gestion();
    QStringList getRenseignement();
    void setBootRenseignement(QStringList rens);
    void setRenseignement(QStringList rens);
    void renseignementBoot();
    void findOpen();
    void findClose();
    QStringList lectureFichier(QString chantier);
    QStringList lectureFichier(QString chantier, QString forage);
    void newChantier(QStringList rens);
    void newForage(QStringList rens);
    void ecrireFichier(QString fichier, QStringList donnee);
    void setFileForageCourant(QString file);
    void setDirForageCourant(QString dir);
    QString getPathForageCourant();
    QDateTime getHeure();
    void ecrireForageCourant(QString *texte);
    QString verifDonneeChantier();
    void createChantierForage();
    void forageTermine();
    void chantierTermine();

    void ecrireParametre(QString param, QStringList data, QStringList listParam);
    QStringList lectureParametre(QString parametre);

    QString getForageRef();
    void setForageRef(QString forageRef);
    
    void setCoteDebut(QString cote);
    QString getCoteDebut();
private:
    QStringList listeChantiers;
    QStringList listeOpen;
    QStringList listeForages;
    QStringList listRenseignementIn;

    QString chantier_ref;
    QString chantier_adr;
    QString chantier_vil;
    QString forage_ref;
    QString forage_typ;
    QString forage_out;
    QString dossier;
    QString fichier;
    QString coteDebutForage;

    bool nouveauChantier;
    bool nouveauForage;

    QStringList listForagesClose;
    QStringList listChantiersClose;
    QList<QStringList> listExistChantiersClose;
    QList<QStringList> listExistForagesClose;
};

#endif // GESTION_H
