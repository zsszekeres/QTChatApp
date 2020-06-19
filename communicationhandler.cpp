#include "communicationhandler.h"

#include <QJsonDocument>
#include <qjsondocument.h>

CommunicationHandler::CommunicationHandler(QString server_address, int port, PacketController *pcket_ctrl)
{
    this->address = server_address;
    this->port = port;
    this->socket = new PacketWrappedTcpSocket();
    this->packet_ctrl = pcket_ctrl;

    this->socket->connectToHost(getAddress(), getPort());
    if(this->socket->waitForConnected()){
        qInfo() << "Successfully connected to " << getAddress() << ":" << getPort();
    } else {
        qCritical() << "Error with connection..";
    }

    connect(this->socket, SIGNAL(packetsReady()), this, SLOT(onPacketReady()));
}

CommunicationHandler::~CommunicationHandler()
{
    if(this->socket->isOpen()) {
        this->socket->close();
    }
}

QString CommunicationHandler::getAddress() const
{
    return address;
}

int CommunicationHandler::getPort() const
{
    return port;
}

void CommunicationHandler::sendMsg(QJsonDocument json_doc)
{
    if(this->socket) {
        if(this->socket->isOpen()) {
            QByteArray data=json_doc.toJson(QJsonDocument::Compact);
            qDebug() << "Sending data : " << data;
            this->socket->write(data);
            this->socket->flush();
        }
    }
}

void CommunicationHandler::onPacketReady()
{
//    qDebug() << this << "Incoming packet..";
    while (this->socket->hasPendingPackets())
    {
        evaluatePacket(this->socket->getNextPacket());
        this->socket->removeNextPacket();
    }
}

void CommunicationHandler::evaluatePacket(QByteArray data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    this->packet_ctrl->parseServerPacket(&doc);
}
