#include "db.h"

#include <QDebug>

#include "db-constdef.h"


DB::DB()
{
}

DB::~DB()
{
    _sdb.close();
}

void DB::init(QString dbFileName)
{
    if(dbFileName.isEmpty())
        return;
    _dbFileName = dbFileName;

    connect();
}

void DB::create(QString dbFileName)
{
    _sdb = QSqlDatabase::addDatabase("QSQLITE");
    _sdb.setDatabaseName(dbFileName);

    executeSqlQuery(Queries::createDB);
}

void DB::executeSqlQuery(const QStringList &queryString)
{
    for(const auto &item : queryString)
    {
        executeSqlQuery(item);
    }
}

QSharedPointer<QSqlQuery> DB::executeSqlQuery(const QString &queryString)
{
    auto query = executeWithoutCommit(queryString);
    _sdb.commit();
    return query;
}

QSharedPointer<QSqlQuery> DB::select(const QString &queryString)
{
    return executeWithoutCommit(queryString);
}


QSqlDatabase& DB::sdb()
{
    _sdb.open();
    return _sdb;
}

void DB::connect()
{
    _sdb = QSqlDatabase::addDatabase("QSQLITE");
    _sdb.setDatabaseName(_dbFileName);
    _sdb.open();
}

QSharedPointer<QSqlQuery> DB::executeWithoutCommit(const QString &queryString)
{
    QSharedPointer<QSqlQuery> query(new QSqlQuery);
    query->prepare(queryString);

    qDebug() << query->exec();
    return query;
}
