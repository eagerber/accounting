#ifndef CONSUMPTIONBYMONTH_H
#define CONSUMPTIONBYMONTH_H


class QCustomPlot;

class ConsumptionByMonth
{
public:
    explicit ConsumptionByMonth(QCustomPlot *customPlot);
    ~ConsumptionByMonth() = default;

    void replot();

private:
    ConsumptionByMonth() = default;
    void setupLegend();

    QCustomPlot *_customPlot;
};
#endif // CONSUMPTIONBYMONTH_H
