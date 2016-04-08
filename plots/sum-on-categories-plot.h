#ifndef SUMONCATEGORIESPLOT_H
#define SUMONCATEGORIESPLOT_H

#include <QVector>


class QCustomPlot;

class SumOnCategoriesPlot
{
public:
    explicit SumOnCategoriesPlot(QCustomPlot *customPlot);
    ~SumOnCategoriesPlot() = default;

    void replot();

private:
    SumOnCategoriesPlot() = default;
    void setupXAxis(QVector<double> ticks, QVector<QString> labels);
    void setupYAxis(double minValue, double maxValue);
    void setupLegend();

    QCustomPlot *_customPlot;
};

#endif // SUMONCATEGORIESPLOT_H
