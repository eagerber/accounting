#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QString>

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
    Ui::MainWindow *ui;

    QString _dbFileName;
};

#endif // MAINWINDOW_H
