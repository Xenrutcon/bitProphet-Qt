#include "bpsplinechart.h"
#include <iostream>
bpSplineChart::bpSplineChart(QWidget *parent) : mParent(parent), mSeries(NULL),mChart(NULL),mView(NULL) {
    mSeries = new QSplineSeries();
//    mSeries2 = new QSplineSeries();
//    mSeries3 = new QSplineSeries(this);
    mView = new QChartView(mParent);
    mChart = new QChart();
    say("Spline Chart Created!");
}

bpSplineChart::~bpSplineChart() {
    mView->deleteLater();
    mChart->deleteLater();
    mSeries->deleteLater();
//    mSeries2->deleteLater();
//    mSeries3->deleteLater();
    say("Spline Chart Destroyed!");
}

void bpSplineChart::say(QString sayThis) {
    std::cout<<sayThis.toStdString().c_str()<<std::endl;
}
