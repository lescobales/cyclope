#ifndef WIFIUDP_H
#define WIFIUDP_H

#include <QWidget>

class QUdpSocket;
class QHostAddress;
class QByteArray;

class WifiUDP : public QWidget
{
    Q_OBJECT

public:
    WifiUDP(QWidget *parent = 0);
    void Connect();

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *udpSocket;
    QHostAddress *address;
};

#endif // WIFIUDP_H
