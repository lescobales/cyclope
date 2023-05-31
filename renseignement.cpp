#include "renseignement.h"
#include "ui_renseignement.h"

Renseignement::Renseignement(QWidget *parent) :
    QWidget(parent)
{

}

/****************************************************************/
Renseignement::Renseignement(Gestion *g, Graphique *gr, QList<Affichage *> a, Keyboard *k) :
    ui(new Ui::Renseignement)
{
    ui->setupUi(this);

    MENU = 1;

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0,0,800,470);

    gestion = g;
    graphique = gr;
    affichage = a;
    keyboard = k;

    comboSelected = false;
    cbIndex = 0;
    ui->cb_TypOutils->setCurrentIndex(cbIndex);

    ui->lb_Renseignement->setStyleSheet("color: rgba(96,160,192)");
    ui->lb_refChantier->setStyleSheet("color : white");
    ui->lb_refForage->setStyleSheet("color : white");
    ui->lb_refVille->setStyleSheet("color : white");
    ui->lb_refAdresse->setStyleSheet("color : white");
    ui->lb_typOutils->setStyleSheet("color : white");
    ui->lb_refCoteDepart->setStyleSheet("color : white");
    ui->cb_TypOutils->setStyleSheet("background : black; color:white");

    MyLineEdit* typOutils = new MyLineEdit(this);
    typOutils->setGeometry(ui->cb_TypOutils->geometry());
    typOutils->setVisible(false);
    listLineEdit << ui->le_refChantier << ui->le_refForage  << ui->le_refVille << ui->le_refAdresse << typOutils << ui->le_refCoteDepart;
    listLabel << ui->lb_refChantier << ui->lb_refForage << ui->lb_refVille << ui->lb_refAdresse << ui->lb_typOutils << ui->lb_refCoteDepart;

    ui->bt_UP->setVisible(false);

    initPosition();

    setOutRenseignement(gestion->getRenseignement());

    decY =  0;
    penRect = QPen(QColor(255,255,255));
    penRect.setWidth(3);

    lePosX = ui->le_refChantier->x();
    lePosY = ui->le_refChantier->y();
    leWidth = ui->le_refChantier->width();
    leHeight = ui->le_refChantier->height();

    connect(ui->le_refChantier,SIGNAL(clicked(MyLineEdit*)),this,SLOT(setLineEdit(MyLineEdit*)));
    connect(ui->le_refForage,SIGNAL(clicked(MyLineEdit*)),this,SLOT(setLineEdit(MyLineEdit*)));
    connect(ui->le_refVille,SIGNAL(clicked(MyLineEdit*)),this,SLOT(setLineEdit(MyLineEdit*)));
    connect(ui->le_refAdresse,SIGNAL(clicked(MyLineEdit*)),this,SLOT(setLineEdit(MyLineEdit*)));

}

Renseignement::~Renseignement()
{
    delete ui;
}

/****************************************************************/
void Renseignement::setOutRenseignement(QStringList listRens)
{
    listRenseignement = listRens;
    affichageDonnees();
    listRenseignement.clear();
}

/***************************************************************/
void Renseignement::setInRenseignement()
{

    gestion->setRenseignement(listRenseignement);
    listRenseignement.clear();
}

/***************************************************************/
QStringList Renseignement::getRenseignement()
{
    return listRenseignement;
}

/***************************************************************/
void Renseignement::affichageDonnees()
{
    if(getRenseignement().isEmpty())
    {
        ui->le_refChantier->setText("");
        ui->le_refForage->setText("");
    }
    else if(getRenseignement().length() > 4)
    {
        ui->le_refChantier->setText(listRenseignement.at(1));
        ui->le_refVille->setText(listRenseignement.at(2));
        ui->le_refAdresse->setText(listRenseignement.at(3));
        QString donnee = listRenseignement.at(4);
        QStringList listDonnee = donnee.split(";");
        ui->le_refForage->setText(listDonnee.at(0));//listRenseignement.at(4)
    }
    else
    {
        ui->le_refChantier->setText(listRenseignement.at(1));
        ui->le_refVille->setText(listRenseignement.at(2));
        ui->le_refAdresse->setText(listRenseignement.at(3));
        ui->le_refForage->setText("");
    }
}

void Renseignement::paintRect(QPainter *painterGraph)
{
    painterGraph->setPen(penRect);

    lePosX = listLineEdit[decY]->x();
    lePosY = listLineEdit[decY]->y();
    leWidth = listLineEdit[decY]->width();
    leHeight = listLineEdit[decY]->height();

    rectLine.setRect(lePosX - 3, lePosY - 3,leWidth + 6,leHeight + 6);


    painterGraph->drawRect(rectLine);
}

void Renseignement::getKey(QString key)
{
    if(key == "Ba")
        ui->bt_Menu->click();
    else if(key == "Bc")
        ui->bt_valider->click();
    else if(key == "Bd")
        ui->bt_UP->click();
    else if(key == "Be")
        ui->bt_DOWN->click();
    else if(key == "Bf")
        ui->bt_Selection->click();
}

/************************************************************************/
void Renseignement::initPosition()
{
    int dec = 0;
    for(int i = 0; i < listLineEdit.length(); i++)
    {
        listLineEdit.at(i)->setGeometry(370, 130 + dec, 231, 40);
        listLabel.at(i)->setGeometry(150, 130 + dec, 201, 30);
        dec += 60;
    }

    for(int i = 0; i < listLineEdit.length(); i++)
    {
        if(listLineEdit.at(i)->y() >= 430 || listLineEdit.at(i)->y() < 130)
        {
            listLineEdit.at(i)->setVisible(false);
            listLabel.at(i)->setVisible(false);
        }
        else if(listLineEdit.at(i)->y() < 430 && listLineEdit.at(i)->y() >= 130)
        {
            if(i != 4)
                listLineEdit.at(i)->setVisible(true);
            listLabel.at(i)->setVisible(true);
        }
    }
    ui->cb_TypOutils->setGeometry(listLineEdit.at(4)->geometry());
}

/*****************************************************************************/
void Renseignement::setLineEdit(MyLineEdit *line)
{
    listLine.prepend(line);
    ui->lb_Info->setText("");
    keyboard->setLine(line);
    keyboard->show();


}

/*****************************************************************/

void Renseignement::on_bt_valider_clicked()
{
    ///Vérification qu'il existe une référence chantier et forage
    if(ui->le_refForage->text().isEmpty() && ui->le_refChantier->text().isEmpty())
    {
        ui->lb_Info->setText("Forage et Chantier Vides");
        return;
    }
    else if(ui->le_refChantier->text().isEmpty())
    {
        ui->lb_Info->setText("Chantier Vide");
        return;
    }
    else if(ui->le_refForage->text().isEmpty())
    {
        ui->lb_Info->setText("Forage Vide");
        return;
    }

    ///Insertion des renseignements dans la liste et dans le bon ordre
    listLineEdit[4]->setText(ui->cb_TypOutils->currentText());
    for(int i = 0; i < listLineEdit.length(); i++)
    {
        listRenseignement << listLineEdit[i]->text();
    }

    ///Enregistrement des donnees de chantiers
    this->setInRenseignement();


    ///Vérification des donnees de chantiers
    QString message(gestion->verifDonneeChantier());
    if(!message.isEmpty())
    {
        ui->lb_Info->setText(message);
    }

    ///Création des donnees de chantiers
    else
    {
        gestion->createChantierForage();

        this->close();
        graphique->updateEchelle();
        graphique->show();
    }
}

void Renseignement::show()
{
    emit menuActif(MENU);
    ui->lb_Info->setText("");
    ui->le_refCoteDepart->setText("0");
    QWidget::show();
}

void Renseignement::paintEvent(QPaintEvent *event)
{
    QPainter painterGraph;
    painterGraph.begin(this);
    painterGraph.setRenderHint(QPainter::Antialiasing);
    this->paintRect(&painterGraph);
    painterGraph.end();

    Q_UNUSED (event );
}

void Renseignement::on_bt_Menu_clicked()
{
    emit menuShow();
    this->close();
}

void Renseignement::on_bt_UP_clicked()
{
    if(comboSelected)
    {
        if(cbIndex > 0)
            cbIndex --;
        ui->cb_TypOutils->setCurrentIndex(cbIndex);
    }
    /*decY--;
    if(decY < 0)
        decY = listLineEdit.length() - 1;

    update();*/
}

void Renseignement::on_bt_DOWN_clicked()
{
    if(!comboSelected)
    {
        decY++;
        if(decY > 4) //4 éléments visibles sur la page
        {
            int x, y, w, h;
            for(int i = 0; i < listLineEdit.length(); i++)
            {
                x = listLineEdit.at(i)->x();
                y = listLineEdit.at(i)->y();
                w = listLineEdit.at(i)->width();
                h = listLineEdit.at(i)->height();
                listLineEdit.at(i)->setGeometry(x, y - h - 20, w , h);// 20-> espace inter éléments
                if(y - h < 130) //Condition de non visibilité des éléments
                {
                    listLineEdit.at(i)->setVisible(false);
                    listLabel.at(i)->setVisible(false);
                }

                x = listLabel.at(i)->x();
                w = listLabel.at(i)->width();
                h = listLabel.at(i)->height();
                listLabel.at(i)->setGeometry(x, y - h - 20, w, h);
                if(y - h >= 130 && y - h < 430)
                {
                    if(i != 4)
                        listLineEdit.at(i)->setVisible(true);
                    listLabel.at(i)->setVisible(true);
                }
                ui->cb_TypOutils->setGeometry(listLineEdit.at(4)->geometry());
            }
        }
        if(decY >= listLineEdit.length())
        {
            decY = 0;
            initPosition();
        }

        update();
    }

    else
    {
        if(cbIndex < ui->cb_TypOutils->count() - 1)
            cbIndex ++;
        else
            cbIndex = 0;
        ui->cb_TypOutils->setCurrentIndex(cbIndex);
    }
}

void Renseignement::on_bt_Selection_clicked()
{
    for(int i = 0; i < listLineEdit.length(); i++)
    {
        if(rectLine.contains(listLineEdit[i]->x(), listLineEdit[i]->y()))
        {
            if(i != 4)
            {
                if(i != 5)
                {
                    keyboard->setLine(listLineEdit[i]);
                    keyboard->setShowKeyboard(true);
                    keyboard->show();
                }
                else if (i == 5)
                {
                    numKeyBoard = new numericKeyboard;
                    numKeyBoard->setWindowFlags(Qt::FramelessWindowHint);
                    numKeyBoard->setLine(listLineEdit.at(5));
                    numKeyBoard->show();
                }
            }
            else if (i == 4)
            {
                if(!comboSelected)
                {
                    penRect.setColor(QColor(96,160,192));
                    ui->bt_UP->setStyleSheet("background: black; color : rgb(96,160,192)");
                    ui->bt_UP->setVisible(true);
                    ui->bt_DOWN->setStyleSheet("background: black; color : rgb(96,160,192)");
                    ui->bt_Selection->setStyleSheet("background: black; color : rgb(96,160,192)");
                    ui->bt_DOWN->setText("BAS");
                    update();
                    comboSelected = true;
                }
                else
                {
                    penRect.setColor(QColor(255,255,255));
                    ui->bt_UP->setStyleSheet("background: black; color : rgb(255,255,255)");
                    ui->bt_UP->setVisible(false);
                    ui->bt_DOWN->setStyleSheet("background: black; color : rgb(255,255,255)");
                    ui->bt_DOWN->setText("Navigation");
                    ui->bt_Selection->setStyleSheet("background: black; color : rgb(255,255,255)");
                    update();
                    comboSelected = false;
                }
            }
        }
    }

}
