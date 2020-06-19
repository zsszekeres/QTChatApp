#ifndef PACKETCONTROLLER_H
#define PACKETCONTROLLER_H

#include "room.h"
#include "user.h"
#include "qobject.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>


class PacketController : public QObject
{
    Q_OBJECT
public:
    PacketController();
    ~PacketController();
    int getMyid() const;
    QList<User> getClients() const;
    QList<Room> getRooms() const;
    Room getRoomById(int id)
    {
        foreach (Room r, getRooms()) {
            if (id == r.getId())
                return r;
        }
    }
    int getRoomid() const;
    User *getUser() const;
    QString getArrivedMsg() const;

    void parseServerPacket(QJsonDocument *doc);
    QJsonDocument buildSetUserData(QString username);
    QJsonDocument buildEnterLeaveMsg(bool in_or_out, int roomid);
    QJsonDocument buildNewRoomMsg(QString room_name);
    QJsonDocument buildSendMessage(QString content, int roomid);




signals:
    void myIdArrived();
    void clientsOnline();
    void thereAreRooms();
    void messageArrived();

private:
    //outer use
    int myid;
    QList<User> *clients;
    QList<Room> *rooms;
    User* user;
    int roomid;
    QString arrivedMsg;


    //inner use
    QString strJson;
};

#endif // PACKETCONTROLLER_H
