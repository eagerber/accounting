#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QSqlQuery>


class DB
{
public:
    DB();
    ~DB();

    void init(QString dbFileName);
    void create(QString dbFileName);

    void executeSqlQuery(const QStringList &queryString);
    QSqlQuery executeSqlQuery(const QString &queryString);

    QSqlRelationalTableModel& model(QObject *parent);

private:
    void connect(QString dbFileName);

    QSqlDatabase _sdb;
    QString _dbFileName;
};

#endif // DATABASE_H
