#include "dhcp_packet.h"

//---------------------------------------------
//-----------------DHCP PACKET-----------------
//---------------------------------------------
dhcp_packet::dhcp_packet(QHostAddress ip, QHostAddress netmask, QNetworkInterface inter)
{
    this->op = 0x01;

    this->htype = 0x01;

    this->hlen = 0x06;

    this->hops = 0x00;

    this->xid[0] = 0x63;
    this->xid[1] = 0x9F;
    this->xid[2] = 0xC0;
    this->xid[3] = 0x4B;

    this->secs[0] = 0x00;
    this->secs[1] =0x00;

    this->flags[0] = 0x00;
    this->flags[1] = 0x00;

    this->ciaddr = ip.toIPv4Address();

    this->yiaddr = 0x00000000;

    this->siaddr = ip.toIPv4Address() & netmask.toIPv4Address();

    for (int i = 0; i < 4; i++)
        this->giaddr[i] = 0x00;


    qDebug() << inter.hardwareAddress();
    bool ok;
    this->chaddr[0] = QString::fromStdString(inter.hardwareAddress().toStdString().substr(0, 2)).toInt(&ok, 16);
    this->chaddr[1] = QString::fromStdString(inter.hardwareAddress().toStdString().substr(3, 2)).toInt(&ok, 16);
    this->chaddr[2] = QString::fromStdString(inter.hardwareAddress().toStdString().substr(6, 2)).toInt(&ok, 16);
    this->chaddr[3] = QString::fromStdString(inter.hardwareAddress().toStdString().substr(9, 2)).toInt(&ok, 16);
    this->chaddr[4] = QString::fromStdString(inter.hardwareAddress().toStdString().substr(12, 2)).toInt(&ok, 16);
    this->chaddr[5] = QString::fromStdString(inter.hardwareAddress().toStdString().substr(15, 2)).toInt(&ok, 16);
    for (int i = 0; i < 5; i++)
        qDebug() << chaddr[i];
    for (int i = 0; i < 10; i++)
        this->chaddr[i+6] = 0x00;

    for (int i = 0; i < 64; i++)
        this->sname[i] = 0x00;

    for (int i = 0; i < 128; i++)
        this->file[i] = 0x00;

    //DHCP protocols
    options.append(0x63);
    options.append(0x82);
    options.append(0x53);
    options.append(0x63);
}

void dhcp_packet::write_message(QDataStream &out)
{
    out << this->op;

    out << this->htype;

    out << this->hlen;

    out << this->hops;

    for (int i = 0; i < 4; i++)
        out << this->xid[i];

    for (int i = 0; i < 2; i++)
        out << this->secs[i];

    for (int i = 0; i < 2; i++)
        out << this->flags[i];

    out << this->ciaddr;

    out << this->yiaddr;

    out << this->siaddr;

    for (int i = 0; i < 4; i++)
        out << this->giaddr[i];

    for (int i = 0; i < 16; i++)
        out << this->chaddr[i];

    for (int i = 0; i < 64; i++)
        out << this->sname[i];

    for (int i = 0; i < 128; i++)
        out << this->file[i];

    for (int i = 0; i < options.size(); i++)
        out << this->options[i];
}

type_message dhcp_packet::get_type_message()const
{
    bool ok;
    int num = QString::number(this->options[10], 16).toInt(&ok, 16);
    if (num == 7)
        return _Release;
    else if (num == 1)
        return _Discover;
    else if (num == 2)
        return _Offer;
    else if (num == 3)
        return _Request;
    else if (num == 5)
        return _ACK;
}

void dhcp_packet::get_message(QDataStream &in, int size)
{
    in >> this->op;
    qDebug() << op;

    in >> this->htype;

    in >> this->hlen;

    in >> this->hops;

    for (int i = 0; i < 4; i++)
    {
        in >> this->xid[i];
    }

    for (int i = 0; i < 2; i++)
    {
        in >> this->secs[i];
    }

    for (int i = 0; i < 2; i++)
    {
        in >> this->flags[i];
    }

    in >> this->ciaddr;

    in >> this->yiaddr;

    in >> this->siaddr;

    for (int i = 0; i < 4; i++)
    {
        in >> this->giaddr[i];
    }

    for (int i = 0; i < 16; i++)
    {
        in >> this->chaddr[i];
    }

    for (int i = 0; i < 64; i++)
    {
        in >> this->sname[i];
    }

    for (int i = 0; i < 128; i++)
    {
        in >> this->file[i];
    }

    size -= 236;

    qDebug() << "DHCP";

    while(size > 0)
    {
        qint8 mess;
        in >> mess;
        this->options.append(mess);
        size--;
        qDebug() << QString::number(mess, 16);
    }
}

void dhcp_packet::create_message(type_message mess)
{
    options.append(53);//option(0x35) - DHCP
    options.append(1);//length(0x01)

    if (mess == type_message(_Release))
    {
        options.append(7);//message (fields)

        /*options.append(54);
        options.append(4);
        options.append(192);
        options.append(168);
        options.append(199);
        options.append(1);*/

        /*options.append(61);
        options.append(7);
        options.append(1);
        options.append(0x1C);
        options.append(0x6F);
        options.append(0x65);
        options.append(0x94);
        options.append(0xC0);
        options.append(0xd5);*/
    }
    else if (mess == type_message(_Discover))
    {
        options.append(1);//dhcp option

        options.append(50);
        options.append(4);
        options.append(192);
        options.append(168);
        options.append(199);
        options.append(7);
    }
    else if (mess == type_message(_Request))
    {
        options.append(0x03);

        options.append(50);
        options.append(4);
        options.append(192);
        options.append(168);
        options.append(199);
        options.append(7);

        options.append(54);
        options.append(4);
        options.append(192);
        options.append(168);
        options.append(199);
        options.append(1);
    }
    options.append(0xFF);
}

QString dhcp_packet::get_server_netmask()const
{
    //    qDebug() << "Get netmask.";
    QString nm = "";
    int byte = 8;
    bool ok;
    for(int i = byte; i < options.size() - 1;)
    {
        //        qDebug() << "DHCP number " + QString::number(i);
        qint8 op, len;
        op = options[i];
        len = options[i+1];

        //        qDebug() << QString("opt: ") + QString::number(op, 16);
        //        qDebug() << QString("len: ") + QString::number(len, 16);

        if (op == (qint8)1)
        {
            //            qDebug() << "YES";
            int n[4];
            for (int j = i + 2; j < i + 6; j++)
            {
                if (options[j] == qint8(-1))
                    n[j - (i + 2)] = 255;
                else if (options[j] == qint8(0))
                    n[j - (i + 2)] = 0;
                else
                    n[j - (i + 2)] = options[j];
            }
            nm.append(QString::number(n[0]) + QString::fromStdString("."));
            nm.append(QString::number(n[1]) + QString::fromStdString("."));
            nm.append(QString::number(n[2]) + QString::fromStdString("."));
            nm.append(QString::number(n[3]));
            return nm;
        }
        else if (op == (qint8)255)
        {
            return nm;
        }
        else
        {
            i += (int)len + 2;
        }
    }
}

QString dhcp_packet::get_server_ip()const
{
    //    qDebug() << "Get netmask.";
    QString nm = "";
    int byte = 8;
    bool ok;
    for(int i = byte; i < options.size() - 1;)
    {
        //        qDebug() << "DHCP number " + QString::number(i);
        qint8 op, len;
        op = options[i];
        len = options[i+1];

        //        qDebug() << QString("opt: ") + QString::number(op, 16);
        //        qDebug() << QString("len: ") + QString::number(len, 16);

        if (op == (qint8)54)
        {
            //            qDebug() << "YES";

            QString s[4];
            for (int j = 0; j < 4; j++)
            {
                if (options[i + 2 + j] < 0)
                    s[j] = QString::fromStdString(QString::number(options[i + 2 + j], 16).toStdString().substr(14, 2));
                else
                    s[j] = QString::number(options[i + 2 + j], 16);
            }

            nm.append(QString::number(s[0].toInt(&ok, 16)) + QString::fromStdString("."));
            nm.append(QString::number(s[1].toInt(&ok, 16)) + QString::fromStdString("."));
            nm.append(QString::number(s[2].toInt(&ok, 16)) + QString::fromStdString("."));
            nm.append(QString::number(s[3].toInt(&ok, 16)));

            return nm;
        }
        else if (op == (qint8)255)
        {
            return nm;
        }
        else
        {
            i += (int)len + 2;
        }
    }
}
