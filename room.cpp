#include "room.h"


Room::Room(int id, QString name)
{
    this->id = id;
    setName(name);
}

Room::Room(int id, QString name, QList<User> list)
{
    this->id = id;
    setName(name);
    this->userList = list;
}

int Room::getId() const
{
    return id;
}

QString Room::getName() const
{
    return name;
}

void Room::setName(const QString &value)
{
    name = value;
}

QList<User> Room::getUserList() const
{
    return userList;
}

void Room::appendUser(User user)
{
    this->userList.append(user);
}

void Room::takeUserById(int uid)
{
    for (int i=0; i<this->userList.size(); i++) {
        if(this->userList.at(i).getId() == uid) {
            this->userList.takeAt(i);
        }
    }
}

void Room::clearList()
{
    this->userList.clear();
}
