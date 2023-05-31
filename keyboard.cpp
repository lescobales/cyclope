#include "keyboard.h"
#include "ui_keyboard.h"



Keyboard::Keyboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Keyboard)

{
    ui->setupUi(this);

    MENU = 4;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0,0,800,460);

    showKeyboard = false;
}


Keyboard::~Keyboard()
{
    delete ui;
}

void Keyboard::affichage(QStringList list)
{
    lb_visu->setText(list.join(""));
}

QString Keyboard::selectionNomChamp(QString d)
{
    QString data;
    if(d.contains("refChantier"))
        data = "Référence Chantier";
    else if(d.contains("refForage"))
        data = "Référence Forage";
    else if(d.contains("refVille"))
        data = "Ville";
    else if(d.contains("refAdresse"))
        data = "Adresse";
    return data;
}

void Keyboard::setLine(QLineEdit *line)
{
    Line = line;
}

QLineEdit *Keyboard::getLine()
{
    return Line;
}

void Keyboard::getKey(QString key)
{
    if(key == "Ba")
        ui->bt_up->click();
    else if(key == "Bb")
        ui->bt_down->click();
    else if(key == "Bc")
        qDebug() << "Do nothing";
    else if(key == "Bd")
        ui->bt_left->click();
    else if(key == "Be")
        ui->bt_right->click();
    else if(key == "Bf")
        ui->bt_valide->click();
}

void Keyboard::paintRect(QPainter *painterGraph)
{
    painterGraph->setPen(penRect);

    rectKey.setRect(decX +  83, decY + 19,64,64);//decX+82,decY,65,65
    if(decX == 9*64 && decY == 4*64)
        rectKey.setRect(decX +  83, decY + 19,64,128);
    if(decX > 64 && decX < 8*64 && decY == 6*64)
        rectKey.setRect(2*64 +  83, decY + 19,64*6,64);

    painterGraph->drawRect(rectKey);
}

void Keyboard::setShowKeyboard(bool show)
{
    showKeyboard = show;
}

bool Keyboard::getShowKeyboard()
{
    return showKeyboard;
}

void Keyboard::buttonClicked(QPushButton *w)
{
    if(w->text() != "Valide")
        {
        if(w->text() != "Espace" && w->text() != "<-")
            listChar << w->text();
        else if(w->text() == "<-")
        {
            if(!listChar.isEmpty())
                listChar.removeLast();
        }
        else //Touche espace
            listChar << "_";

        affichage(listChar);
        }
    else //Touche valider
        {
        Line->setText(lb_visu->text());
        listChar.clear();
        lb_visu->clear();
        lb_nomChamp->clear();
        listKey.clear();
        setShowKeyboard(false);
        emit menuActif(1); //Réactivation du menu de renseignement
        QWidget::close();
    }
}

void Keyboard::show()
{
    emit menuActif(MENU);
    setShowKeyboard(true);
    decX = decY = 0;

    penRect = QPen(QColor(96,160,192));
    penRect.setWidth(3);

    QStringList positionLettre;
    positionLettre << "A" << "Z" <<"E" << "R" << "T" << "Y" << "U" << "I" << "O" << "P"
                   << "Q" << "S" << "D" << "F" << "G" << "H" << "J" << "K" << "L" << "M"
                   << "W" << "X" << "C" << "V" << "B" << "N";

    ///Alphabet capital
    char* alphabet = new char[26];
    for(int i = 0; i < 26; i++)
    {
        alphabet[i] = (char)(65 + i);
    }

    ///Chiffres
    char* chiffre = new char[10];
    for(int i = 0; i < 10; i++)
    {
        chiffre[i] = (char)(48 + i);
    }

    ///Charactères spéciaux
    char *speciaux = new char[12];
    for(int i = 0; i < 12; i++)
    {
        if(i == 4)
        {
            speciaux[i] = (char)46;
        }
        else
            speciaux[i] = (char)(34 + i);
    }

    ///Initialisation des decalages
    int dx1 = 85;
    int dy1 = 149;
    int dx2 = 85;
    int dx3 = 85;
    int dy2 = dy1 + 64;
    int dy3 = dy2 + 64;

    ///Largeur(w) et hauteur(h) des boutons
    int w = 60;
    int h = 60;

    ///Création des boutons avec lettres CAPITALES
    for(int i = 0; i < 26; i++)
    {
        QPushKey *button;
        button = new QPushKey(this);
        button->setObjectName("bt_" + positionLettre[i]);
        button->setText(positionLettre[i]);
        if(i < 10)
        {
            button->setGeometry(dx1,dy1,w,h);
            dx1 += 64;
        }
        else if(i >= 10 && i < 20)
        {
            button->setGeometry(dx2,dy2,w,h);
            dx2 += 64;
        }
        else if(i >= 20)
        {
            button->setGeometry(dx3,dy3,w,h);
            dx3 += 64;
        }
        listKey << button;
        connect(button,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));
    }

    ///Création des bouttons avec chiffres
    dx1 = 85;
    dy1 = 85;
    for(int i = 0; i < 10; i++)
    {
        QPushKey *button;
        button = new QPushKey(this);
        button->setObjectName("bt_" + (QString)chiffre[i]);
        button->setText((QString)chiffre[i]);
        button->setGeometry(dx1,dy1,w,h);
        dx1 += 64;

        listKey << button;
        connect(button,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));
    }

    ///Création des boutons charactères spéciaux
    dx1 = 85;
    dy1 = 341;
    for(int i = 0; i < 12; i++)
    {
        QPushKey *button;
        button = new QPushKey(this);
        button->setObjectName("bt_" + (QString)speciaux[i]);
        button->setText((QString)speciaux[i]);
        if(i == 0)
            button->setGeometry(597,277,w,h);
        else if(i == 1)
            button->setGeometry(469,277,w,h);
        else if(i == 2)
            button->setGeometry(533,277,w,h);
        /*else if(i == 3)
        {
            //button->setText("/");
            button->setGeometry(597,277,w,h);
        }*/
        else if(i > 2)
        {
            button->setGeometry(dx1,dy1,w,h);
            dx1 += 64;
        }
        listKey << button;
        connect(button,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));
    }

    ///Touche valider
    QPushKey *enterButton;
    enterButton = new QPushKey(this);
    enterButton->setObjectName("bt_Enter");
    enterButton->setText("Valide");
    enterButton->setFont(QFont("Arial,15,63,false"));
    enterButton->setGeometry(661,277,61,123);

    listKey << enterButton;
    connect(enterButton,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));

    ///Touche space
    QPushKey *spaceButton;
    spaceButton = new QPushKey(this);
    spaceButton->setObjectName("bt_Space");
    spaceButton->setText("Espace");
    spaceButton->setGeometry(213,405,380,h);

    listKey << spaceButton;
    connect(spaceButton,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));

    ///Touche delete
    QPushKey *delButton;
    delButton = new QPushKey(this);
    delButton->setObjectName("bt_Del");
    delButton->setText("<-");
    delButton->setGeometry(661,21,w,h);

    listKey << delButton;
    connect(delButton,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));

    ///Touche BIS
    QPushKey *bisButton;
    bisButton = new QPushKey(this);
    bisButton->setObjectName("bt_BIS");
    bisButton->setText("BIS");
    bisButton->setGeometry(85,21,w,h);

    listKey << bisButton;
    connect(bisButton,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));


    ///Touche SP
    QPushKey *spButton;
    spButton = new QPushKey(this);
    spButton->setObjectName("bt_SP");
    spButton->setText("SP");
    spButton->setGeometry(149,21,w,h);

    listKey << spButton;
    connect(spButton,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));

    ///Touche ST
    QPushKey *stButton;
    stButton = new QPushKey(this);
    stButton->setObjectName("bt_ST");
    stButton->setText("ST");
    stButton->setGeometry(213,21,w,h);

    listKey << stButton;
    connect(stButton,SIGNAL(clique(QPushButton*)),this,SLOT(buttonClicked(QPushButton*)));

    lb_visu = new QLabel(this);
    lb_visu->setGeometry(277,40,374,40);
    lb_visu->setAlignment(Qt::AlignCenter);
    lb_visu->setFrameShape(QFrame::Box);
    lb_visu->setStyleSheet("color: rgba(96,160,192)");
    lb_visu->setFont(QFont("Arial",15,63,true));
    lb_visu->setText("");
    lb_visu->show();

    lb_nomChamp = new QLabel(this);
    lb_nomChamp->setGeometry(277,0,374,40);
    lb_nomChamp->setAlignment(Qt::AlignCenter);
    lb_nomChamp->setStyleSheet("color: rgba(96,160,192)");
    lb_nomChamp->setFont(QFont("Arial",20,63,true));
    lb_nomChamp->setText(selectionNomChamp(getLine()->objectName()));

    QWidget::show();
}

void Keyboard::paintEvent(QPaintEvent *event)
{
    QPainter painterGraph;
    painterGraph.begin(this);
    painterGraph.setRenderHint(QPainter::Antialiasing);
    this->paintRect(&painterGraph);
    painterGraph.end();

    Q_UNUSED( event );

}


void Keyboard::on_bt_down_clicked()
{
    decY += 64;


    ///En bas du clavier dans la zone de la touche espace
    if(decY == 7 * 64 && decX <= 5* 64)
    {
        decY = 0;
        decX = 2 * 64;
    }
    if(decY == 7 * 64 && decX > 5*64)
    {
        decY = 0;
        decX = 9 * 64;
    }


    ///zone touche valide
    if(decY == 5 * 64 && decX == 9 * 64)
        decY += 64;

    ///En bas à coté de la touche espace(droite et gauche)
    if(decY == 6 * 64 && decX < 2*64) //A gauche
    {
        decY = 0;
        //decX = 0;
    }
    if(decY == 6 * 64 && decX >=7* 64)//A droite
    {
        decY = 0;
        decX = 9*64;
    }


    update();
}

void Keyboard::on_bt_up_clicked()
{
    decY -= 64;

    ///zone touche valide
    if(decY < 0 && decX == 9 * 64)
        decY = 4 * 64;

    ///En haut dans la zone de la touche espace
    if(decY < 0 && decX >= 128 && decX <=7* 64)
    {
        decY = 6*64;
        //decX = 0;
    }

    ///En haut et dans la zone des labels
    if(decY == 0 * 64 && decX < 6*64)
    {
        decX = 2*64;
        //decX = 0;
    }
    if(decY == 0 * 64 && decX >= 6*64)
    {
        decX = 9*64;
    }


    if(decY < 0)
        decY = 5 * 64;

    update();
}

void Keyboard::on_bt_left_clicked()
{
    decX -= 64;

    if(decX < 0)
        decX = 9 * 64;
    if(decX == 8*64 && decY == 0)
        decX = 2*64;

    ///zone touche valide
    if(decY == 5 * 64 && decX == 9 * 64)
        decY = 4 * 64;

    update();
}

void Keyboard::on_bt_right_clicked()
{
    decX += 64;

    ///zone touche valide
    if(decY == 5 * 64 && decX == 9 * 64)
        decY = 4 * 64;

    if(decX == 640)
        decX = 0;
    if(decX == 3*64 && decY == 0)
        decX = 9*64;
    update();
}

void Keyboard::on_bt_valide_clicked()
{
    for(int i = 0; i < listKey.length(); i++)
    {
        if(rectKey.contains(listKey[i]->x(), listKey[i]->y()))
            listKey[i]->clique(listKey[i]);
    }
}
