#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSql>
#include <QString>
#include <QCompleter>

#include <QStringListModel>
#include "db.h"
#include "db-settings-form.h"
#include "statisticsfrom.h"

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

private:
    void autoCompleteModelForField(const QString field, QStringListModel& completerModel);
    void resizeTableView();
    void readSettings(QString settingsFileName);
    void updateTableView();
    void insert();

    Ui::MainWindow *ui;

    DB _db;

    QCompleter _productCompleter, _storeCompleter, _categoryCompleter;
    QStringListModel _productCompleterModel, _storeCompleterModel, _categoryCompleterModel;

    DBSettingsForm _dbsettingsForm;
    StatisticsFrom _statisticsForm;
};

#endif // MAINWINDOW_H
