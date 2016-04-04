#include "db.h"

#include <QDebug>

#include "db-constdef.h"


DB::DB()
{
    _model = nullptr;
}

DB::~DB()
{

}

void DB::init(QString dbFileName)
{
    if(dbFileName.isEmpty())
        return;
    _dbFileName = dbFileName;

    connect();
}

void DB::connect()
{
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

std::shared_ptr<QSqlRelationalTableModel> DB::model(QObject *parent)
{
    _sdb.open();

    _model.reset();
    _model = std::shared_ptr<QSqlRelationalTableModel>(new QSqlRelationalTableModel(parent, _sdb));

    _model->setEditStrategy(QSqlTableModel::OnFieldChange);
    _model->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    _model->setTable("Purchases");
    _model->select();

    return _model;
}

void DB::create(QString dbFileName)
{
    _sdb = QSqlDatabase::addDatabase("QSQLITE");
    _sdb.setDatabaseName(dbFileName);

    if (!_sdb.open())
        return;

    executeSqlQuery(Queries::createDB);
}
