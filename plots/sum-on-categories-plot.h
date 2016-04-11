#ifndef SUMONCATEGORIESPLOT_H
#define SUMONCATEGORIESPLOT_H

#include "acc-plot.h"

#include <QVector>

class SumOnCategoriesPlot : public AccPlot
{
public:
    explicit SumOnCategoriesPlot(QCustomPlot *customPlot);
    ~SumOnCategoriesPlot() = default;

    virtual void replot() override;

private:
    SumOnCategoriesPlot() = default;
    void setupXAxis(QVector<double> ticks, QVector<QString> labels);
};

#endif // SUMONCATEGORIESPLOT_H
