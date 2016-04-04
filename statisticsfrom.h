#ifndef STATISTICSFROM_H
#define STATISTICSFROM_H

#include <QDialog>

namespace Ui {
class StatisticsFrom;
}

class StatisticsFrom : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsFrom(QWidget *parent = 0);
    ~StatisticsFrom();

private slots:
    void on_pushButton_clicked();

private:
    Ui::StatisticsFrom *ui;
};

#endif // STATISTICSFROM_H
