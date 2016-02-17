#include "dhcp_client.h"

dhcp_client::dhcp_client(QObject *parent) : QObject(parent)
{
    this->s_nm = this->s_ip = "not given yet";

    soc = new QUdpSocket;
    soc->bind(port.in);
    connect(soc, SIGNAL(readyRead()), this, SLOT(read()));
    bool ok;
    //send(0);

    QNetworkInterface in;
    for (int i = 0; i < in.allInterfaces().size(); i++)
    {
        QNetworkInterface t = in.allInterfaces()[i];
        if (t.humanReadableName() == QString("Ethernet"))
        {
            ithernet = t;
            for (int i = 0; i < t.addressEntries().size(); i++)
            {
                QNetworkAddressEntry en = t.addressEntries()[i];
                bool tr;
                int count = 0;
                for (int i = 0; i < en.ip().toString().length(); i++)
                    if (en.ip().toString()[i] == '.')
                    {
                        count++;
                    }

                if (count == 3)
                    tr = true;
                else
                    tr = false;

                if (tr)
                {
                    this->ip = en.ip();
                    this->netmask = en.netmask();
                    qDebug() << t.hardwareAddress();
                }
            }
        }
    }
}

void dhcp_client::read()
{
    qDebug() << "read";
    QByteArray datagram;//message input
    QHostAddress *addr = new QHostAddress;//from ip

    //some default options
    datagram.resize(soc->pendingDatagramSize());
    soc->readDatagram(datagram.data(), datagram.size(), addr);

    //in 'in' we will have message in right view
    QDataStream in(&datagram, QIODevice::ReadOnly);

    dhcp_packet pack(this->ip, this->netmask, this->ithernet);

    pack.get_message(in, datagram.size());

    if (pack.get_type_message() == type_message(_ACK))
    {
        this->s_ip = pack.get_server_ip();
        this->s_nm = pack.get_server_netmask();
        emit update_info();
    }

    qDebug() << QString::fromStdString("input ip:") + addr->toString();
}

void dhcp_client::send(int num)
{
    //---!!!it is default settings!!!---
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    data.clear();
    out.device()->reset();

    if (num == 0)
        sendRelease(out);
    else if (num == 1)
        sendDiscover(out);
    else if (num == 2)
        sendRequest(out);

    soc->writeDatagram(data, QHostAddress::Broadcast, port.out);
}

void dhcp_client::sendRelease(QDataStream &out)
{
    dhcp_packet pack(this->ip, this->netmask, this->ithernet);
    pack.create_message(type_message(_Release));
    pack.write_message(out);
}

void dhcp_client::sendDiscover(QDataStream &out)
{
    dhcp_packet pack(this->ip, this->netmask, this->ithernet);
    pack.create_message(type_message(_Discover));
    pack.write_message(out);
}

void dhcp_client::sendRequest(QDataStream &out)
{
    dhcp_packet pack(this->ip, this->netmask, this->ithernet);
    pack.create_message(type_message(_Request));
    pack.write_message(out);
}
