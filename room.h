#ifndef ROOM_H
#define ROOM_H

#include "user.h"

#include <qlist.h>




class Room
{
public:
    Room(int id, QString name);
    Room(int id, QString name, QList<User> list);
    int getId() const;
    QString getName() const;
    void setName(const QString &value);
    QList<User> getUserList() const;

    void appendUser(User user);
    void takeUserById(int uid);
    void clearList();

private:
    int id;
    QString name;
    QList<User> userList;
};

#endif // ROOM_H
