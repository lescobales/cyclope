#-------------------------------------------------
#
# Project created by QtCreator 2017-01-04T15:14:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += network widgets

TARGET = Cyclope-v2
TEMPLATE = app


SOURCES += main.cpp\
    mylabel.cpp \
    mymessagebox.cpp \
    mylineedit.cpp \
    communication.cpp \
    mainwidget.cpp \
    renseignement.cpp \
    graphique.cpp \
    gestion.cpp \
    qpushkey.cpp \
    bandeau.cpp \
    ressortie.cpp \
    transfert.cpp \
    affichage.cpp \
    moufflage.cpp \
    parametre.cpp \
    echelles.cpp \
    keyboard.cpp \
    numerickeyboard.cpp

HEADERS  += \
    mylabel.h \
    mymessagebox.h \  
    mylineedit.h \
    communication.h \
    mainwidget.h \
    renseignement.h \
    graphique.h \
    qpushkey.h \
    bandeau.h \
    gestion.h \
    ressortie.h \
    transfert.h \
    affichage.h \
    moufflage.h \
    parametre.h \
    echelles.h \
    keyboard.h \
    numerickeyboard.h


FORMS    += \
    mainwidget.ui \
    renseignement.ui \
    graphique.ui \
    bandeau.ui \
    ressortie.ui \
    transfert.ui \
    affichage.ui \
    moufflage.ui \
    parametre.ui \
    echelles.ui \
    keyboard.ui \
    numerickeyboard.ui

DISTFILES +=

RESOURCES += \
    ressources.qrc

target.path = /home/root
INSTALLS += target

OTHER_FILES +=
