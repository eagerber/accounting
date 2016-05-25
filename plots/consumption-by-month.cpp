#include "consumption-by-month.h"

#include "database/db.h"

#include "qcustomplot.h"
#include "queries.h"


ConsumptionByMonth::ConsumptionByMonth(QCustomPlot *customPlot)
    : AccPlot(customPlot)
{
}

void ConsumptionByMonth::replot()
{
    auto query = DB::select(Queries::consumptionByMonthWithoutGroup);

    _customPlot->clearPlottables();
    _customPlot->clearGraphs();
    _customPlot->clearItems();
    _customPlot->clearMask();
    _customPlot->clearFocus();

    _customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    // Add data:
    QVector<double> consumptionData;
    QVector<double> ticks;
    QVector<QString> labels;
    QVector<double> time;
    double minValue = std::numeric_limits<double>::max(),
           maxValue = std::numeric_limits<double>::min(),
           minTime  = std::numeric_limits<double>::max(),
           maxTime  = std::numeric_limits<double>::min();

    int count = 0;
    while(query->next())
    {
        double value = query->value(0).toDouble();
        consumptionData << value;
        minValue = qMin(minValue, value);
        maxValue = qMax(maxValue, value);

        QString dateString = query->value(1).toString() + "-" + query->value(2).toString();
        labels << dateString;
        double currentTime = QDateTime::fromString(dateString, "yyyy-MM").toTime_t();
        time << currentTime;

        minTime = qMin(minTime, currentTime);
        maxTime = qMax(maxTime, currentTime);
        ticks << ++count;
    }

    _customPlot->addGraph();
    QPen pen;
    pen.setColor(QColor(0, 0, 255, 200));
    _customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    _customPlot->graph()->setPen(pen);
    _customPlot->graph()->setBrush(QBrush(QColor(255,160,50,150)));
    _customPlot->graph()->setData(time, consumptionData);

    setupXAxis(minTime, maxTime);
    setupYAxis(minValue, maxValue);

    setupLegend();

    _customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _customPlot->replot();
}
/*
void ConsumptionByMonth::replot()
{
    auto query = DB::select(Queries::consumptionByMonth);

    _customPlot->clearPlottables();
    _customPlot->clearGraphs();
    _customPlot->clearItems();
    _customPlot->clearMask();
    _customPlot->clearFocus();

    _customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    // Add data:
    QVector<double> consumptionData;
    QVector<double> ticks;
    QVector<QString> labels;
    QVector<double> time;
    double minValue = std::numeric_limits<double>::max(),
           maxValue = std::numeric_limits<double>::min(),
           minTime  = std::numeric_limits<double>::max(),
           maxTime  = std::numeric_limits<double>::min();

    int count = 0;
    while(query->next())
    {
        double value = query->value(0).toDouble();
        consumptionData << value;
        minValue = qMin(minValue, value);
        maxValue = qMax(maxValue, value);

        QString dateString = query->value(1).toString() + "-" + query->value(2).toString();
        labels << dateString;
        double currentTime = QDateTime::fromString(dateString, "yyyy-MM").toTime_t();
        time << currentTime;

        minTime = qMin(minTime, currentTime);
        maxTime = qMax(maxTime, currentTime);
        ticks << ++count;
    }

    _customPlot->addGraph();
    QPen pen;
    pen.setColor(QColor(0, 0, 255, 200));
    _customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    _customPlot->graph()->setPen(pen);
    _customPlot->graph()->setBrush(QBrush(QColor(255,160,50,150)));
    _customPlot->graph()->setData(time, consumptionData);

    setupXAxis(minTime, maxTime);
    setupYAxis(minValue, maxValue);

    setupLegend();

    _customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _customPlot->replot();
}
*/
void ConsumptionByMonth::setupXAxis(double minTime, double maxTime)
{
    _customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    _customPlot->xAxis->setDateTimeFormat("MMMM\nyyyy");

    _customPlot->xAxis->setAutoTicks(true);
    _customPlot->xAxis->setAutoTickLabels(true);
    _customPlot->xAxis->setAutoTickStep(true);

    _customPlot->xAxis->setTickStep(2628000);
    _customPlot->xAxis->setSubTickCount(3);

    AccPlot::setupXAxis(minTime, maxTime);
}
