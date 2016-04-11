#ifndef CONSUMPTIONBYMONTH_H
#define CONSUMPTIONBYMONTH_H

#include <QVector>

class QCustomPlot;

class ConsumptionByMonth
{
public:
    explicit ConsumptionByMonth(QCustomPlot *customPlot);
    ~ConsumptionByMonth() = default;

    void replot();

private:
    ConsumptionByMonth() = default;
    void setupXAxis(double minTime, double maxTime);
    void setupYAxis(double minValue, double maxValue);
    void setupLegend();

    QCustomPlot *_customPlot;
};
#endif // CONSUMPTIONBYMONTH_H
