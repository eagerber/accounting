#include "statistics-from.h"
#include "ui_statisticsfrom.h"

#include "qcustomplot.h"
#include "db-constdef.h"

#include "plots/consumption-by-month.h"
#include "plots/sum-on-plot.h"



StatisticsFrom::StatisticsFrom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticsFrom),
    tableViewModel(nullptr)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = Qt::Window   |
            Qt::WindowSystemMenuHint     |
            Qt::WindowMinimizeButtonHint |
            Qt::WindowMaximizeButtonHint |
            Qt::WindowCloseButtonHint;

    this->setWindowFlags(flags);
}

StatisticsFrom::~StatisticsFrom()
{
    delete ui;
}

void StatisticsFrom::on_sumOnCategoryButton_clicked()
{
    SumOnPlot plot(ui->widget, SumOnPlot::PlotType::Categories);
    plot.replot();
}

void StatisticsFrom::on_sumOnStoresButton_clicked()
{
    SumOnPlot plot(ui->widget, SumOnPlot::PlotType::Stores);
    plot.replot();
}

void StatisticsFrom::on_consumptionByMonthPushButton_clicked()
{
    ConsumptionByMonth plot(ui->widget);
    plot.replot();
}

void StatisticsFrom::on_accumulateSumByDatePushButton_clicked()
{
    updateTableView();
}

void StatisticsFrom::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);

    resizeTableView();
}

void StatisticsFrom::updateTableView()
{
    double maxTotal = ui->maxTotalLineEdit->text().toDouble();

    QSharedPointer<QSqlQuery> query;
    if(ui->purchaseRadioButton->isChecked())
    {
        query = DB::select(Queries::accumulatedSumByID(maxTotal));
    }
    else if(ui->categoryRadioButton->isChecked())
    {
        query = DB::select(Queries::groupAccumulatedSum(maxTotal));
    }
    qDebug() << Queries::groupAccumulatedSum(maxTotal);

    delete tableViewModel;
    tableViewModel = new QStandardItemModel;

    QStandardItem *item;

    //Заголовки столбцов
    QStringList horizontalHeader;
    horizontalHeader.append("ID");
    horizontalHeader.append("Product");
    horizontalHeader.append("Store");
    horizontalHeader.append("Count");
    horizontalHeader.append("Price");
    horizontalHeader.append("Date");
    horizontalHeader.append("Accumulated Total");

    tableViewModel->setHorizontalHeaderLabels(horizontalHeader);

    int index = 0;
    while(query->next())
    {
        QStringList verticalHeader;
        verticalHeader.append("index");
        item = new QStandardItem(query->value(0).toString());
        tableViewModel->setItem(index, 0, item);
        item = new QStandardItem(query->value(1).toString());
        tableViewModel->setItem(index, 1, item);
        item = new QStandardItem(query->value(2).toString());
        tableViewModel->setItem(index, 2, item);
        item = new QStandardItem(query->value(3).toString());
        tableViewModel->setItem(index, 3, item);
        item = new QStandardItem(query->value(4).toString());
        tableViewModel->setItem(index, 4, item);
        item = new QStandardItem(query->value(7).toString());
        tableViewModel->setItem(index, 5, item);
        if(ui->purchaseRadioButton->isChecked())
        {
            item = new QStandardItem(query->value(9).toString());
        }
        else if(ui->categoryRadioButton->isChecked())
        {
            item = new QStandardItem(query->value(10).toString());
        }
        tableViewModel->setItem(index, 6, item);

        index++;
    }

    ui->tableView->setModel(tableViewModel);

    ui->tableView->verticalHeader()->hide();
    resizeTableView();
}

void StatisticsFrom::resizeTableView()
{
    double width = ui->tableView->width() / 100.0f - 0.2;
    ui->tableView->setColumnWidth(0, round(width * 4));
    ui->tableView->setColumnWidth(1, round(width * 20));
    ui->tableView->setColumnWidth(2, round(width * 20));
    ui->tableView->setColumnWidth(3, round(width * 10));
    ui->tableView->setColumnWidth(4, round(width * 10));
    ui->tableView->setColumnWidth(5, round(width * 16));
    ui->tableView->setColumnWidth(6, round(width * 20));
}
