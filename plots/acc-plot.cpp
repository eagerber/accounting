#include "acc-plot.h"

#include "qcustomplot.h"
#include "database/db.h"

#include "queries.h"


AccPlot::AccPlot(QCustomPlot *customPlot)
{
    _customPlot = customPlot;
}

void AccPlot::setupXAxis(double minValue, double maxValue)
{
    _customPlot->xAxis->setTickLabelRotation(60);
    _customPlot->xAxis->grid()->setVisible(true);
    _customPlot->xAxis->setRange(minValue, maxValue);
}

void AccPlot::setupYAxis(double minValue, double maxValue)
{
    _customPlot->yAxis->setRange(minValue, maxValue);
    _customPlot->yAxis->setPadding(5); // a bit more space to the left border
    _customPlot->yAxis->setLabel("Sum on Categories");

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    _customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    _customPlot->yAxis->grid()->setSubGridPen(gridPen);
}

void AccPlot::setupLegend()
{
    _customPlot->legend->setVisible(false);
}

double AccPlot::exchangeRate(QString currency, QDate date)
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
