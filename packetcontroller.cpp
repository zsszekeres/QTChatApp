#include "packetcontroller.h"
#include <QDebug>

#define T "type"
#define ST "subtype"
#define UN "username"
#define S_TYPE "system"
#define C_TYPE "chat"
#define CINFO_STYPE "clientinfo"
#define AINFO_STYPE "allinfo"
#define CSTATE_STYPE "clientstate"
#define MSG_STYPE "message"
#define CS_STYPE "clientstate"

PacketController::PacketController()
{
    this->clients = new QList<User>();
    this->rooms = new QList<Room>();
    this->user = new User("");
}

PacketController::~PacketController(){}

QJsonDocument PacketController::buildSetUserData(QString username)
{
    QJsonDocument jsondata (QJsonObject{
                                {"type", "system"},
                                {"msgid", 1},
                                {"action", "SetUserData"},
                                {"username", username},
                                {"versiontoken", "i8xral"}
                            });

    return jsondata;
}

QJsonDocument PacketController::buildEnterLeaveMsg(bool in_or_out, int roomid)
{
    QJsonDocument jsondata (QJsonObject{
                                {"type", "system"},
                                {"msgid", 4},
                                {"roomid", roomid},
                                {"action", in_or_out ? "JoinRoom" : "LeaveRoom"}
                            });

    return jsondata;
}

QJsonDocument PacketController::buildNewRoomMsg(QString room_name)
{
    QJsonDocument jsondata (QJsonObject{
                                {"type", "system"},
                                {"msgid", 2},
                                {"roomname", room_name},
                                {"action", "NewRoom"}
                            });

    return jsondata;
}

QJsonDocument PacketController::buildSendMessage(QString content, int roomid)
{
    QJsonDocument jsondata (QJsonObject{
                                {"content", content},
                                {"msgid", 5},
                                {"roomid", roomid},
                                {"type", "chat"}
                            });

    return jsondata;
}

int PacketController::getRoomid() const
{
    return roomid;
}

QString PacketController::getArrivedMsg() const
{
    return arrivedMsg;
}

User *PacketController::getUser() const
{
    return user;
}

void PacketController::parseServerPacket(QJsonDocument *doc)
{
    QJsonObject obj = doc->object();
    QJsonObject tmpobj = obj;


    if (obj.value(T) == S_TYPE) {
//        qDebug() << "Packet type is SYSTEM..";
        if (obj.value(ST) == CINFO_STYPE) {
            //set my id
            myid = obj.value("userid").toInt();
            emit myIdArrived();
//            qDebug() << "myid: " << obj.value("userid").toInt();

        } else if (obj.value(ST) == AINFO_STYPE){
//            qDebug() << "Packet is about ALL_INFO";

            tmpobj.remove("timestamp");
            QJsonDocument tmpdoc(tmpobj);
            QString tmp(tmpdoc.toJson(QJsonDocument::Compact));

            if(strJson == tmp) {
//                qDebug() << "JSONS are the same.. there is nothing to do..";
            } else {
//                qDebug() << "JSONS are NOT the same, save the new one.. ";
//                qDebug() << "JSONS changed from: " << strJson << " to: " << tmp;
                strJson = tmp;

                //get clients
                this->clients->clear();
                QJsonArray arr = obj.value("clients").toArray();
//                qDebug() << "allinfo: " << arr;

                if(!arr.isEmpty()) {
                    foreach (const QJsonValue v, arr) {
                        User u = User(v.toObject().value("userid").toInt(),v.toObject().value("username").toString());
                        this->clients->append(u);
                    }
                    emit clientsOnline();
                }
                /* -------------------------------------------------------------------------------------------------------------------
                    get rooms */
                QJsonArray r_arr = obj.value("rooms").toArray();
                QJsonArray clientsInRoom;
//                qDebug() << "rooms: " << r_arr;

                if(!r_arr.isEmpty()) {
                    this->rooms->clear();
                    foreach (const QJsonValue vr, r_arr) {
                        Room r = Room(vr.toObject().value("roomid").toInt(), vr.toObject().value("roomname").toString());
                        foreach (const QJsonValue vc, vr.toObject().value("clients").toArray()) {
                            User c = User(vc.toObject().value("userid").toInt(), vc.toObject().value("username").toString());
                            r.appendUser(c);
                        }
                        this->rooms->append(r);
//                        qDebug() << "Room object: " << r.getId() << " " + r.getName();
                    }
                    emit thereAreRooms();
                }
            }
        }
    } else if (obj.value(T) == C_TYPE) {
//        qDebug() << "Packet type is CHAT..";
        if (obj.value(ST) == MSG_STYPE) {

//            qDebug() << "Messages subtype came..";
            this->roomid = obj.value("roomid").toInt();
            this->user->setId(obj.value("userid").toInt());
            this->user->setName(obj.value(UN).toString());
            this->arrivedMsg = obj.value("content").toString();
            emit messageArrived();

        } else if (obj.value(ST) == CS_STYPE) {

            qDebug() << "Clientstate subtype came..";
        }
    }
}

QList<User> PacketController::getClients() const
{
    return *clients;
}

QList<Room> PacketController::getRooms() const
{
    return *rooms;
}

int PacketController::getMyid() const
{
    return myid;
}
