#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSharedPointer>
#include <QSqlQueryModel>
#include <QSqlRelation>
#include <memory>


class DbImplementation;

class DB
{
public:

    static void init(QString dbFileName);
    static void create(QString dbFileName);

    static void executeSqlQuery(const QStringList &queryString);
    static QSharedPointer<QSqlQuery> executeSqlQuery(const QString &queryString);

    static QSharedPointer<QSqlQuery> select(const QString &queryString);

    static QSqlDatabase& sdb();

private:
    DB() = delete;
    ~DB() = delete;

    static DbImplementation* instance();
    static void connect();
    static QSharedPointer<QSqlQuery> executeWithoutCommit(const QString &queryString);
};


#endif // DATABASE_H
