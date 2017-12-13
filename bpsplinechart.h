#ifndef BPSPLINECHART_H
#define BPSPLINECHART_H

#include <QWidget>
#include <QTabWidget>
#include <QtCharts>
#include "bpdatabase.h"


class bpDatabase;

using namespace QtCharts;
class bpSplineChart : public QWidget
{
    Q_OBJECT
public:
    explicit bpSplineChart(QWidget *parent,QString title);
    ~bpSplineChart();
    void say(QString sayThis);
    QSplineSeries *mSeries;
    QChart *mChart;
    QChartView *mView;
    void addBtcSpotPriceHistoryData(bpDatabase *theDb);
    void reloadBtcSpotPriceHistoryData(bpDatabase *theDb);
    void reloadLtcSpotPriceHistoryData(bpDatabase *theDb);
    void reloadEthSpotPriceHistoryData(bpDatabase *theDb);
private:
    QWidget *mParent;
signals:

public slots:
};

#endif // BPSPLINECHART_H
