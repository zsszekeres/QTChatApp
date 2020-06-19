#ifndef COMMUNICATIONHANDLER_H
#define COMMUNICATIONHANDLER_H

#include <QObject>
#include "packetcontroller.h"
#include "packetwrappedtcpsocket.h"

class CommunicationHandler : QObject
{
    Q_OBJECT
public:
    CommunicationHandler(QString server_address, int port, PacketController *pcket_ctrl);
    ~CommunicationHandler();
    QString getAddress() const;
    int getPort() const;

    void sendMsg(QJsonDocument json_doc);
private slots:
    void onPacketReady();
private:
    QString address;
    int port;
    PacketWrappedTcpSocket* socket;
    PacketController *packet_ctrl;

    void evaluatePacket(QByteArray data);
};

#endif // COMMUNICATIONHANDLER_H
