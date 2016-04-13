#ifndef SUMONCATEGORIESPLOT_H
#define SUMONCATEGORIESPLOT_H

#include "acc-plot.h"

#include <QString>
#include <QVector>

class SumOnPlot : public AccPlot
{
public:
    enum PlotType
    {
        Categories,
        Stores
    };

    explicit SumOnPlot(QCustomPlot *customPlot, PlotType plotType);
    ~SumOnPlot() = default;

    virtual void replot() override;

private:
    SumOnPlot() = default;
    void setupXAxis(QVector<double> ticks, QVector<QString> labels);

    QString _queryString;
};

#endif // SUMONCATEGORIESPLOT_H
