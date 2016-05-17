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

    auto allRecords = DB::select(Queries::selectAll);
    switch(_plotType)
    {
        case Categories:
        {
            categoriesPlot(allRecords);
            break;
        }
        case Stores:
        {
            storesPlot(allRecords);
            break;
        }
    }

    _customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _customPlot->replot();
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

void SumOnPlot::categoriesPlot(QSharedPointer<QSqlQuery> allRecords)
{
    auto categories = DB::select(Queries::categories);

    QMap<QString, double> categoriesData;
    QVector<QString> labels;
    QVector<double> ticks;
    int count = 0;
    while(categories->next())
    {
        QString category = categories->value(0).toString();
        labels << category;
        categoriesData[category] = 0.0;
        ticks << ++count;
    }

    // create empty bar chart objects:
    QCPBars *categoriesBar = new QCPBars(_customPlot->xAxis, _customPlot->yAxis);
    _customPlot->addPlottable(categoriesBar);
    categoriesBar->setName("Categories");

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

    double minValue = std::numeric_limits<double>::max(),
           maxValue = std::numeric_limits<double>::min();
    for(const auto item : categoriesData.values())
    {
        minValue = qMin(minValue, item);
        maxValue = qMax(maxValue, item);
    }

    categoriesBar->setData(ticks, categoriesData.values().toVector());

    setupXAxis(ticks, labels);
    setupYAxis(0.0, maxValue);
    setupLegend();
}

void SumOnPlot::storesPlot(QSharedPointer<QSqlQuery> allRecords)
{
    auto stores = DB::select(Queries::stores);

    QMap<QString, double> storesData;
    QVector<QString> labels;
    QVector<double> ticks;
    int count = 0;
    while(stores->next())
    {
        QString store = stores->value(0).toString();
        labels << store;
        storesData[store] = 0.0;
        ticks << ++count;
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

    double minValue = std::numeric_limits<double>::max(),
           maxValue = std::numeric_limits<double>::min();
    for(const auto item : storesData.values())
    {
        minValue = qMin(minValue, item);
        maxValue = qMax(maxValue, item);
    }

    storesBar->setData(ticks, storesData.values().toVector());

    setupXAxis(ticks, labels);
    setupYAxis(0.0, maxValue);
    setupLegend();
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
