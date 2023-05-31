#include "wifiudp.h"

#include <QByteArray>
#include <QtWidgets>

WifiUDP::WifiUDP(QWidget *parent)
    : QWidget(parent)
{
    udpSocket = new QUdpSocket(this);
}

void WifiUDP::Connect()
{
    address = new QHostAddress(QUdpSocket::ShareAddress);
    udpSocket->bind(adress, 7755);
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(readPendingDatagrams()));
}

void WifiUDP::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        qDebug() << datagram.data() << "oui";
    }
}
