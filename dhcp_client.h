#ifndef DHCP_CLIENT_H
#define DHCP_CLIENT_H

#include <QObject>
#include <QDebug>
#include <QString>
#include "dhcp_packet.h"
#include <QNetworkInterface>
#include <QHostAddress>
#include <QUdpSocket>

struct ports
{
    quint16 in = 68;
    quint16 out = 67;
};

class dhcp_client : public QObject
{
    Q_OBJECT
public:
    explicit dhcp_client(QObject *parent = 0);

    void send(int);
    void sendRelease(QDataStream&);
    void sendDiscover(QDataStream&);
    void sendRequest(QDataStream&);

    QString getServerIp()const{return this->s_ip;}
    QString getServerNetmask()const{return this->s_nm;}

signals:
    //when message was got
    void update_info();

private slots:
    void read();

private:
    //for network
    QUdpSocket *soc;
    ports port;
    //for packets
    QHostAddress ip;
    QHostAddress netmask;
    QNetworkInterface ithernet;

    //for server
    QString s_ip;
    QString s_nm;
};

#endif // DHCP_CLIENT_H
