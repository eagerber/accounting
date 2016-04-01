#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dbsettingsform.h"
#include "db_constdef.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCompleter>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    QString queryString = Queries::insertIntoPurchases(
        ui->productNameEdit->text(),
        ui->storeNameEdit->text(),
        ui->countEdit->text().replace(",", "."),
        ui->priceEdit->text().replace(",", "."),
        ui->categoryEdit->text(),
        ui->dateEdit->text());

    _db.executeSqlQuery(queryString);

    autoCompleteModelForField(Queries::product, _productCompleterModel);
    autoCompleteModelForField(Queries::store, _storeCompleterModel);
    autoCompleteModelForField(Queries::category, _categoryCompleterModel);

    ui->productNameEdit->setFocus();
}

void MainWindow::on_updateButton_clicked()
{
    QSqlRelationalTableModel& model = _db.model(this);
    ui->tableView->setModel(&model);
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
    QString dbFileName = QFileDialog::getSaveFileName(
                this,
                tr("Choose DB"),
                tr("SQL Lite Files (*.sqldb *.db)"),
                tr("SQL Lite Files (*.sqldb *.db)"));

    if(dbFileName.isEmpty())
        return;

    _db.create(dbFileName);
}

void MainWindow::on_connectPushButton_clicked()
{
    QString dbFileName = QFileDialog::getOpenFileName(
                this,
                tr("Choose DB"),
                tr("SQL Lite Files (*.sqldb *.db)"),
                tr("SQL Lite Files (*.sqldb *.db)"));

    if(dbFileName.isEmpty())
        return;

    _db.init(dbFileName);

    autoCompleteModelForField("Product", _productCompleterModel);
    autoCompleteModelForField("Store", _storeCompleterModel);
    autoCompleteModelForField("Category", _categoryCompleterModel);
}

void MainWindow::on_pushButton_2_clicked()
{
    DBSettingsForm dbsettingsform(this);
    dbsettingsform.show();
}

void MainWindow::autoCompleteModelForField(const QString field, QStringListModel &completerModel)
{
    QSqlQuery query = _db.executeSqlQuery(Queries::distinctPurchases.arg(field));

    QStringList list;
    while(query.next())
    {
        list.push_back(query.value(field).toString());
    }
    completerModel.setStringList(list);
}
