#ifndef BPSPLINECHART_H
#define BPSPLINECHART_H

#include <QWidget>
#include <QTabWidget>
#include <QtCharts>
using namespace QtCharts;
class bpSplineChart : public QWidget
{
    Q_OBJECT
public:
    explicit bpSplineChart(QTabWidget *parent);
    ~bpSplineChart();
    void say(QString sayThis);
private:
    QTabWidget *mParent;
    QSplineSeries *mSeries;
    QChart *mChart;
    QChartView *mView;


signals:

public slots:
};

#endif // BPSPLINECHART_H
