#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QSqlRelation>
#include <QFileDialog>
#include <QMessageBox>
#include <QCompleter>


void MainWindow::autoCompleteModelForField(const QString field, QStringListModel &completerModel)
{
    if (!_sdb.open())
        return;

    QString queryString("SELECT DISTINCT [%1] FROM Purchases;");

    QSqlQuery query(queryString.arg(field));
    query.exec();

    QStringList list;
    while(query.next())
    {
        list.push_back(query.value(field).toString());
    }

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


    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("db-scripts");

    QFileInfoList dirContent = dir.entryInfoList({"*.sql"});
    for(const auto &item : dirContent)
    {
        ui->convertScriptComboBox->addItem(item.fileName());
    }

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

    QSqlQuery query;
    query.prepare("INSERT INTO Purchases (Product, Store, Count, Price, Category, Date) VALUES (:Product, :Store, :Count, :Price, :Category, :Date);");

    query.bindValue( ":Product", ui->productNameEdit->text());
    query.bindValue(   ":Store", ui->storeNameEdit->text());
    query.bindValue(   ":Count", ui->countEdit->text().replace(",", "."));
    query.bindValue(   ":Price", ui->priceEdit->text().replace(",", "."));
    query.bindValue(":Category", ui->categoryEdit->text());
    query.bindValue(    ":Date", ui->dateEdit->text());

    qDebug()<<query.exec()<<endl;

    autoCompleteModelForField("Product", _productCompleterModel);
    autoCompleteModelForField("Store", _storeCompleterModel);
    autoCompleteModelForField("Category", _categoryCompleterModel);

    ui->productNameEdit->setFocus();
}

void MainWindow::on_updateButton_clicked()
{
    if (!_sdb.open())
        return;

    QSqlRelationalTableModel *model = new QSqlRelationalTableModel(this, _sdb);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    model->setTable("Purchases");
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0, 30);
    ui->tableView->setColumnWidth(1, 150);
    ui->tableView->setColumnWidth(2, 150);
    ui->tableView->setColumnWidth(3, 50);
    ui->tableView->setColumnWidth(4, 50);
    ui->tableView->setColumnWidth(5, 100);
    ui->tableView->setColumnWidth(6, 100);
    ui->tableView->setColumnWidth(7, 100);
}

void MainWindow::on_createDBButton_clicked()
{
    _dbFileName = QFileDialog::getSaveFileName(this, tr("Choose DB"), "", tr("SQL Lite Files (*.sqldb *.db)"));

    if(_dbFileName.isEmpty())
        return;

    _sdb = QSqlDatabase::addDatabase("QSQLITE");
    _sdb.setDatabaseName(_dbFileName);

    if (!_sdb.open())
        return;

    QSqlQuery query;
    query.prepare("\
CREATE TABLE [Purchases] ( \n\
[ID] INTEGER PRIMARY KEY AUTOINCREMENT, \n\
[Product] TEXT NOT NULL ON CONFLICT FAIL, \n\
[Store] TEXT NOT NULL ON CONFLICT FAIL, \n\
[Count] INT NOT NULL ON CONFLICT FAIL DEFAULT 1, \n\
[Price] FLOAT NOT NULL ON CONFLICT FAIL, \n\
[Currency] CHAR(3) NOT NULL ON CONFLICT FAIL DEFAULT RUB, \n\
[Discount] BOOL NOT NULL ON CONFLICT FAIL DEFAULT FALSE , \n\
[Date] DATE NOT NULL ON CONFLICT FAIL DEFAULT TODAY, \n\
[Category] TEXT NOT NULL ON CONFLICT FAIL DEFAULT Other);");


    query.exec();
}

void MainWindow::on_connectPushButton_clicked()
{
    _dbFileName = QFileDialog::getOpenFileName(this, tr("Choose DB"), "", tr("SQL Lite Files (*.sqldb *.db)"));

    if(_dbFileName.isEmpty())
        return;

    _sdb = QSqlDatabase::addDatabase("QSQLITE");
    _sdb.setDatabaseName(_dbFileName);

    autoCompleteModelForField("Product", _productCompleterModel);
    autoCompleteModelForField("Store", _storeCompleterModel);
    autoCompleteModelForField("Category", _categoryCompleterModel);
}

void MainWindow::executeSqlQuery(const QStringList &queryString)
{
    if (!_sdb.open())
        return;

    for(const auto &item : queryString)
    {
        QSqlQuery query;
        query.prepare(item);
        query.exec();
    }
}

void MainWindow::on_convertDBButton_clicked()
{
    if(_dbFileName.isEmpty())
        return;

    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("db-scripts");

    QString fileName = dir.path() + "/" + ui->convertScriptComboBox->currentText();
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return;

    QTextStream in(&file);
    QStringList sqlScript = in.readAll().split(";");
    sqlScript.removeLast();

    file.close();
    executeSqlQuery(sqlScript);
}

void MainWindow::on_productNameEdit_returnPressed()
{
    ui->pushButton->click();
}

void MainWindow::on_storeNameEdit_returnPressed()
{
    ui->pushButton->click();
}

void MainWindow::on_countEdit_returnPressed()
{
    ui->pushButton->click();
}

void MainWindow::on_priceEdit_returnPressed()
{
    ui->pushButton->click();
}

void MainWindow::on_currencyEdit_returnPressed()
{
    ui->pushButton->click();
}

void MainWindow::on_categoryEdit_returnPressed()
{
    ui->pushButton->click();
}

void MainWindow::on_discountEdit_returnPressed()
{
    ui->pushButton->click();
}

void MainWindow::on_dateEdit_returnPressed()
{
    ui->pushButton->click();
}
