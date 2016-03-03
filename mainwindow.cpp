#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QCompleter>


QStringListModel* MainWindow::autoCompleteModelForField(const QString field, QStringListModel &completerModel)
{
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(_dbFileName);

    if (!sdb.open())
        return new QStringListModel();

    QString queryString("SELECT DISTINCT [%1] FROM Purchases;");

    QSqlQuery query(queryString.arg(field));
    query.exec();

    QStringList list;
    while(query.next())
    {
        list.push_back(query.value(field).toString());
    }

    sdb.close();

    completerModel.setStringList(list);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _dbFileName = "";

    ui->dateEdit->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    ui->currencyEdit->setText("RUB");
    ui->discountEdit->setText("");

    ui->productNameEdit->setCompleter(&_productCompleter);
    ui->storeNameEdit->setCompleter(&_storeCompleter);
    ui->categoryEdit->setCompleter(&_categoryCompleter);

    _productCompleter.setModel(&_productCompleterModel);
    _storeCompleter.setModel(&_storeCompleterModel);
    _categoryCompleter.setModel(&_categoryCompleterModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(_dbFileName.isEmpty())
    {
        QMessageBox::warning(0, tr("DB warrrning."), tr("No associated DB file."));
        return;
    }

    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(_dbFileName);

    if (!sdb.open())
        return;

    QSqlQuery query;
    query.prepare("INSERT INTO Purchases (ProductName, StoreName, Count, Price, Category, Date) VALUES (:ProductName, :StoreName, :Count, :Price, :Category, :Date);");

    query.bindValue(":ProductName", ui->productNameEdit->text());
    query.bindValue(  ":StoreName", ui->storeNameEdit->text());
    query.bindValue(      ":Count", ui->countEdit->text().replace(",", "."));
    query.bindValue(      ":Price", ui->priceEdit->text().replace(",", "."));
    query.bindValue(   ":Category", ui->categoryEdit->text());
    query.bindValue(       ":Date", ui->dateEdit->text());

    qDebug()<<query.exec()<<endl;

    sdb.close();

    autoCompleteModelForField("ProductName", _productCompleterModel);
    autoCompleteModelForField("StoreName", _storeCompleterModel);
    autoCompleteModelForField("Category", _categoryCompleterModel);
}

void MainWindow::on_updateButton_clicked()
{
    if(_dbFileName.isEmpty())
        return;

    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(_dbFileName);

    if (!sdb.open())
        return;

    QSqlQueryModel *model = new QSqlQueryModel;

    QSqlQuery query;
    query.prepare("\
SELECT [ID], [ProductName], [StoreName], [Count], [Price], [Date], [Category], [Discount] \n\
FROM Purchases;");
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "ProductName");
    model->setHeaderData(2, Qt::Horizontal, "StoreName");
    model->setHeaderData(3, Qt::Horizontal, "Count");
    model->setHeaderData(4, Qt::Horizontal, "Price");
    model->setHeaderData(5, Qt::Horizontal, "Date");
    model->setHeaderData(6, Qt::Horizontal, "Category");
    model->setHeaderData(7, Qt::Horizontal, "Discount");

    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0, 30);
    ui->tableView->setColumnWidth(1, 150);
    ui->tableView->setColumnWidth(2, 150);
    ui->tableView->setColumnWidth(3, 50);
    ui->tableView->setColumnWidth(4, 50);
    ui->tableView->setColumnWidth(5, 100);
    ui->tableView->setColumnWidth(6, 100);
    ui->tableView->setColumnWidth(7, 100);
    sdb.close();
}

void MainWindow::on_createDBButton_clicked()
{
    _dbFileName = QFileDialog::getSaveFileName(this, tr("Choose DB"), "", tr("SQL Lite Files (*.sqldb *.db)"));

    if(_dbFileName.isEmpty())
        return;

    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(_dbFileName);

    if (!sdb.open())
        return;

    QSqlQuery query;
    query.prepare("\
CREATE TABLE [Purchases] (                                \n\
[ID] INTEGER PRIMARY KEY AUTOINCREMENT,                   \n\
[ProductName] TEXT NOT NULL ON CONFLICT FAIL,             \n\
[StoreName] TEXT NOT NULL ON CONFLICT FAIL,               \n\
[Count] INT NOT NULL ON CONFLICT FAIL DEFAULT 1,          \n\
[Price] FLOAT NOT NULL ON CONFLICT FAIL,                  \n\
[Currency] CHAR(3) NOT NULL ON CONFLICT FAIL DEFAULT RUB, \n\
[Discount] BOOL NOT NULL ON CONFLICT FAIL DEFAULT FALSE , \n\
[Date] DATE NOT NULL ON CONFLICT FAIL DEFAULT TODAY,      \n\
[Category] TEXT NOT NULL ON CONFLICT FAIL DEFAULT Other);");


    query.exec();
    sdb.close();
}

void MainWindow::on_connectPushButton_clicked()
{
    _dbFileName = QFileDialog::getOpenFileName(this, tr("Choose DB"), "", tr("SQL Lite Files (*.sqldb *.db)"));

    autoCompleteModelForField("ProductName", _productCompleterModel);
    autoCompleteModelForField("StoreName", _storeCompleterModel);
    autoCompleteModelForField("Category", _categoryCompleterModel);
}

void MainWindow::on_testButton_clicked()
{
    QString str = "156.50";

    float a = str.toFloat();

    str = "156,50";
    a = str.replace(",", ".").toFloat();

    int ff = 5;
}
