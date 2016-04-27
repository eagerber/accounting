#ifndef STATISTICSFROM_H
#define STATISTICSFROM_H

#include <QDialog>
#include <QStandardItemModel>
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

    void on_sumOnStoresButton_clicked();

    void on_accumulateSumByDatePushButton_clicked();

    void resizeEvent(QResizeEvent* event);

private:
    void setupXAxis(QVector<double> ticks, QVector<QString> labels);
    void setupYAxis(double minValue, double maxValue);
    void setupLegend();

    void updateTableView();
    void resizeTableView();

    Ui::StatisticsFrom *ui;
    QStandardItemModel *tableViewModel;
};

#endif // STATISTICSFROM_H
