#include "consumption-by-month.h"

#include "db-constdef.h"


ConsumptionByMonth::ConsumptionByMonth(DB &db, QCustomPlot *customPlot)
    : _db(db)
{
    _customPlot = customPlot;
}

void ConsumptionByMonth::replot()
{
    auto query = _db.select(Queries::consumptionByMonth);

    _customPlot->clearPlottables();
    _customPlot->clearGraphs();
    _customPlot->clearItems();
    _customPlot->clearMask();
    _customPlot->clearFocus();

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

    _customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    _customPlot->addGraph();
    QPen pen;
    pen.setColor(QColor(0, 0, 255, 200));
    _customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    _customPlot->graph()->setPen(pen);
    _customPlot->graph()->setBrush(QBrush(QColor(255,160,50,150)));
    _customPlot->graph()->setData(time, consumptionData);

    _customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    _customPlot->xAxis->setDateTimeFormat("MMMM\nyyyy");
    _customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    _customPlot->xAxis->setTickLabelRotation(60);
    _customPlot->xAxis->setTickStep(2628000);
    _customPlot->xAxis->setAutoTickStep(false);
    _customPlot->xAxis->setSubTickCount(3);
    _customPlot->xAxis->setRange(minTime, maxTime);

    _customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    _customPlot->yAxis->setAutoTicks(false);
    _customPlot->yAxis->setAutoTickLabels(false);
    _customPlot->yAxis->setTickVector(QVector<double>() << minValue << maxValue - minValue);
    _customPlot->yAxis->setTickVectorLabels(QVector<QString>() << "Not so\nhigh" << "Very\nhigh");
    _customPlot->yAxis->setLabel("Consumptions By Month");
    _customPlot->xAxis2->setVisible(true);
    _customPlot->yAxis2->setVisible(true);
    _customPlot->xAxis2->setTicks(false);
    _customPlot->yAxis2->setTicks(false);
    _customPlot->xAxis2->setTickLabels(false);
    _customPlot->yAxis2->setTickLabels(false);
    _customPlot->yAxis->setRange(minValue, maxValue);

    setupLegend();

    _customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _customPlot->replot();
}

void ConsumptionByMonth::setupLegend()
{
    _customPlot->legend->setVisible(false);
}
