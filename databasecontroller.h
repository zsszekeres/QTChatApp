#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include "usermessage.h"

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DatabaseController
{
public:
    DatabaseController(const QString filename);
    ~DatabaseController();

    void insertMessage(int roomid, QString username, QString content);
    QList<UserMessage> listChatMessages(int roomid) const;
private:
    QSqlDatabase db;
    QSqlQuery insertMessageQuery;

    void initializeDatabase();
};

#endif // DATABASECONTROLLER_H
