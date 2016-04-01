#include "db.h"

#include <QSqlQueryModel>
#include <QSqlRelation>

#include "db_constdef.h"


DB::DB()
{
}

DB::~DB()
{

}

void DB::init(QString dbFileName)
{
    if(dbFileName.isEmpty())
        return;

    connect(dbFileName);
}

void DB::executeSqlQuery(const QStringList &queryString)
{
    if (!_sdb.open())
        return;

    for(const auto &item : queryString)
    {
        executeSqlQuery(item);
    }
}

QSqlQuery DB::executeSqlQuery(const QString &queryString)
{
    QSqlQuery query;
    query.prepare(queryString);
    query.exec();

    return query;
}

QSqlRelationalTableModel& DB::model(QObject *parent)
{
    QSqlRelationalTableModel model(parent, _sdb);
    model.setEditStrategy(QSqlTableModel::OnFieldChange);
    model.setJoinMode(QSqlRelationalTableModel::LeftJoin);
    model.setTable("Purchases");
    model.select();

    return model;
}

void DB::create(QString dbFileName)
{
    _sdb = QSqlDatabase::addDatabase("QSQLITE");
    _sdb.setDatabaseName(dbFileName);

    if (!_sdb.open())
        return;

    executeSqlQuery(Queries::createDB);
}

void DB::connect(QString dbFileName)
{
    _dbFileName = dbFileName;
    _sdb = QSqlDatabase::addDatabase("QSQLITE");
    _sdb.setDatabaseName(_dbFileName);
}
