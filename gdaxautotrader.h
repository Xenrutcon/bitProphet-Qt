#ifndef GDAXAUTOTRADER_H
#define GDAXAUTOTRADER_H

#include <QObject>
#include "bitprophet.h"

class bitProphet;

class gdaxAutoTrader : public QObject {
    Q_OBJECT
public:
    explicit gdaxAutoTrader(bitProphet *parent);
    ~gdaxAutoTrader();
    void say(QString sayThis);
    void sayGdaxAutoTrader(QString sayThis,QString whichBox="ALL");
    QString findHighestValue(QList<QString> hayStack);
    QString findLowestValue(QList<QString> hayStack);
    QTextEdit *mBTCLog;
    QTextEdit *mLTCLog;
    QTextEdit *mETHLog;
    QList<QString> mTradeTypes; //actively sell these coins
    QList<QString> mBuyTypes; //actively buy these coins
    QString mUSDStartAmount;
    double mMinUSDBuyAmount;
    double mMinPercentProfit; //DECIMAL VERSION -> 1% = 0.01
    QString mLastBuyPriceBTC;
    QString mLastBuyPriceLTC;
    QString mLastBuyPriceETH;
    QString mLastBuyTimeBTC;
    QString mLastBuyTimeLTC;
    QString mLastBuyTimeETH;
    QString mLastSellPriceBTC;
    QString mLastSellPriceLTC;
    QString mLastSellPriceETH;
    QString mLastSellTimeBTC;
    QString mLastSellTimeLTC;
    QString mLastSellTimeETH;
    double mLastSellExpireAfter;
    double mLastBuyExpireAfter;
private:
    bitProphet *mParent;
private slots:
    void autoTradeCheck();
    void checkForBuyFills();
    void checkForSellFills();
};

#endif // GDAXAUTOTRADER_H
