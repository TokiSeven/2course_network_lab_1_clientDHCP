#ifndef DHCP_PACKET_H
#define DHCP_PACKET_H

#include <QDataStream>
#include <QList>
#include <QNetworkInterface>

enum type_message{_Release, _Discover, _Offer, _Request, _ACK};

class dhcp_packet
{
public:
    dhcp_packet(QHostAddress, QHostAddress, QNetworkInterface);

    void create_message(type_message);

    type_message get_type_message()const;

    void write_message(QDataStream &);

    void get_message(QDataStream &, int);
    QString get_server_ip()const;
    QString get_server_netmask()const;

private:
    qint8 op;
    qint8 htype;
    qint8 hlen;
    qint8 hops;
    qint8 xid[4];
    qint8 secs[2];
    qint8 flags[2];
    qint32 ciaddr;
    qint32 yiaddr;
    qint32 siaddr;
    qint8 giaddr[4];
    qint8 chaddr[16];
    qint8 sname[64];
    qint8 file[128];
    QList<qint8> options;
};

#endif // DHCP_PACKET_H
