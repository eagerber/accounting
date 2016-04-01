#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSql>
#include <QString>к
#include <QCompleter>

#include <QStringListModel>

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

    void on_convertDBButton_clicked();

    void on_productNameEdit_returnPressed();

    void on_storeNameEdit_returnPressed();

    void on_countEdit_returnPressed();

    void on_priceEdit_returnPressed();

    void on_currencyEdit_returnPressed();

    void on_categoryEdit_returnPressed();

    void on_discountEdit_returnPressed();

    void on_dateEdit_returnPressed();

private:
    void autoCompleteModelForField(const QString field, QStringListModel& completerModel);
    void executeSqlQuery(const QStringList &queryString);

    Ui::MainWindow *ui;

    QSqlDatabase _sdb;

    QString _dbFileName;
    QCompleter _productCompleter, _storeCompleter, _categoryCompleter;
    QStringListModel _productCompleterModel, _storeCompleterModel, _categoryCompleterModel;
};

#endif // MAINWINDOW_H
