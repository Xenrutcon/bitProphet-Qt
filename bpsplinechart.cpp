#include "bpsplinechart.h"
#include <iostream>
bpSplineChart::bpSplineChart(QWidget *parent,QString title) : QWidget(parent), mSeries(NULL),mChart(NULL),mView(NULL) {
    mSeries = new QSplineSeries(this);
    mView = new QChartView(parent);
    mChart = new QChart();
    mChart->legend()->hide();
    mChart->addSeries(mSeries);
    mChart->createDefaultAxes();
    mView->setRenderHint(QPainter::Antialiasing);
    mView->setChart(mChart);
    mChart->setTitle(title);
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

void bpSplineChart::addBtcSpotPriceHistoryData(bpDatabase *theDb) {
    say("Adding Btc Spot Price To History...");
    mSeries->clear();
    mChart->removeSeries(mSeries);
    theDb->getBtcSpotPriceHistoryLast(1200,this);
    mChart->addSeries(mSeries);
    mChart->createDefaultAxes();
    mView->repaint();
}

void bpSplineChart::reloadBtcSpotPriceHistoryData(bpDatabase *theDb) {
    mSeries->clear();
    mChart->removeSeries(mSeries);
    theDb->getBtcSpotPriceHistoryLast(1200,this);
    mChart->addSeries(mSeries);
    mChart->createDefaultAxes();
    mView->repaint();
}

void bpSplineChart::reloadLtcSpotPriceHistoryData(bpDatabase *theDb) {
    mSeries->clear();
    mChart->removeSeries(mSeries);
    theDb->getLtcSpotPriceHistoryLast(1200,this);
    mChart->addSeries(mSeries);
    mChart->createDefaultAxes();
    mView->repaint();
}

void bpSplineChart::reloadEthSpotPriceHistoryData(bpDatabase *theDb) {
    mSeries->clear();
    mChart->removeSeries(mSeries);
    theDb->getEthSpotPriceHistoryLast(1200,this);
    mChart->addSeries(mSeries);
    mChart->createDefaultAxes();
    mView->repaint();
}
