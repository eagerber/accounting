#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql>

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
    query.prepare("INSERT INTO Purchases (ProductName, StoreName, Count, Price, Date) VALUES (:ProductName, :StoreName, :Count, :Price, :Date);");

    query.bindValue(":ProductName", ui->productNameEdit->text());
    query.bindValue(  ":StoreName", ui->storeNameEdit->text());
    query.bindValue(      ":Count", ui->countEdit->text());
    query.bindValue(      ":Price", ui->priceEdit->text());
    query.bindValue(       ":Date", ui->dateEdit->text());

    qDebug()<<query.exec()<<endl;

    sdb.close();
}
