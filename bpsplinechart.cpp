#include "bpsplinechart.h"
#include <iostream>
bpSplineChart::bpSplineChart(QWidget *parent) : QWidget(parent), mSeries(NULL),mChart(NULL),mView(NULL) {
    mSeries = new QSplineSeries();
    mSeriesLtc = new QSplineSeries();
    mSeriesEth = new QSplineSeries();
    mView = new QChartView(parent);
    mChart = new QChart();
    say("Spline Chart Created!");
}

bpSplineChart::~bpSplineChart() {
    mView->deleteLater();
    mChart->deleteLater();
    mSeries->deleteLater();
    say("Spline Chart Destroyed!");
}

void bpSplineChart::say(QString sayThis) {
    std::cout<<sayThis.toStdString().c_str()<<std::endl;
}
