#include "db.h"

#include <QSqlQueryModel>
#include <QSqlRelation>
#include <QDebug>

#include "db-constdef.h"


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

void DB::connect(QString dbFileName)
{
    _dbFileName = dbFileName;
    _sdb = QSqlDatabase::addDatabase("QSQLITE");
    _sdb.setDatabaseName(_dbFileName);
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
    if (!_sdb.open())
        return QSharedPointer<QSqlQuery>(nullptr);

    QSharedPointer<QSqlQuery> query(new QSqlQuery);
    query->prepare(queryString);

    qDebug() << query->exec();

    _sdb.commit();
    return query;
}

QSqlRelationalTableModel* DB::model(QObject *parent)
{
    _sdb.open();

    QSqlRelationalTableModel* model(new QSqlRelationalTableModel(parent, _sdb));
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    model->setTable("Purchases");
    model->select();

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
