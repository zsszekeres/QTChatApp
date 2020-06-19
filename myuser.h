#ifndef MYUSER_H
#define MYUSER_H

#include "user.h"

class MyUser : public User
{
public:
    MyUser(QString name);
    int getRoomid() const;
    void setRoomid(int value);

    bool getInroom() const;
    void setInroom(bool value);

    int getLangId() const;
    void setLangId(int value);

private:
    int* roomid;
    bool inroom = false;
    int langId = 0;
};

#endif // MYUSER_H
