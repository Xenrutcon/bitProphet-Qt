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
    explicit bpSplineChart(QWidget *parent);
    ~bpSplineChart();
    void say(QString sayThis);
    QSplineSeries *mSeries;
    QSplineSeries *mSeriesLtc;
    QSplineSeries *mSeriesEth;
    QChart *mChart;
    QChartView *mView;
private:
    QWidget *mParent;
signals:

public slots:
};

#endif // BPSPLINECHART_H
