#include "statisticsfrom.h"
#include "ui_statisticsfrom.h"

#include "qcustomplot.h"
#include "db-constdef.h"

#include "sumoncategoriesplot.h"
#include "consumptionbymonth.h"

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

void StatisticsFrom::on_sumOnCategoryButton_clicked()
{
    SumOnCategoriesPlot plot(_db, ui->widget);
    plot.replot();
}

void StatisticsFrom::on_consumptionByMonthPushButton_clicked()
{
    ConsumptionByMonth plot(_db, ui->widget);
    plot.replot();

}
