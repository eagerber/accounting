#ifndef SUMONCATEGORIESPLOT_H
#define SUMONCATEGORIESPLOT_H

#include "acc-plot.h"

#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QSqlQuery>

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
    QMap<QString, double> categoriesPlotData(QSharedPointer<QSqlQuery> allRecords);
    QMap<QString, double> storesPlotData(QSharedPointer<QSqlQuery> allRecords);

    void setupXAxis(QVector<double> ticks, QVector<QString> labels);

    PlotType _plotType;
};

#endif // SUMONCATEGORIESPLOT_H
