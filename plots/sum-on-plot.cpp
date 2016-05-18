#include "sum-on-plot.h"

#include <QPair>

#include "qcustomplot.h"
#include "db-constdef.h"
#include "db.h"


SumOnPlot::SumOnPlot(QCustomPlot *customPlot, PlotType plotType)
    : AccPlot(customPlot)
{
    _plotType = plotType;
}

void SumOnPlot::replot()
{
    _customPlot->clearPlottables();
    _customPlot->clearGraphs();
    _customPlot->clearItems();
    _customPlot->clearMask();
    _customPlot->clearFocus();
    _customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    QCPBars *plotBar = new QCPBars(_customPlot->xAxis, _customPlot->yAxis);
    _customPlot->addPlottable(plotBar);

    auto allRecords = DB::select(Queries::selectAll);
    QMap<QString, double> data;
    switch(_plotType)
    {
        case Categories:
        {
            data = categoriesPlotData(allRecords);
            plotBar->setName("Categories");
            break;
        }
        case Stores:
        {
            data = storesPlotData(allRecords);
            plotBar->setName("Stores");
            break;
        }
    }

    QList<QPair<QString, double>> sortedMapData;
    QMap<QString, double>::iterator item;
    for(item = data.begin(); item != data.end(); ++item)
    {
        QString key = item.key();
        double value = item.value();
        sortedMapData.append(QPair<QString, double>(key, value));
    }

    qSort(sortedMapData.begin(),
          sortedMapData.end(),
          [](QPair<QString, double> &l, QPair<QString, double> &r){ return l.second > r.second; });

    QVector<QString> labels;
    QVector<double>  values,
                     ticks;
    int count = 0;
    for(const auto &item : sortedMapData)
    {
        labels << item.first;
        values << item.second;
        ticks << ++count;
    }

    plotBar->setData(ticks, values);
    setupXAxis(ticks, labels);

    double maxValue = sortedMapData.begin()->second;
    setupYAxis(0.0, maxValue);
    setupLegend();

    _customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _customPlot->replot();
}

QMap<QString, double> SumOnPlot::categoriesPlotData(QSharedPointer<QSqlQuery> allRecords)
{
    auto categories = DB::select(Queries::categories);

    QMap<QString, double> categoriesData;
    while(categories->next())
    {
        QString category = categories->value(0).toString();
        categoriesData[category] = 0.0;
    }

    while(allRecords->next())
    {
        QString currentCategory = allRecords->value(8).toString(),
                currency = allRecords->value(5).toString();
        QDate date = allRecords->value(7).toDate();
        double purchaseCount = allRecords->value(3).toFloat(),
               purchasePrice = allRecords->value(4).toFloat(),
               course = exchangeRate(currency, date);
        categoriesData[currentCategory] += purchaseCount * purchasePrice * course;
    }

    return categoriesData;
}

QMap<QString, double> SumOnPlot::storesPlotData(QSharedPointer<QSqlQuery> allRecords)
{
    auto stores = DB::select(Queries::stores);

    QMap<QString, double> storesData;
    while(stores->next())
    {
        QString store = stores->value(0).toString();
        storesData[store] = 0.0;
    }

    // create empty bar chart objects:
    QCPBars *storesBar = new QCPBars(_customPlot->xAxis, _customPlot->yAxis);
    _customPlot->addPlottable(storesBar);
    storesBar->setName("Categories");

    while(allRecords->next())
    {
        QString currentStore = allRecords->value(2).toString(),
                currency = allRecords->value(5).toString();
        QDate date = allRecords->value(7).toDate();

        double purchaseCount = allRecords->value(3).toFloat(),
               purchasePrice = allRecords->value(4).toFloat(),
               course = exchangeRate(currency, date);
        storesData[currentStore] += purchaseCount * purchasePrice * course;
    }

    return storesData;
}

void SumOnPlot::setupXAxis(QVector<double> ticks, QVector<QString> labels)
{
    _customPlot->xAxis->setTickLabelType(QCPAxis::ltNumber);

    _customPlot->xAxis->setAutoTicks(false);
    _customPlot->xAxis->setAutoTickLabels(false);

    _customPlot->xAxis->setTickVector(ticks);
    _customPlot->xAxis->setTickVectorLabels(labels);

    _customPlot->xAxis->setSubTickCount(0);
    _customPlot->xAxis->setTickLength(0, 5);

    AccPlot::setupXAxis(0, ticks.size() + 1);
}
