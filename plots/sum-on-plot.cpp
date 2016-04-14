#include "sum-on-plot.h"

#include <QPair>

#include "qcustomplot.h"
#include "db-constdef.h"
#include "db.h"


SumOnPlot::SumOnPlot(QCustomPlot *customPlot, PlotType plotType)
    : AccPlot(customPlot)
{
    switch(plotType)
    {
        case Categories:
        {
            _queryString = Queries::sumOnCategories;
            break;
        }
        case Stores:
        {
            _queryString = Queries::sumOnStores;
            break;
        }
    }
}

void SumOnPlot::replot()
{
    auto query = DB::select(_queryString);

    _customPlot->clearPlottables();
    _customPlot->clearGraphs();
    _customPlot->clearItems();
    _customPlot->clearMask();
    _customPlot->clearFocus();

    _customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    // create empty bar chart objects:
    QCPBars *categories = new QCPBars(_customPlot->xAxis, _customPlot->yAxis);
    _customPlot->addPlottable(categories);
    categories->setName("Categories");

    // Add data:
    QVector<double> categoriesData;
    QVector<double> ticks;
    QVector<QString> labels;
    double minValue = std::numeric_limits<double>::max(),
           maxValue = std::numeric_limits<double>::min();

    int count = 0;
    while(query->next())
    {
        labels << query->value(0).toString();        
        double value = query->value(1).toFloat();
        categoriesData << value;
        minValue = qMin(minValue, value);
        maxValue = qMax(maxValue, value);
        ticks << ++count;
    }

    categories->setData(ticks, categoriesData);    

    setupXAxis(ticks, labels);
    setupYAxis(0.0, maxValue);
    setupLegend();

    _customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _customPlot->replot();
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
