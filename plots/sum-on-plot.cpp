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
    switch(_plotType)
    {
        case Categories:
        {
            categoriesPlot();
            break;
        }
        case Stores:
        {
            storesPlot();
            break;
        }
    }
}

double SumOnPlot::exchangeRate(QString currency, QDate date)
{
    auto exchangeRate = DB::select(Queries::exchangeRate.arg(currency));

    double course = 1.0;
    while(exchangeRate->next())
    {
        QDate currentDate = exchangeRate->value(2).toDate();
        if(date < currentDate)
        {
            break;
        }

        course = exchangeRate->value(3).toFloat();
    }
    return course;
}

void SumOnPlot::categoriesPlot()
{
    auto categories = DB::select(Queries::categories);
    auto allRecords = DB::select(Queries::selectAll);

    _customPlot->clearPlottables();
    _customPlot->clearGraphs();
    _customPlot->clearItems();
    _customPlot->clearMask();
    _customPlot->clearFocus();

    _customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    // create empty bar chart objects:
    QCPBars *categoriesBar = new QCPBars(_customPlot->xAxis, _customPlot->yAxis);
    _customPlot->addPlottable(categoriesBar);
    categoriesBar->setName("Categories");

    // Add data:
    QVector<double> categoriesData;
    QVector<double> ticks;
    QVector<QString> labels;
    double minValue = std::numeric_limits<double>::max(),
           maxValue = std::numeric_limits<double>::min();

    int count = 0;
    while(categories->next())
    {
        QString category = categories->value(0).toString();
        labels << category;

        double sumByCategory = 0;
        allRecords->first();
        while(allRecords->next())
        {
            QString currentCategory = allRecords->value(8).toString();
            if(currentCategory == category)
            {
                QString currency = allRecords->value(5).toString();
                QDate date = allRecords->value(7).toDate();
                double purchaseCount = allRecords->value(3).toFloat(),
                       purchasePrice = allRecords->value(4).toFloat(),
                       course = exchangeRate(currency, date);
                sumByCategory += purchaseCount * purchasePrice * course;
            }
        }
        categoriesData << sumByCategory;
        ticks << ++count;
        minValue = qMin(minValue, sumByCategory);
        maxValue = qMax(maxValue, sumByCategory);
    }

    categoriesBar->setData(ticks, categoriesData);

    setupXAxis(ticks, labels);
    setupYAxis(0.0, maxValue);
    setupLegend();

    _customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _customPlot->replot();
}

void SumOnPlot::storesPlot()
{
    auto stores = DB::select(Queries::stores);
    auto allRecords = DB::select(Queries::selectAll);

    _customPlot->clearPlottables();
    _customPlot->clearGraphs();
    _customPlot->clearItems();
    _customPlot->clearMask();
    _customPlot->clearFocus();

    _customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    // create empty bar chart objects:
    QCPBars *storesBar = new QCPBars(_customPlot->xAxis, _customPlot->yAxis);
    _customPlot->addPlottable(storesBar);
    storesBar->setName("Categories");

    // Add data:
    QVector<double> categoriesData;
    QVector<double> ticks;
    QVector<QString> labels;
    double minValue = std::numeric_limits<double>::max(),
           maxValue = std::numeric_limits<double>::min();

    int count = 0;
    while(stores->next())
    {
        QString store = stores->value(0).toString();
        labels << store;

        double sumByCategory = 0;
        allRecords->first();
        while(allRecords->next())
        {
            QString currentStore = allRecords->value(2).toString();
            if(currentStore == store)
            {
                QString currency = allRecords->value(5).toString();
                QDate date = allRecords->value(7).toDate();
                double purchaseCount = allRecords->value(3).toFloat(),
                       purchasePrice = allRecords->value(4).toFloat(),
                       course = exchangeRate(currency, date);
                sumByCategory += purchaseCount * purchasePrice * course;
            }
        }
        categoriesData << sumByCategory;
        ticks << ++count;
        minValue = qMin(minValue, sumByCategory);
        maxValue = qMax(maxValue, sumByCategory);
    }

    storesBar->setData(ticks, categoriesData);

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
