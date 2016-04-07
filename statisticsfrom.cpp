#include "statisticsfrom.h"
#include "ui_statisticsfrom.h"

#include "qcustomplot.h"
#include "db-constdef.h"


StatisticsFrom::StatisticsFrom(DB &db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsFrom),
    _db(db)
{
    ui->setupUi(this);
}

StatisticsFrom::~StatisticsFrom()
{
    delete ui;
}

void StatisticsFrom::updatePlot()
{

}

void StatisticsFrom::on_showPlotButton_clicked()
{
    QString queryString = Queries::sumOnCategories;
    auto query = _db.select(queryString);

    auto &customPlot = ui->widget;
    customPlot->clearPlottables();

    // create empty bar chart objects:
    QCPBars *categories = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(categories);
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

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->replot();
}

void StatisticsFrom::on_pushButton_clicked()
{
    auto query = _db.select(Queries::consumptionByMonth);

    auto &customPlot = ui->widget;
    customPlot->clearPlottables();

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

    customPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

    customPlot->addGraph();
    QPen pen;
    pen.setColor(QColor(0, 0, 255, 200));
    customPlot->graph()->setLineStyle(QCPGraph::lsLine);
    customPlot->graph()->setPen(pen);
    customPlot->graph()->setBrush(QBrush(QColor(255,160,50,150)));

    customPlot->graph()->setData(time, consumptionData);

    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("MMMM\nyyyy");
    customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setTickStep(2628000);
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setSubTickCount(3);
    customPlot->xAxis->setLabel("Date");
    customPlot->xAxis->setRange(minTime, maxTime);

    customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    customPlot->yAxis->setAutoTicks(false);
    customPlot->yAxis->setAutoTickLabels(false);
    customPlot->yAxis->setTickVector(QVector<double>() << minValue << maxValue - minValue);
    customPlot->yAxis->setTickVectorLabels(QVector<QString>() << "Not so\nhigh" << "Very\nhigh");
    customPlot->yAxis->setLabel("Consumptions By Month");
    customPlot->xAxis2->setVisible(true);
    customPlot->yAxis2->setVisible(true);
    customPlot->xAxis2->setTicks(false);
    customPlot->yAxis2->setTicks(false);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setTickLabels(false);
    customPlot->yAxis->setRange(minValue, maxValue);

    setupLegend();

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->replot();
}

void StatisticsFrom::setupXAxis(QVector<double> ticks, QVector<QString> labels)
{
    auto &customPlot = ui->widget;

    customPlot->xAxis->setAutoTicks(false);
    customPlot->xAxis->setAutoTickLabels(false);
    customPlot->xAxis->setTickVector(ticks);
    customPlot->xAxis->setTickVectorLabels(labels);
    customPlot->xAxis->setTickLabelRotation(60);
    customPlot->xAxis->setSubTickCount(0);
    customPlot->xAxis->setTickLength(0, 5);
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->xAxis->setRange(0, ticks.size() + 1);
}

void StatisticsFrom::setupYAxis(double minValue, double maxValue)
{
    auto &customPlot = ui->widget;

    customPlot->yAxis->setRange(minValue, maxValue);
    customPlot->yAxis->setPadding(5); // a bit more space to the left border
    customPlot->yAxis->setLabel("Sum on Categories");
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    customPlot->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    customPlot->yAxis->grid()->setSubGridPen(gridPen);
}

void StatisticsFrom::setupLegend()
{
    auto &customPlot = ui->widget;
    customPlot->legend->setVisible(false);
}
