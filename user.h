#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User(QString name);
    User(int id, QString name);
    int getId() const;
    void setId(int value);
    QString getName() const;
    void setName(const QString &value);

private:
    int id;
    QString name;
};

#endif // USER_H
