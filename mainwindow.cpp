#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql>
#include <QSqlQueryModel>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("C:\\usr\\other\\accounting\\accounting.db");

    if (!sdb.open())
        return;

    QSqlQuery query;
    query.prepare("INSERT INTO Purchases (ProductName, StoreName, Count, Price, Category, Date) VALUES (:ProductName, :StoreName, :Count, :Price, :Category, :Date);");

    query.bindValue(":ProductName", ui->productNameEdit->text());
    query.bindValue(  ":StoreName", ui->storeNameEdit->text());
    query.bindValue(      ":Count", ui->countEdit->text());
    query.bindValue(      ":Price", ui->priceEdit->text());
    query.bindValue(   ":Category", ui->categoryEdit->text());
    query.bindValue(       ":Date", ui->dateEdit->text());

    qDebug()<<query.exec()<<endl;

    sdb.close();
}

void MainWindow::on_updateButton_clicked()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("C:\\usr\\other\\accounting\\accounting.db");

    if (!sdb.open())
        return;

    QSqlQueryModel *model = new QSqlQueryModel;

    QSqlQuery query;
    query.prepare("SELECT [ID], [ProductName], [StoreName], [Count], [Price], [Date], [Category] FROM Purchases");
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "ProductName");
    model->setHeaderData(2, Qt::Horizontal, "StoreName");
    model->setHeaderData(3, Qt::Horizontal, "Count");
    model->setHeaderData(4, Qt::Horizontal, "Price");
    model->setHeaderData(5, Qt::Horizontal, "Date");
    model->setHeaderData(6, Qt::Horizontal, "Category");

    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0, 30);
    ui->tableView->setColumnWidth(1, 150);
    ui->tableView->setColumnWidth(2, 150);
    ui->tableView->setColumnWidth(3, 50);
    ui->tableView->setColumnWidth(4, 50);
    ui->tableView->setColumnWidth(5, 100);
    ui->tableView->setColumnWidth(6, 100);
    sdb.close();
}

void MainWindow::on_createDBButton_clicked()
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("C:\\usr\\other\\accounting\\accounting.db");

    if (!sdb.open())
        return;

    QSqlQuery query;
    query.prepare("\
CREATE TABLE [Purchases] (                                \
[ID] INTEGER PRIMARY KEY AUTOINCREMENT,                   \
[ProductName] TEXT NOT NULL ON CONFLICT FAIL,             \
[StoreName] TEXT NOT NULL ON CONFLICT FAIL,               \
[Count] INT NOT NULL ON CONFLICT FAIL DEFAULT 1,          \
[Price] FLOAT NOT NULL ON CONFLICT FAIL,                  \
[Currency] CHAR(3) NOT NULL ON CONFLICT FAIL DEFAULT RUB, \
[Date] DATE NOT NULL ON CONFLICT FAIL DEFAULT TODAY,      \
[Category] TEXT NOT NULL ON CONFLICT FAIL DEFAULT Other);");


    query.exec();
    sdb.close();
}
