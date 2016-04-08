#ifndef CONSUMPTIONBYMONTH_H
#define CONSUMPTIONBYMONTH_H

#include <memory>
#include "qcustomplot.h"
#include "db.h"


class ConsumptionByMonth
{
public:
    explicit ConsumptionByMonth(DB &db, QCustomPlot *customPlot);
    ~ConsumptionByMonth() = default;

    void replot();

private:
    ConsumptionByMonth() = default;
    void setupLegend();

    QCustomPlot *_customPlot;
    DB &_db;
};
#endif // CONSUMPTIONBYMONTH_H
