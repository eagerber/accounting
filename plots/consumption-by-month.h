#ifndef CONSUMPTIONBYMONTH_H
#define CONSUMPTIONBYMONTH_H

#include "acc-plot.h"

#include <QVector>

class ConsumptionByMonth : public AccPlot
{
public:
    explicit ConsumptionByMonth(QCustomPlot *customPlot);
    ~ConsumptionByMonth() = default;

    virtual void replot() override;

private:
    ConsumptionByMonth() = default;
    void setupXAxis(double minTime, double maxTime);
};
#endif // CONSUMPTIONBYMONTH_H
