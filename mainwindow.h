#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSql>
#include <QString>
#include <QCompleter>

#include <QStringListModel>
#include "db.h"
#include "db-settings-form.h"

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
    void on_pushButton_clicked();

    void on_updateButton_clicked();

    void on_createDBButton_clicked();

    void on_connectPushButton_clicked();

    void on_settingsPushButton_clicked();

private:
    void autoCompleteModelForField(const QString field, QStringListModel& completerModel);

    Ui::MainWindow *ui;

    DB _db;

    QCompleter _productCompleter, _storeCompleter, _categoryCompleter;
    QStringListModel _productCompleterModel, _storeCompleterModel, _categoryCompleterModel;

    DBSettingsForm _dbsettingsform;
};

#endif // MAINWINDOW_H
