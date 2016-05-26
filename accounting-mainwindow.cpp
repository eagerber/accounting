#include "accounting-mainwindow.h"
#include "ui_accounting-mainwindow.h"

#include "queries.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCompleter>
#include <QSettings>
#include <QDesktopWidget>


AccountingMainWindow::AccountingMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AccountingMainWindow),
    _dbsettingsForm(this),
    _statisticsForm(this)
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
        800, 500);
    _statisticsForm.show();

    connect(ui->plotAction, &QAction::triggered, this, &AccountingMainWindow::plotForm);
    connect(ui->settingsAction, &QAction::triggered, this, &AccountingMainWindow::settingsForm);

    ui->tableView->setSortingEnabled(true);
}

AccountingMainWindow::~AccountingMainWindow()
{
    delete ui;
}

void AccountingMainWindow::on_updateButton_clicked()
{
    updateTableView();
}

void AccountingMainWindow::on_connectPushButton_clicked()
{
    QString dbFileName = QFileDialog::getOpenFileName(
        this, tr("Choose DB"), "", tr("SQL Lite Files (*.sqldb *.db)"));

    if(dbFileName.isEmpty())
        return;

    DB::init(dbFileName);

    autoCompleteModelForField("Product", _productCompleterModel);
    autoCompleteModelForField("Store", _storeCompleterModel);
    autoCompleteModelForField("Category", _categoryCompleterModel);
}

void AccountingMainWindow::on_settingsPushButton_clicked()
{
    _dbsettingsForm.show();
}

void AccountingMainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    resizeTableView();
}

void AccountingMainWindow::on_insertPushButton_clicked()
{
    insert();
    ui->productNameEdit->setFocus();
    ui->productNameEdit->selectAll();

    updateTableView();
}

void AccountingMainWindow::on_productNameEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void AccountingMainWindow::on_storeNameEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void AccountingMainWindow::on_countEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void AccountingMainWindow::on_priceEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void AccountingMainWindow::on_currencyEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void AccountingMainWindow::on_categoryEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void AccountingMainWindow::on_discountEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void AccountingMainWindow::on_dateEdit_returnPressed()
{
    on_insertPushButton_clicked();
}

void AccountingMainWindow::plotForm()
{
    reverseViewState(_statisticsForm);
}

void AccountingMainWindow::settingsForm()
{
    reverseViewState(_dbsettingsForm);
}

void AccountingMainWindow::autoCompleteModelForField(const QString field, QStringListModel &completerModel)
{
    QString queryString = Queries::distinctField(field);
    auto query = DB::executeSqlQuery(queryString);

    if(query.isNull())
        return;

    QStringList list;
    while(query->next())
    {
        list.push_back(query->value(field).toString());
    }
    completerModel.setStringList(list);
}

void AccountingMainWindow::resizeTableView()
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

void AccountingMainWindow::readSettings(QString settingsFileName)
{
    QSettings settings(settingsFileName, QSettings::IniFormat);

    QString dbFileName = settings.value("SQLiteDBFileName", "").toString();
    DB::init(dbFileName);
}

void AccountingMainWindow::updateTableView(QString filter)
{
    auto model = new QSqlRelationalTableModel(this, DB::sdb());

    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    model->setTable(Queries::purchasesTable);

    if(!filter.isEmpty())
    {
        model->setFilter(filter);
    }

    model->select();

    ui->tableView->setModel(model);

    autoCompleteModelForField(Queries::product, _productCompleterModel);
    autoCompleteModelForField(Queries::store, _storeCompleterModel);
    autoCompleteModelForField(Queries::category, _categoryCompleterModel);

    resizeTableView();
}

void AccountingMainWindow::insert()
{
    QString queryString = Queries::insertIntoPurchases(
        ui->productNameEdit->text(),
        ui->storeNameEdit->text(),
        ui->countEdit->text().replace(",", "."),
        ui->priceEdit->text().replace(",", "."),
        ui->currencyEdit->text(),
        ui->categoryEdit->text(),
        ui->dateEdit->text());

    qDebug() << queryString;
    DB::executeSqlQuery(queryString);

    autoCompleteModelForField(Queries::product, _productCompleterModel);
    autoCompleteModelForField(Queries::store, _storeCompleterModel);
    autoCompleteModelForField(Queries::category, _categoryCompleterModel);
}

void AccountingMainWindow::reverseViewState(QDialog &dialog)
{
    dialog.isVisible() ? dialog.hide() : dialog.show();
}

void AccountingMainWindow::on_updateLineEdit_editingFinished()
{
    QString pattern = "'%" + ui->updateLineEdit->text() + "%'";
    updateTableView("LOWER(Purchases.Product) LIKE " + pattern + "OR LOWER(Purchases.Store) LIKE " + pattern);
}
