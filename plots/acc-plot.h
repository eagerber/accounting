#ifndef ACCPLOT_H
#define ACCPLOT_H

class QCustomPlot;

class AccPlot
{
public:
    virtual void replot() = 0;

protected:
    explicit AccPlot(QCustomPlot *customPlot);
    ~AccPlot() = default;

    void setupXAxis(double minValue, double maxValue);
    void setupYAxis(double minValue, double maxValue);
    void setupLegend();

    QCustomPlot *_customPlot;
};

#endif // ACCPLOT_H
