#include "databasecontroller.h"

DatabaseController::DatabaseController(const QString filename)
{
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)) {
        qDebug() << "DB DRIVER IS AVAILABLE";
        this->db = QSqlDatabase::addDatabase(DRIVER);
        this->db.setDatabaseName(filename);
        qDebug() << "DB added: " << this->db;
        if (db.open()) {
            qDebug() << "DB is open..";
            if (db.tables().size() == 0) {
                qDebug() << "DB is empty, initializing...";
                initializeDatabase();
            }
        } else {
            qDebug() << "DB open error!";
        }

        insertMessageQuery=QSqlQuery(db);
        insertMessageQuery.prepare("INSERT INTO messages (roomid, content, username) VALUES (:roomid, :content, :username)");
    }
    qDebug() << "DB constructor's end..";
}

DatabaseController::~DatabaseController()
{
    this->db.close();
}

void DatabaseController::insertMessage(int roomid, QString content, QString username)
{
    insertMessageQuery.bindValue(":roomid", roomid);
    insertMessageQuery.bindValue(":content", content);
    insertMessageQuery.bindValue(":username", username);
    if (insertMessageQuery.exec()) {
        qDebug() << "INSERT saved: " << insertMessageQuery.lastQuery();
    } else {
        qDebug() << "ERROR insert: " << insertMessageQuery.lastError();
    }
}

QList<UserMessage> DatabaseController::listChatMessages(int roomid) const
{
    QList<UserMessage> tmpList;
    QSqlQuery query(db);

    query.prepare("SELECT * FROM messages WHERE roomid = :roomid");
    query.bindValue(":roomid", roomid);
    query.exec();

    while (query.next()) {
        tmpList.append(UserMessage(query.value("username").toString(), query.value("content").toString()));
//        qDebug() << query.value("id") << " | " << query.value("roomid") << " | " << query.value("content") << " | " << query.value("username");
    }

    return tmpList;
}

void DatabaseController::initializeDatabase()
{
    QSqlQuery query(db);
    for (QString tablename : db.tables()) {
        query.exec("DROP TABLE " + tablename);
    }
    //bool error = false;
    if (!query.exec("CREATE TABLE messages (id INTEGER PRIMARY KEY AUTOINCREMENT, roomid INTEGER, content TEXT, username VARCHAR(90))")) {
        qDebug() << "ERROR messages: " << query.lastError();
    }
}


