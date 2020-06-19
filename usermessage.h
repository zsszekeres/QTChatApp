#ifndef USERMESSAGE_H
#define USERMESSAGE_H

#include <QString>



class UserMessage
{
public:
    UserMessage(QString username, QString content);

    QString getUsername() const;
    QString getContent() const;

private:
    QString username;
    QString content;
};

#endif // USERMESSAGE_H
