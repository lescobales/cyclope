#include "numerickeyboard.h"
#include "ui_numerickeyboard.h"

numericKeyboard::numericKeyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::numericKeyboard)
{
    ui->setupUi(this);

    this->setGeometry(0, 0, 800, 460);
}

numericKeyboard::~numericKeyboard()
{
    delete ui;
}

void numericKeyboard::setLine(QLineEdit *line)
{
    Line = line;
}

QLineEdit *numericKeyboard::getLine()
{
    return Line;
}

QString numericKeyboard::selectionNomChamp(QString d)
{
    QString data;
    if(d.contains("refCoteDepart"))
        data = "Côte de départ";

    return data;
}

void numericKeyboard::affichage(QStringList list)
{
    lb_visu->setText(list.join(""));
}

void numericKeyboard::paintRect(QPainter *painterGraph)
{
    painterGraph->setPen(penRect);

    rectKey.setRect(decX +  278, decY + 83,84,84);


    painterGraph->drawRect(rectKey);
}

void numericKeyboard::show()
{
    decX = decY = 0;

    penRect = QPen(QColor(96,160,192));
    penRect.setWidth(3);
    ///Largeur(w) et hauteur(h) des boutons
    int w = 80;
    int h = 80;

    ///Chiffres
    char* chiffre = new char[10];
    for(int i = 0; i < 10; i++)
    {
        chiffre[i] = (char)(48 + i);
    }

    ///Création des bouttons avec chiffres
    int dx1 = 280;
    int dy1 = 85;
    for(int i = 7; i < 10; i++)
    {
        QPushKey *button;
        button = new QPushKey(this);
        button->setObjectName("bt_" + (QString)chiffre[i]);
        button->setText((QString)chiffre[i]);
        button->setGeometry(dx1,dy1,w,h);
        dx1 += w + 4;

        listKey << button;
        connect(button,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));
    }

    dx1 = 280;
    dy1 = 85 + w + 4;
    for(int i = 4; i < 7; i++)
    {
        QPushKey *button;
        button = new QPushKey(this);
        button->setObjectName("bt_" + (QString)chiffre[i]);
        button->setText((QString)chiffre[i]);
        button->setGeometry(dx1,dy1,w,h);
        dx1 += w + 4;

        listKey << button;
        connect(button,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));
    }

    dx1 = 280;
    dy1 = 85 + 2*(w + 4);
    for(int i = 1; i < 4; i++)
    {
        QPushKey *button;
        button = new QPushKey(this);
        button->setObjectName("bt_" + (QString)chiffre[i]);
        button->setText((QString)chiffre[i]);
        button->setGeometry(dx1,dy1,w,h);
        dx1 += w + 4;

        listKey << button;
        connect(button,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));
    }

    ///Boutton du chiffre 0
    QPushKey *buttonZero;
    buttonZero = new QPushKey(this);
    buttonZero->setObjectName("bt_" + (QString)chiffre[0]);
    buttonZero->setText((QString)chiffre[0]);
    buttonZero->setGeometry(280, 3*84+85, w, h);

    listKey << buttonZero;
    connect(buttonZero,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));

    ///Bouton du point
    QPushKey *buttonPoint;
    buttonPoint = new QPushKey(this);
    buttonPoint->setObjectName("bt_Point");
    buttonPoint->setText(".");
    buttonPoint->setGeometry(280 + w + 4, 3*84+85, w, h);

    listKey << buttonPoint;
    connect(buttonPoint,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));

    ///Bouton valide
    QPushKey *buttonValide;
    buttonValide = new QPushKey(this);
    buttonValide->setFont(QFont("Arial",15,63,false));
    buttonValide->setObjectName("bt_Valide");
    buttonValide->setText("Valider");
    buttonValide->setGeometry(280 + 2*(w + 4), 3*84+85, w, h);

    listKey << buttonValide;
    connect(buttonValide,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));

    lb_visu = new QLabel(this);
    lb_visu->setGeometry(280,40,240,40);
    lb_visu->setAlignment(Qt::AlignCenter);
    lb_visu->setFrameShape(QFrame::Box);
    lb_visu->setStyleSheet("color: rgba(96,160,192)");
    lb_visu->setFont(QFont("Arial",15,63,true));
    lb_visu->setText("");
    lb_visu->show();

    lb_nomChamp = new QLabel(this);
    lb_nomChamp->setGeometry(280,0,240,40);
    lb_nomChamp->setAlignment(Qt::AlignCenter);
    lb_nomChamp->setStyleSheet("color: rgba(96,160,192)");
    lb_nomChamp->setFont(QFont("Arial",20,63,true));
    lb_nomChamp->setText(selectionNomChamp(getLine()->objectName()));

    QWidget::show();
}

void numericKeyboard::buttonClicked(QPushButton *w)
{
    if(w->text() != "Valider")
    {
        listChar << w->text();
        affichage(listChar);
    }
    else
    {
        Line->setText(lb_visu->text());
        listChar.clear();
        lb_visu->clear();
        lb_nomChamp->clear();
        listKey.clear();
        QWidget::close();
    }

}

void numericKeyboard::on_bt_UP_clicked()
{
    decY -= 84;
    if(decY < 0)
        decY = 3*84;
    update();
}

void numericKeyboard::on_bt_LEFT_clicked()
{
    decX -= 84;
    if(decX < 0)
        decX = 2*84;
    update();
}

void numericKeyboard::on_bt_DOWN_clicked()
{
    decY += 84;
    if(decY > 3*84)
        decY = 0;
    update();
}

void numericKeyboard::on_bt_RIGHT_clicked()
{
    decX += 84;
    if(decX > 2*84)
        decX = 0;
    update();
}

void numericKeyboard::on_bt_SELECT_clicked()
{
    for(int i = 0; i < listKey.length(); i++)
    {
        if(rectKey.contains(listKey[i]->x(), listKey[i]->y()))
            listKey[i]->clique(listKey[i]);
    }
}

void numericKeyboard::paintEvent(QPaintEvent *event)
{
    QPainter painterGraph;
    painterGraph.begin(this);
    painterGraph.setRenderHint(QPainter::Antialiasing);
    this->paintRect(&painterGraph);
    painterGraph.end();

    Q_UNUSED( event );
}
