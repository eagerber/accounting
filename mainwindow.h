#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSql>
#include <QString>
#include <QCompleter>

#include <QStringListModel>
#include "db.h"
#include "db-settings-form.h"
#include "statistics-from.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_updateButton_clicked();

    void on_createDBButton_clicked();

    void on_connectPushButton_clicked();

    void on_settingsPushButton_clicked();

    void resizeEvent(QResizeEvent* event);

    void on_insertPushButton_clicked();

    void on_productNameEdit_returnPressed();

    void on_storeNameEdit_returnPressed();

    void on_countEdit_returnPressed();

    void on_priceEdit_returnPressed();

    void on_currencyEdit_returnPressed();

    void on_categoryEdit_returnPressed();

    void on_discountEdit_returnPressed();

    void on_dateEdit_returnPressed();

    void plotForm();
    void settingsForm();

private:
    void autoCompleteModelForField(const QString field, QStringListModel& completerModel);
    void resizeTableView();
    void readSettings(QString settingsFileName);
    void updateTableView();
    void insert();    

    void reverseViewState(QDialog &dialog);

    Ui::MainWindow *ui;

    QCompleter _productCompleter, _storeCompleter, _categoryCompleter;
    QStringListModel _productCompleterModel, _storeCompleterModel, _categoryCompleterModel;

    DBSettingsForm _dbsettingsForm;
    StatisticsFrom _statisticsForm;
};

#endif // MAINWINDOW_H
