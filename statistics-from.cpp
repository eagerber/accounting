#include "statistics-from.h"
#include "ui_statisticsfrom.h"

#include "qcustomplot.h"
#include "db-constdef.h"

#include "plots/consumption-by-month.h"
#include "plots/sum-on-categories-plot.h"

StatisticsFrom::StatisticsFrom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsFrom)
{
    ui->setupUi(this);
}

StatisticsFrom::~StatisticsFrom()
{
    delete ui;
}

void StatisticsFrom::on_sumOnCategoryButton_clicked()
{
    SumOnCategoriesPlot plot(ui->widget);
    plot.replot();
}

void StatisticsFrom::on_consumptionByMonthPushButton_clicked()
{
    ConsumptionByMonth plot(ui->widget);
    plot.replot();

}
