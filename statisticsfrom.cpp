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
    auto &customPlot = ui->widget;
    customPlot->clearPlottables();

    QVector<double> datax = QVector<double>() << 1 << 2 << 3 << 4;
    QVector<double> datay1 = QVector<double>() << 0.6 << 0.5 << 0.3 << 0.15;
    QVector<double> datay2 = QVector<double>() << 0.3 << 0.28 << 0.2 << 0.1;
    QVector<double> datay3 = QVector<double>() << 0.33 << 0.31 << 0.27 << 0.13;

    QCPBarsGroup *group1 = new QCPBarsGroup(customPlot);
    QCPBars *bars;
    bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(bars);
    bars->setData(datax, datay1);
    bars->setBrush(QColor(0, 0, 255, 50));
    bars->setPen(QColor(0, 0, 255));
    bars->setWidth(0.15);
    bars->setBarsGroup(group1);
    bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(bars);
    bars->setData(datax, datay2);
    bars->setBrush(QColor(180, 00, 120, 50));
    bars->setPen(QColor(180, 00, 120));
    bars->setWidth(0.15);
    bars->setBarsGroup(group1);
    bars = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(bars);
    bars->setData(datax, datay3);
    bars->setBrush(QColor(255, 154, 0, 50));
    bars->setPen(QColor(255, 154, 0));
    bars->setWidth(0.15);
    bars->setBarsGroup(group1);

    customPlot->xAxis->setRange(0.1, 4.9);
    customPlot->yAxis->setRange(0, 0.7);
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(1);

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
