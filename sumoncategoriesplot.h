#ifndef SUMONCATEGORIESPLOT_H
#define SUMONCATEGORIESPLOT_H

#include <memory>
#include "qcustomplot.h"
#include "db.h"


class SumOnCategoriesPlot
{
public:
    explicit SumOnCategoriesPlot(DB &db, QCustomPlot *customPlot);
    ~SumOnCategoriesPlot() = default;

    void replot();

private:
    SumOnCategoriesPlot() = default;
    void setupXAxis(QVector<double> ticks, QVector<QString> labels);
    void setupYAxis(double minValue, double maxValue);
    void setupLegend();

    QCustomPlot *_customPlot;
    DB &_db;
};

#endif // SUMONCATEGORIESPLOT_H
