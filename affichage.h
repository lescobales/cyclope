#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>


#include "qpushkey.h"

namespace Ui {
class Affichage;
}

class Affichage : public QWidget
{
    Q_OBJECT

public:
    explicit Affichage(QWidget *parent = 0);
    Affichage(int typGraph);
    ~Affichage();

    void paintGraph(QPainter *painterGraph);
    void paintAxisHoriz(QPainter *painterAxis);
    void paintAxisVert(QPainter *painterAxis);

    void setData(QString donnee);
    void setDataReprise(QStringList dataReprise);
    void setParentPosition(int x, int y);

    QStringList getData();

    void initAffichage(int type);

    void setEchelle(int ech);
    int getEchelle();

    void setGraduation(int grad);
    int getGraduation();

    void setPolygon(QList<QPolygonF> *listPoly);
    QList<QPolygonF> *getPolygon();

    void setVisuPolygon(QList<QPolygonF> *listPoly);

    void concatenatePolygon();

    void setVisualisation(bool visu);
    bool getVisualisation();

    void verifListPolygon(QList<QPolygonF>* listPoly);
    void verifPolygon(QPolygonF poly);

    void drawReprisePolygon(QPainter *painterGraph);
    void drawPolygon(QPainter* painterGraph);

    void drawVAxe(QPainter* painterAxis);
    void drawRepriseVAxe(QPainter* painterAxis);

     void setProfondeurRepriseForage(int prof);

     void setNumPage(int num);
     int getNumPage();

     void clearAllData();

     void setReprise(bool rep);
     bool getReprise();

     void setRessortie(bool res);
     bool getRessortie();

     void setRessortiePolygon(QPolygonF poly);
     QPolygonF getRessortiePolygon();

     void setLargeurGraphique(int largeur);

     void setRatioEchelle(float ratio);
     float getRatioEchelle();

     void setChoixGraphique(int choixGraph);

     int getOffset();
     void setOffset(int offset);


    typedef enum
    {
        via = 0,
        PO,
        CR,
        PI,
        vAxe,
        hAxe,
    }Type_Graph;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
      Ui::Affichage *ui;

      QStringList data, dataReprise;

      QBrush background;
      QFont textFont;

      QPen textPen, repPen;
      int decY;
      QList<QPolygonF> listPolygon;
      QList<QPolygonF>* pListPolygon;

      QList<QPolygonF> *pGetListPolygon;
      QList<QPolygonF> GetListPolygon;


      QList<QBrush> listBrush;
      QList<QPen> listPen;

      int graduation;
      int typeGraphe;
      int echelle;
      float scaleRatio;
      int offset;

      int dxH;
      int numPage;

      int choixGraph;
      QPolygonF polygon;

      QPolygonF* pPolyInt;
      QPolygonF polyInt;

      bool visualisation;
      bool reprise;
      bool ressortie;

      QPolygonF polyRessortie;//Polygon initial
      QPolygonF polyTravailRessortie;//polygon de travail

      int memVia;

      int largeurGraphique;

      void updateEchellePolygon();

signals:
    void clique();
    void profondeurReprise(int prof);

public slots:
      //void envoiePolygon();
      void navigationPage(QPushButton* k);
      void show();


};

#endif // AFFICHAGE_H
