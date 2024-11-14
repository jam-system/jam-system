#include "jamdatabase.h"

JamDb::JamDb(QObject *parent) : QObject(parent)
{
    initDatabase();
}

void JamDb::initDatabase(){
    if (m_db.open()) m_db.close();
    QString dbFile =  "/Users/jameelberg/_S8scannerProject/s8scanner.db";
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbFile);
    if (!m_db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }

    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("scenes"); // Table name
    model->select();
}



bool JamDb::executeQuery(const QString &queryStr)
{
    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qDebug() << "Error executing query: " << query.lastError().text();
        return false;
    }
    return true;
}

void JamDb::dropTables(){
    executeQuery( "drop table reels");
    executeQuery( "drop table scenes");
}

void JamDb::createTables(){
    executeQuery("create table if not exists "
                "reels ("
                "reelId         INTEGER PRIMARY KEY, "
                "reelFileId     INTEGER,"
                "reelDescr      text)"
                );

    executeQuery("create table if not exists "
                 "scenes ("
                 "id            INTEGER PRIMARY KEY,"
                 "sceneDescr    text, "
                 "reelId        int, "
                 "sceneFileId   int,"
                 "startFrame    int, "
                 "endFrame      int, "
                 "gain          int, "
                 "exposure      int, "

                 "FOREIGN KEY(reelId) REFERENCES reels(reelId)"
                 ");");

    //executeQuery("ALTER TABLE reels AUTOINCREMENT=1000");
    //executeQuery("ALTER TABLE scenes AUTOINCREMENT=2000");
}
