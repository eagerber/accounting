#ifndef SUMONCATEGORIESPLOT_H
#define SUMONCATEGORIESPLOT_H

#include "acc-plot.h"

#include <QString>
#include <QVector>
#include <QDateTime>
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
    double exchangeRate(QString currency, QDate date);
    void categoriesPlot(QSharedPointer<QSqlQuery> allRecords);
    void storesPlot(QSharedPointer<QSqlQuery> allRecords);

    void setupXAxis(QVector<double> ticks, QVector<QString> labels);

    PlotType _plotType;
};

#endif // SUMONCATEGORIESPLOT_H
