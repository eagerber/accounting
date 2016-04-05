#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlQuery>
#include <QSharedPointer>
#include <QSqlQueryModel>
#include <QSqlRelation>
#include <memory>


class DB
{
public:
    DB();
    ~DB();

    void init(QString dbFileName);
    void create(QString dbFileName);

    void executeSqlQuery(const QStringList &queryString);
    QSharedPointer<QSqlQuery> executeSqlQuery(const QString &queryString);

    QSharedPointer<QSqlQuery> select(const QString &queryString);

    QSqlDatabase& sdb();

private:
    void connect();
    QSharedPointer<QSqlQuery> executeWithoutCommit(const QString &queryString);

    QSqlDatabase _sdb;
    QString _dbFileName;
};

#endif // DATABASE_H
