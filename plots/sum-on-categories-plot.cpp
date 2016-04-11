#include "sum-on-categories-plot.h"

#include "qcustomplot.h"
#include "db-constdef.h"
#include "db.h"


SumOnCategoriesPlot::SumOnCategoriesPlot(QCustomPlot *customPlot)
{
    _customPlot = customPlot;
}

void SumOnCategoriesPlot::replot()
{
    QString queryString = Queries::sumOnCategories;
    auto query = DB::select(queryString);

    _customPlot->clearPlottables();    

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
    setupYAxis(minValue, maxValue);
    setupLegend();

    _customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _customPlot->replot();
}

void SumOnCategoriesPlot::setupXAxis(QVector<double> ticks, QVector<QString> labels)
{
    _customPlot->xAxis->setTickLabelType(QCPAxis::ltNumber);

    _customPlot->xAxis->setAutoTicks(false);
    _customPlot->xAxis->setAutoTickLabels(false);
    _customPlot->xAxis->setTickVector(ticks);
    _customPlot->xAxis->setTickVectorLabels(labels);
    _customPlot->xAxis->setTickLabelRotation(60);
    _customPlot->xAxis->setSubTickCount(0);
    _customPlot->xAxis->setTickLength(0, 5);

    _customPlot->xAxis->grid()->setVisible(true);
    _customPlot->xAxis->setRange(0, ticks.size() + 1);
}

void SumOnCategoriesPlot::setupYAxis(double minValue, double maxValue)
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

void SumOnCategoriesPlot::setupLegend()
{
    _customPlot->legend->setVisible(false);
}
