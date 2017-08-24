#ifndef CBAUTOSPOTTRADER_H
#define CBAUTOSPOTTRADER_H

#include <iostream>
#include <QObject>
#include <QString>
#include "bitprophet.h"
#include "cbapihandler.h"
#include "spotprice.h"

class bitProphet;
class cbApiHandler;

class cbAutoSpotTrader : public QObject {
    Q_OBJECT
public:
    explicit cbAutoSpotTrader(bitProphet *parent);
    ~cbAutoSpotTrader();
    bitProphet *mParent;
    void say(QString sayIt, QString coinLog="ALL");
    QTextEdit *mBTCLog;
    QTextEdit *mLTCLog;
    QTextEdit *mETHLog;
    QList<QString> mTradeTypes;
    QString findLowestPrice(QList<QString> hayStack);
    QString findHighestPrice(QList<QString> hayStack);
    QString findPriceGap(QList<QString> hayStack);
    void checkAutoBuysForProfit(QString coin);
signals:

public slots:
    void autoTradeCheck();
};

#endif // CBAUTOSPOTTRADER_H
