#include "usermessage.h"

UserMessage::UserMessage(QString username, QString content)
{
    this->username = username;
    this->content = content;
}

QString UserMessage::getUsername() const
{
    return username;
}

QString UserMessage::getContent() const
{
    return content;
}
