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

    std::shared_ptr<QSqlRelationalTableModel> model(QObject *parent);

private:
    void connect();

    QSqlDatabase _sdb;
    QString _dbFileName;
    std::shared_ptr<QSqlRelationalTableModel> _model;
};

#endif // DATABASE_H
