#ifndef STATISTICSFROM_H
#define STATISTICSFROM_H

#include <QDialog>
#include <memory>

#include "db.h"


namespace Ui {
class StatisticsFrom;
}

class StatisticsFrom : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticsFrom(QWidget *parent = 0);
    ~StatisticsFrom();

    void updatePlot();

private slots:
    void on_sumOnCategoryButton_clicked();

    void on_consumptionByMonthPushButton_clicked();

private:
    void setupXAxis(QVector<double> ticks, QVector<QString> labels);
    void setupYAxis(double minValue, double maxValue);
    void setupLegend();

    Ui::StatisticsFrom *ui;
};

#endif // STATISTICSFROM_H
