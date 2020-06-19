#include "myuser.h"


MyUser::MyUser(QString name):
    User(name)
{
    roomid = new int;
}

int MyUser::getRoomid() const
{
    return *roomid;
}

void MyUser::setRoomid(int value)
{
    *roomid = value;
}

bool MyUser::getInroom() const
{
    return inroom;
}

void MyUser::setInroom(bool value)
{
    inroom = value;
}

int MyUser::getLangId() const
{
    return langId;
}

void MyUser::setLangId(int value)
{
    langId = value;
}


