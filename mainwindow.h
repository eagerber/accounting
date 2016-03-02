#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QString>
#include <QCompleter>

class QStringListModel;

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

    void on_testButton_clicked();

private:
    QStringListModel* autoCompleteModelForField(const QString field);

    Ui::MainWindow *ui;

    QString _dbFileName;
    QCompleter _productNameCompleter, _storeNameCompleter;
};

#endif // MAINWINDOW_H
