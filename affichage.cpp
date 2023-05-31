#include "affichage.h"
#include "ui_affichage.h"

#include <QDebug>

Affichage::Affichage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Affichage)
{
    ui->setupUi(this);

}

Affichage::Affichage(int typGraph) : ui(new Ui::Affichage)
{
      ui->setupUi(this);

      QBrush polygonViaBrush, polygonPOBrush, polygonCRBrush, polygonPIBrush;
      listBrush << polygonViaBrush << polygonPOBrush << polygonCRBrush << polygonPIBrush;

      QPen polygonViaPen, polygonPOPen, polygonCRPen, polygonPIPen, axisVPen, axisHPen;
      listPen << polygonViaPen << polygonPOPen << polygonCRPen << polygonPIPen << axisVPen << axisHPen;

      background = QBrush(QColor(0, 0, 0));

      //initAffichage(typGraph);

      textPen = QPen(Qt::white);
      repPen  = QPen(Qt::gray);
      repPen.setStyle(Qt::DashLine);
      textFont.setPixelSize(50);

      decY = 0;
      offset = 0;
      numPage = 1;
      largeurGraphique = 0;//Largeur des graphiques
      memVia = 0;
      scaleRatio = -1.0;
      echelle = 1;
      choixGraph = 0;

      pListPolygon = &listPolygon;
      pPolyInt = &polyInt;
      pGetListPolygon = &GetListPolygon;

      setVisualisation(false);
      initAffichage(typGraph);

      setReprise(false);
      setRessortie(false);
}


Affichage::~Affichage()
{
    delete ui;
}

void Affichage::paintGraph(QPainter *painterGraph)
{
    if(!getRessortie())
    {
        if(!getVisualisation())
        {
            QRect rect(1,1,this->width()-2,this->height() - 2);
            painterGraph->fillRect(rect, background);

            painterGraph->translate(2, 1);
            painterGraph->setBrush(listBrush[typeGraphe]);
            painterGraph->setPen(listPen[typeGraphe]);

            //si on ne continue pas un forage
            if(!getReprise())
            {
                drawPolygon(painterGraph);
                painterGraph->setBrush(listBrush[typeGraphe]);
                painterGraph->setPen(listPen[typeGraphe]);
            }

            //Si on reprend un forage en cours
            else if(getReprise())
                drawReprisePolygon(painterGraph);
        }

        else if(getVisualisation())
        {
            QRect rect(1,1,this->width()-2,this->height() - 2);
            painterGraph->fillRect(rect, background);

            painterGraph->translate(2, 1);
            painterGraph->setBrush(listBrush[typeGraphe]);
            painterGraph->setPen(listPen[typeGraphe]);
            painterGraph->translate(0, decY);
            verifListPolygon(pGetListPolygon);
            setPolygon(pGetListPolygon);
            for(int i = 0; i < pGetListPolygon->length(); i++)
            {
                painterGraph->drawPolygon(pGetListPolygon->at(i));
            }
            for(float i = 0; i <= 400 - decY; i+=50)
            {
                painterGraph->setPen(repPen);
                painterGraph->drawLine(0, i, largeurGraphique, i);
            }
        }
    }
    else if(getRessortie())
    {
        QRect rect(1,1,this->width()-2,this->height() - 2);
        painterGraph->fillRect(rect, background);


        painterGraph->translate(2, 1);
        painterGraph->setBrush(listBrush[typeGraphe]);
        painterGraph->setPen(listPen[typeGraphe]);
        painterGraph->translate(0, decY);

        painterGraph->drawPolygon(getRessortiePolygon());

        for(float i = 0; i <= 400 - decY; i+=50)
        {
            painterGraph->setPen(repPen);

            painterGraph->drawLine(0, i, largeurGraphique, i);
        }
    }
    painterGraph->setPen(QPen(QBrush(Qt::white), 2.0));
    painterGraph->drawLine(largeurGraphique, -decY, largeurGraphique, -decY + 400);

    painterGraph->setBrush(QBrush(Qt::black));
    QRect rectJointure(largeurGraphique, -decY - 2, largeurGraphique, -decY + 402);
    painterGraph->drawRect(rectJointure);

    if(getRessortie())
    {
        if(typeGraphe == choixGraph)
        {
            painterGraph->setPen(listPen[typeGraphe]);
            painterGraph->setBrush(listBrush[typeGraphe]);
            painterGraph->drawEllipse(QPoint(142,-decY + 5),5,5);
        }
    }
}

void Affichage::paintAxisHoriz(QPainter *painterAxis)
{
    QRect rect(0,0,this->width(),this->height());
    painterAxis->fillRect(rect, background);

    painterAxis->setPen(listPen[typeGraphe]);
    painterAxis->drawLine(0, 15, largeurGraphique, 15);
    for(int i = 0; i <= echelle; i+=graduation)
    {
        painterAxis->drawLine(i*largeurGraphique/echelle, 15,i*largeurGraphique/echelle, 25);
        if(i == 0)
            painterAxis->drawText(i*largeurGraphique/echelle, 10,QString::number(i));
        else if(i < 100)
            painterAxis->drawText(i*largeurGraphique/echelle - 5, 10,QString::number(i));
        else if(i < 1000)
            painterAxis->drawText(i*largeurGraphique/echelle - 10, 10,QString::number(i));
        else if(i < 10000)
            painterAxis->drawText(i*largeurGraphique/echelle - 15, 10,QString::number(i));
    }
}

void Affichage::paintAxisVert(QPainter *painterAxis)
{
    if(!getRessortie())
    {
        if(!getVisualisation())
        {
        QRect rect(0,0,this->width(),this->height());
        painterAxis->fillRect(rect, background);
        data = getData();
        painterAxis->setPen(listPen[typeGraphe]);
        painterAxis->translate(0,20);
        //si on ne continue pas un forage
        if(!getReprise())
            drawVAxe(painterAxis);

        //Si on reprend un forage en cours
        else if(getReprise())
            drawRepriseVAxe(painterAxis);
        }

        else if(getVisualisation())
        {
            painterAxis->translate(0,0);
            QRect rect(0,0,this->width(),this->height());
            painterAxis->fillRect(rect, background);
            painterAxis->setPen(listPen[typeGraphe]);
            painterAxis->translate(0,decY);
            qDebug() << "Translate Y";
            for(float i = 0; i <= 400 - decY; i += 50) //Dessin de l'echelle verticale
            {
                painterAxis->drawLine(35, i+20, 40, i+20);
                painterAxis->drawText(10,i - 5 + 20,QString::number((i)/100));
                painterAxis->setFont(QFont("Arial",10,8));
                painterAxis->drawText(13,i + 10 + 20,"m");
                painterAxis->setFont(QFont("Arial",10,8));
            }
        }
    }
    else if(getRessortie())
    {
        painterAxis->translate(0,0);
        QRect rect(0,0,this->width(),this->height());
        painterAxis->fillRect(rect, background);
        painterAxis->setPen(listPen[typeGraphe]);
        painterAxis->translate(0,decY);
        qDebug() << "Translate Y";
        for(float i = 0; i <= 400 - decY; i += 50) //Dessin de l'echelle verticale
        {
            painterAxis->drawLine(35, i+20, 40, i+20);
            painterAxis->drawText(10,i - 5 + 20,QString::number((i)/100));
            painterAxis->setFont(QFont("Arial",10,8));
            painterAxis->drawText(13,i + 10 + 20,"m");
            painterAxis->setFont(QFont("Arial",10,8));
        }
    }
}

void Affichage::setData(QString donnee)
{
    data = donnee.split(";");
    /*if(typeGraphe == via)
    {
            if(data.at(1).toFloat() < 1500  && data.at(1).toFloat() >= 0)//Donnee valide
                memVia = data.at(1).toFloat();
            else
                data.replace(1, QString::number(memVia));
    }*/
    update();
}

void Affichage::setDataReprise(QStringList dataReprise)
{
    this->dataReprise = dataReprise;
    update();

}

void Affichage::setParentPosition(int x, int y)
{
    if(!getVisualisation())
    {
        if(typeGraphe < 4)
            this->setGeometry(x,y,largeurGraphique + 15,400);
        else if(typeGraphe == hAxe)
            this->setGeometry(x, y, largeurGraphique + 15, 20);
        else if(typeGraphe == vAxe)
            this->setGeometry(x, y, 40, 440);
    }
    else if(getVisualisation())
    {
        if(typeGraphe < 4)
            this->setGeometry(x,y,largeurGraphique + 15,400);
        else if(typeGraphe == hAxe)
            this->setGeometry(x, y, largeurGraphique + 15, 20);
        else if(typeGraphe == vAxe)
            this->setGeometry(x, y, 40, 440);
    }
}

QStringList Affichage::getData()
{
    if(data.isEmpty())
       data <<"0"<<"0"<<"0"<<"0"<<"0";
    return data;
}

void Affichage::initAffichage(int type)
{
    switch(type)
    {
    case via:
          typeGraphe = via;
          listBrush[typeGraphe] = QBrush(Qt::red);
          listPen[typeGraphe] = QPen(Qt::black);
          listPen[typeGraphe].setWidthF(0.1);
          listPen[typeGraphe].setStyle(Qt::SolidLine);
        break;
    case PO:
          typeGraphe = PO;
          listBrush[typeGraphe] = QBrush(Qt::yellow);
          listPen[typeGraphe] = QPen(Qt::black);
          listPen[typeGraphe].setWidthF(0.1);
        break;

    case CR:
          typeGraphe = CR;
          listBrush[typeGraphe] = QBrush(Qt::green);
          listPen[typeGraphe] = QPen(Qt::black);
          listPen[typeGraphe].setWidthF(0.1);
        break;

    case PI:
          typeGraphe = PI;
          listBrush[typeGraphe] = QBrush(Qt::blue);
          listPen[typeGraphe] = QPen(Qt::black);
          listPen[typeGraphe].setWidthF(0.1);
        break;

    case vAxe:
          typeGraphe = vAxe;
          listPen[typeGraphe] = QPen(Qt::white);
          listPen[typeGraphe].setWidthF(0.5);
        break;

    case hAxe:
          typeGraphe = hAxe;
          listPen[typeGraphe] = QPen(Qt::white);
          listPen[typeGraphe].setWidthF(1.0);
        break;

    default:
        for(int i = 0; i < listBrush.length(); i++)
        {
            listBrush[i] = QBrush(Qt::red);
        }
        for(int i = 0; i < listPen.length(); i++)
        {
            listPen[i] = QPen(Qt::black);
            listPen[i].setWidthF(0.1);
        }
      break;
    }
}

void Affichage::setEchelle(int ech)
{
    if(scaleRatio == -1.0)
    {
        scaleRatio = 1;
    }
    else
    {
        scaleRatio = (float)ech / (float)echelle;
    }
    echelle = ech;
    updateEchellePolygon();
    update();
}

int Affichage::getEchelle()
{
    return echelle;
}

void Affichage::setGraduation(int grad)
{
    graduation = grad;
}

int Affichage::getGraduation()
{
    return graduation;
}

void Affichage::setPolygon(QList<QPolygonF> *listPoly)
{
    if(!listPoly->isEmpty() && !polyInt.isEmpty()) //Si plusieurs polygones
    {
        for(int i = 0; i < listPoly->length(); i++)
        {
            QPointF pList = listPoly->at(i).last();
            if(pList.x() != 0)
            {
                QPointF pList1(0,pList.y());
                QPolygonF poly;
                poly = listPoly->at(i);
                poly << pList1;
                listPoly->replace(i,poly);
            }
        }
        listPolygon << polyInt;
        //GetListPolygon = listPolygon;
        pGetListPolygon = listPoly;
    }

    else if(polyInt.isEmpty() && listPoly->isEmpty()) //Tout est vide
    {
        QPolygonF polyNULL;
        QPointF p(0,0);
        polyNULL << p;
        GetListPolygon << polyNULL;

    }
    else if(!polyInt.isEmpty() && listPoly->isEmpty()) //Si 1ere page
    {
        GetListPolygon.clear();
        GetListPolygon << polyInt;
    }
}

QList<QPolygonF> *Affichage::getPolygon()
{


    setPolygon(pListPolygon);

    return pGetListPolygon;
}

void Affichage::setVisuPolygon(QList<QPolygonF> *listPoly)
{
    verifListPolygon(listPoly);

    GetListPolygon = * pGetListPolygon;
    if(scaleRatio != 1 && typeGraphe < 5)
    {
        for(int i = 0; i < GetListPolygon.length(); i++)
        {
            for(int j = 0; j < GetListPolygon.at(i).length(); j++)
            {
                QPointF point;
                point.setX(GetListPolygon.at(i).at(j).x() / scaleRatio);
                point.setY(GetListPolygon.at(i).at(j).y());
                GetListPolygon[i].replace(j, point);
            }

        }
    }
    *pGetListPolygon = GetListPolygon;

}

void Affichage::concatenatePolygon()
{
    QPolygon poly, polygon;

    if(!pListPolygon->isEmpty())
    {
        for(int i = 0; i < pListPolygon->length(); i++)
        {
            for(int j = 0; j < pListPolygon->at(i).length(); j++)
            {
                polygon << poly.point(j);
            }
        }

        QPoint p(0,poly.at(poly.length() - 1).y());
        polygon << p;

    this->polygon = polygon;
    }
}

void Affichage::setVisualisation(bool visu)
{
    if(visu)
    {
        visualisation = visu;
    }
    else if(!visu)
    {
        visualisation = visu;
    }
}

bool Affichage::getVisualisation()
{
    return visualisation;
}

void Affichage::verifListPolygon(QList<QPolygonF> *listPoly)
{
    if(!listPoly->isEmpty())
    {
    for(int i = 0; i < listPoly->length(); i++)
    {
        QPointF pList = listPoly->at(i).last();//Ajout du point (0,data)
        if(pList.x() != 0)
        {
            QPointF pList1(0,pList.y());
            QPolygonF poly;
            poly = listPoly->at(i);
            poly << pList1;
            listPoly->replace(i,poly);
        }

        pList = listPoly->at(i).first();//Ajout du point (0,data)
        if(pList.x() != 0)
        {
            QPointF pList1(0,pList.y());
            QPolygonF poly;
            poly = listPoly->at(i);
            poly.prepend(pList1);
            listPoly->replace(i,poly);
        }
    }
    pGetListPolygon = listPoly;

    }
}

void Affichage::verifPolygon(QPolygonF poly)
{
    QPointF point = poly.last();
    if(point.x() != 0)
    {
        QPointF point1(0,point.y());
        poly.replace(poly.indexOf(poly.last()), point1);
    }

    point = poly.first();
    if(point.x() != 0)
    {
        QPointF point1(0,point.y());
        poly.replace(poly.indexOf(poly.first()), point1);
    }

    ///Mise à l'echelle
    QPointF point3;
    for(int i = 0; i < poly.length(); i++)
    {
        point3.setX(poly.at(i).x()*largeurGraphique/echelle);
        point3.setY(poly.at(i).y());
        poly.replace(poly.indexOf(poly.at(i)), point3);
    }
    setRessortiePolygon(poly);
}

void Affichage::drawReprisePolygon(QPainter *painterGraph)
{
    int nbrePage = 0;
    for(float i = 0; i < dataReprise.length(); i += 400) //Calcul du nombre de pages
    {
        if(i / dataReprise.length() <= 1)
            nbrePage++;
    }
    qDebug() << "dessine reprise";
    int offsetInt = offset;
    QString donnee;
    QStringList data;

    for(int i = 0; i < dataReprise.length(); i++) //Création du polygon à sauvegarder pour la visualisation
    {

        donnee = dataReprise.at(i);
        data = donnee.split(";");
        if(typeGraphe > 0)
        {
            QPointF point(data.at(typeGraphe + 1).toFloat()*largeurGraphique/echelle,data.at(0).toFloat());
            *pPolyInt << point;// << point2;
        }
        else if(typeGraphe == 0)
        {
            QPointF point((data.at(typeGraphe + 1).toFloat() / 10)*largeurGraphique/echelle,data.at(0).toFloat());
            *pPolyInt << point;// << point2;
        }

        if(data.at(0).toFloat() == offset + 400) //Nouvelle page
        {
            numPage++;
            setNumPage(numPage);
            QPointF fermeture(0, data.at(0).toFloat());
            polyInt << fermeture;
            listPolygon << polyInt;
            //setPolygon(pListPolygon);
            pPolyInt->clear();

            offset = data.at(0).toFloat();
        }
    }
    QPointF fermeture(0, data.at(0).toFloat());
    polyInt << fermeture;
    offset = offsetInt;
    //setPolygon(pListPolygon);

   // GetListPolygon = listPolygon; //Polygon pour la reprise

    if(dataReprise.length() == 1 && nbrePage == 1)// Polygon vide (1 point)
    {
        for(int i = 0; i < dataReprise.length(); i++)
        {
            donnee = dataReprise.at(i);
            data = donnee.split(";");

            QPointF point3(data.at(typeGraphe + 1).toFloat()*largeurGraphique/echelle,data.at(0).toFloat() - offset);
            //QPointF point4(0,data.at(0).toFloat() - offset);
            polygon << point3; // << point4;

            painterGraph->drawPolygon(polygon);
            setProfondeurRepriseForage(data.at(0).toFloat());
            dataReprise.clear();

            for(float j = 0; j <= 400; j+=50)
            {
                painterGraph->setPen(repPen);
                painterGraph->drawLine(0, j, largeurGraphique, j);
                painterGraph->setPen(listPen[typeGraphe]);
            }
        }
    }
    else if(nbrePage == 1) //Polygon sur une seule page
    {
        //QPointF initPoint(0,0);
        //polygon << initPoint;
        for(int i = 0; i < dataReprise.length(); i++) //Création du polygon
        {
            donnee = dataReprise.at(i);
            data = donnee.split(";");
            if(typeGraphe > 0)
            {
                QPointF point3(data.at(typeGraphe + 1).toFloat()*largeurGraphique/echelle,data.at(0).toFloat() - offset);
                polygon << point3;// << point4;
            }

            else if(typeGraphe == 0)
            {
                QPointF point3((data.at(typeGraphe + 1).toFloat() /10 )*largeurGraphique/echelle,data.at(0).toFloat() - offset);
                polygon << point3;// << point4;
            }
        }
        QPointF point4(0,data.at(0).toFloat() - offset); //Ajout du point de fermeture
        polygon << point4;

        painterGraph->drawPolygon(polygon);
        //polygon.removeLast();
        setProfondeurRepriseForage(data.at(0).toFloat());
        dataReprise.clear();
        for(float j = 0; j <= 400; j+=50)
        {
            painterGraph->setPen(repPen);
            painterGraph->drawLine(0, j, largeurGraphique, j);
            painterGraph->setPen(listPen[typeGraphe]);
        }
    }

    else if(nbrePage > 1) //Polygon sur plusieurs pages
    {
        QPointF initPoint(0,0);
        polygon << initPoint;

        int offsetReprise = (nbrePage - 1) * 400 + offset;
        for(int i = offsetReprise; i < dataReprise.length() + offset; i++) //Création du polygon de la dernière page
        {
            donnee = dataReprise.at(i - offset);
            data = donnee.split(";");
            if(typeGraphe > 0)
            {
                QPointF point3(data.at(typeGraphe + 1).toFloat()*largeurGraphique/echelle,data.at(0).toFloat() - offsetReprise);
                polygon << point3;// << point4;
            }

            else if(typeGraphe == 0)
            {
                QPointF point3((data.at(typeGraphe + 1).toFloat() / 10)*largeurGraphique/echelle,data.at(0).toFloat() - offsetReprise);
                polygon << point3;// << point4;
            }
        }

        QPointF point4(0,data.at(0).toFloat() - offsetReprise); //Ajout du point de fermeture
        polygon << point4;

        offset = offsetReprise;
        painterGraph->drawPolygon(polygon);
        setProfondeurRepriseForage(data.at(0).toFloat());
        dataReprise.clear();
        for(float j = 0; j <= 400; j+=50)
        {
            painterGraph->setPen(repPen);
            painterGraph->drawLine(0, j, largeurGraphique, j);
            painterGraph->setPen(listPen[typeGraphe]);
        }
    }
    //setPolygon(pListPolygon);
    setReprise(false);
    qDebug() << "fin";
}

void Affichage::drawPolygon(QPainter *painterGraph)
{

if(!polygon.isEmpty())
{
    while(polygon.last().y() <= 0 && polygon.length() > 1)
        polygon.removeLast();
}

    //qDebug() << "Dessine new polygon";
    data = getData();

    QPointF point(data.at(typeGraphe + 1).toFloat()*largeurGraphique/echelle,data.at(0).toFloat());
    QPointF point2(0,data.at(0).toFloat());//point de fermeture
    *pPolyInt << point << point2;

    QPointF point3(data.at(typeGraphe + 1).toFloat()*largeurGraphique/echelle,data.at(0).toFloat() - offset);
    QPointF point4(0,data.at(0).toFloat() - offset);//point de fermeture
    polygon << point3 << point4;

    painterGraph->drawPolygon(polygon);
    polygon.removeLast();
    pPolyInt->removeLast();

    for(float i = 0; i <= 400; i+=50)
    {
        painterGraph->setPen(repPen);

        painterGraph->drawLine(0, i, largeurGraphique, i);


    }
    if(data.at(0).toFloat() >= offset + 400) //Nouvelle page
    {
        numPage++;
        setNumPage(numPage);
        listPolygon << polyInt;
        GetListPolygon = listPolygon;
        polygon.clear();
        pPolyInt->clear();
        QPointF initPoint(0,0);
        polygon << initPoint;

        offset = data.at(0).toFloat();

    }
}

void Affichage::drawVAxe(QPainter *painterAxis)
{
    for(float i = 0; i <= 400; i+=50)
    {
        painterAxis->drawLine(35, i, 40, i);

        if(data.at(0).toFloat() >= offset + 400)//Nouvelle page
        {
            numPage++;
            setNumPage(numPage);
            offset = data.at(0).toFloat();
        }
        //qDebug() << "Echelle verticale";
        painterAxis->drawText(10,i - 5,QString::number((i + offset)/100));
        painterAxis->setFont(QFont("Arial",10,8));
        painterAxis->drawText(13,i + 10,"m");
        painterAxis->setFont(QFont("Arial",10,8));
    }
}

void Affichage::drawRepriseVAxe(QPainter *painterAxis)
{
    int nbrePage = 0;
    for(float i = 0; i < dataReprise.length(); i += 400) //Calcul du nombre de pages
    {
        if(i / dataReprise.length() <= 1)
            nbrePage++;
    }
    setNumPage(nbrePage);

    int offsetReprise = (nbrePage - 1) * 400 + offset;
    offset = offsetReprise;
    for(float j = 0; j <=400; j+=50)
    {
        painterAxis->drawLine(35, j, 40, j);
        //qDebug() << "Echelle verticale Reprise";
        painterAxis->drawText(10,j - 5,QString::number((j + offsetReprise)/100));
        painterAxis->setFont(QFont("Arial",10,8));
        painterAxis->drawText(13,j + 10,"m");
        painterAxis->setFont(QFont("Arial",10,8));
    }

    setReprise(false);
    dataReprise.clear();
}


void Affichage::paintEvent(QPaintEvent *event)
{

    QPainter painterGraph, painterAxisHoriz, painterAxisVert, painterRessortie;

    if(typeGraphe < 4)
    {
        painterGraph.begin(this);
        painterGraph.setRenderHint(QPainter::Antialiasing);
        this->paintGraph(&painterGraph);
        painterGraph.end();
    }
    else if(typeGraphe == 5)
    {
        painterAxisHoriz.begin(this);
        painterAxisHoriz.setRenderHint(QPainter::Antialiasing);
        painterAxisHoriz.setFont(QFont("Arial",8,8));
        this->paintAxisHoriz(&painterAxisHoriz);
        painterAxisHoriz.end();
    }


    else if(typeGraphe == 4)
    {
        painterAxisVert.begin(this);
        painterAxisVert.setRenderHint(QPainter::Antialiasing);
        painterAxisVert.setFont(QFont("Arial",8,8));
        this->paintAxisVert(&painterAxisVert);
        painterAxisVert.end();
    }


    Q_UNUSED( event );//elimine le warning du parametre event
}

void Affichage::updateEchellePolygon()
{
    if(!getRessortie())
    {
        if(!polygon.isEmpty())
        {
            for(int i = 0; i < polygon.length(); i++)
            {
                QPointF point;
                point.setX(polygon.at(i).x() / scaleRatio);
                point.setY(polygon.at(i).y());
                polygon.replace(i, point);
            }
        }
    }
    else
    {
        for(int i = 0; i < polyTravailRessortie.length(); i++)
        {
            QPointF point(polyTravailRessortie.at(i).x() / scaleRatio, polyTravailRessortie.at(i).y());
            polyTravailRessortie.replace(i, point);
        }
    }
}

void Affichage::setProfondeurRepriseForage(int prof)
{
    if(typeGraphe == 0)//envoie une seule fois de la profondeur de reprise quand l'axe vertical est dessinné(typeGraphe == 4)
        emit profondeurReprise(prof);
}

void Affichage::setNumPage(int num)
{
    numPage = num;
}

int Affichage::getNumPage()
{
    return numPage;
}

void Affichage::clearAllData()
{
    pPolyInt->clear();
    polygon.clear();
    data.clear();
    numPage = 1;
    pGetListPolygon->clear();
    pListPolygon->clear();
    decY = 0;
    offset = 0;
}

void Affichage::setReprise(bool rep)
{
    reprise = rep;
}

bool Affichage::getReprise()
{
    return reprise;
}

void Affichage::setRessortie(bool res)
{
    ressortie = res;
}

bool Affichage::getRessortie()
{
    return ressortie;
}

void Affichage::setRessortiePolygon(QPolygonF poly)
{
    polyRessortie = poly;
    polyTravailRessortie = poly;
}

QPolygonF Affichage::getRessortiePolygon()
{

    return polyTravailRessortie;
}

void Affichage::setLargeurGraphique(int largeur)
{
    largeurGraphique = largeur;
}

void Affichage::setRatioEchelle(float ratio)
{
    scaleRatio = ratio;
}

float Affichage::getRatioEchelle()
{
    return scaleRatio;
}

void Affichage::setChoixGraphique(int choixGraph)
{
    this->choixGraph = choixGraph;
    update();
}

int Affichage::getOffset()
{
    return offset;
}

void Affichage::setOffset(int offset)
{
    this->offset = offset;
}


void Affichage::navigationPage(QPushButton *k)
{
    if(k->objectName() == "DOWN")
    {
        if(decY > (numPage - 1) * -400)
        {
        decY -= 100;
        update();
        }
    }
    else if(k->objectName() == "UP")
    {
        if(decY != 0)
        {
            decY += 100;
            update();
        }

    }
}

void Affichage::show()
{
    QWidget::show();
    update();
}


