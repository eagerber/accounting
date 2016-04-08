#include "db.h"

#include <QDebug>

#include "db-constdef.h"


class DbImplementation
{
public:
    void init(QString dbFileName)
    {
        if(dbFileName.isEmpty())
            return;
        _dbFileName = dbFileName;

        connect();
    }
    void create(QString dbFileName)
    {
        _sdb = QSqlDatabase::addDatabase("QSQLITE");
        _sdb.setDatabaseName(dbFileName);

        executeSqlQuery(Queries::createDB);
    }

    void executeSqlQuery(const QStringList &queryString)
    {
        for(const auto &item : queryString)
        {
            executeSqlQuery(item);
        }
    }
    QSharedPointer<QSqlQuery> executeSqlQuery(const QString &queryString)
    {
        auto query = executeWithoutCommit(queryString);
        _sdb.commit();
        return query;
    }

    QSharedPointer<QSqlQuery> select(const QString &queryString)
    {
        return executeWithoutCommit(queryString);
    }

    QSqlDatabase& sdb()
    {
        _sdb.open();
        return _sdb;
    }

private:
    void connect()
    {
        _sdb = QSqlDatabase::addDatabase("QSQLITE");
        _sdb.setDatabaseName(_dbFileName);
        _sdb.open();
    }
    QSharedPointer<QSqlQuery> executeWithoutCommit(const QString &queryString)
    {
        QSharedPointer<QSqlQuery> query(new QSqlQuery);
        query->prepare(queryString);

        qDebug() << query->exec();
        return query;
    }

    QSqlDatabase _sdb;
    QString _dbFileName;
};

void DB::init(QString dbFileName)
{
    instance()->init(dbFileName);
}

void DB::create(QString dbFileName)
{
    instance()->create(dbFileName);
}

void DB::executeSqlQuery(const QStringList &queryString)
{
    instance()->executeSqlQuery(queryString);
}

QSharedPointer<QSqlQuery> DB::executeSqlQuery(const QString &queryString)
{
    return instance()->executeSqlQuery(queryString);
}

QSharedPointer<QSqlQuery> DB::select(const QString &queryString)
{
    return instance()->executeSqlQuery(queryString);
}

QSqlDatabase& DB::sdb()
{
    return instance()->sdb();
}

DbImplementation* DB::instance()
{
    static DbImplementation* implementation = new DbImplementation();
    return implementation;
}

