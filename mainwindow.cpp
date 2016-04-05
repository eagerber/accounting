#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "db-constdef.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCompleter>
#include <QSettings>
#include <QDesktopWidget>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _dbsettingsForm(this),
    _statisticsForm(_db, this),
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

    _dbsettingsForm.setWindowTitle("Database Settings");

    QString settingsFileName = QDir::current().filePath("settings.ini");
    readSettings(settingsFileName);

    updateTableView();

    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen()); // прямоугольник с размерами экрана
    QPoint center = rect.center(); //координаты центра экрана

    _statisticsForm.setGeometry(
                center.x() + geometry().width() / 2 + 20,
                center.y() - geometry().height() / 2,
                300,
                300);
    _statisticsForm.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_updateButton_clicked()
{
    updateTableView();
}

void MainWindow::on_createDBButton_clicked()
{
    QString dbFileName = QFileDialog::getSaveFileName(
                this, tr("Choose DB"), "", tr("SQL Lite Files (*.sqldb *.db)"));

    if(dbFileName.isEmpty())
        return;

    _db.create(dbFileName);
}

void MainWindow::on_connectPushButton_clicked()
{
    QString dbFileName = QFileDialog::getOpenFileName(
                this, tr("Choose DB"), "", tr("SQL Lite Files (*.sqldb *.db)"));

    if(dbFileName.isEmpty())
        return;

    _db.init(dbFileName);

    autoCompleteModelForField("Product", _productCompleterModel);
    autoCompleteModelForField("Store", _storeCompleterModel);
    autoCompleteModelForField("Category", _categoryCompleterModel);
}

void MainWindow::on_settingsPushButton_clicked()
{
    _dbsettingsForm.show();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    resizeTableView();
}

void MainWindow::on_insertPushButton_clicked()
{
    insert();
    ui->productNameEdit->setFocus();
    ui->productNameEdit->selectAll();

    updateTableView();
}

void MainWindow::on_productNameEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void MainWindow::on_storeNameEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void MainWindow::on_countEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void MainWindow::on_priceEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void MainWindow::on_currencyEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void MainWindow::on_categoryEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void MainWindow::on_discountEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void MainWindow::on_dateEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void MainWindow::autoCompleteModelForField(const QString field, QStringListModel &completerModel)
{
    QString queryString = Queries::distinctField(field);
    auto query = _db.executeSqlQuery(queryString);

    if(query.isNull())
        return;

    QStringList list;
    while(query->next())
    {
        list.push_back(query->value(field).toString());
    }
    completerModel.setStringList(list);
}

void MainWindow::resizeTableView()
{
    auto &tableView = ui->tableView;

    tableView->verticalHeader()->setDefaultSectionSize(20);
    tableView->verticalHeader()->hide();

    float width = this->width() / 100.0f - 1.5;
    tableView->setColumnWidth(0, round(width * 4));
    tableView->setColumnWidth(1, round(width * 20));
    tableView->setColumnWidth(2, round(width * 20));
    tableView->setColumnWidth(3, round(width * 7));
    tableView->setColumnWidth(4, round(width * 7));
    tableView->setColumnWidth(5, round(width * 14));
    tableView->setColumnWidth(6, round(width * 14));
    tableView->setColumnWidth(7, round(width * 14));
}

void MainWindow::readSettings(QString settingsFileName)
{
    QSettings settings(settingsFileName, QSettings::IniFormat);

    QString dbFileName = settings.value("SQLiteDBFileName", "").toString();
    _db.init(dbFileName);
}

void MainWindow::updateTableView()
{
    auto model = new QSqlRelationalTableModel(this, _db.sdb());

    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    model->setTable(Queries::purchasesTable);
    model->select();

    ui->tableView->setModel(model);

    resizeTableView();
}

void MainWindow::insert()
{
    QString queryString = Queries::insertIntoPurchases(
        ui->productNameEdit->text(),
        ui->storeNameEdit->text(),
        ui->countEdit->text().replace(",", "."),
        ui->priceEdit->text().replace(",", "."),
        ui->categoryEdit->text(),
        ui->dateEdit->text());

    qDebug() << queryString;
    _db.executeSqlQuery(queryString);

    autoCompleteModelForField(Queries::product, _productCompleterModel);
    autoCompleteModelForField(Queries::store, _storeCompleterModel);
    autoCompleteModelForField(Queries::category, _categoryCompleterModel);
}
