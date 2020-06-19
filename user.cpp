#include "user.h"


User::User(QString name)
{
    setName(name);
}

User::User(int id, QString name)
{
    setId(id);
    setName(name);
}

int User::getId() const
{
    return id;
}

QString User::getName() const
{
    return name;
}

void User::setName(const QString &value)
{
    name = value;
}

void User::setId(int value)
{
    id = value;
}
