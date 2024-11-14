#ifndef JAMDATABASE_H
#define JAMDATABASE_H

#include <QObject>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QRegularExpression>

class JamDb : public QObject
{
    Q_OBJECT
public:
    explicit JamDb(QObject *parent = nullptr);
    void createTables();
    void dropTables();
    bool executeQuery(const QString &queryStr);
    //QSqlTableModel* getModel() const { return model; }
    // Initialize QSqlTableModel



signals:

public slots:
    void initDatabase();

private:
    QSqlDatabase m_db;
    QSqlTableModel *model;

};

#endif // JAMDATABASE_H
