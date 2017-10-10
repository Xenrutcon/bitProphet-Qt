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
    QTextEdit *mBTCLog;
    QTextEdit *mLTCLog;
    QTextEdit *mETHLog;
    QList<QString> mTradeTypes; //actively sell these coins
    QList<QString> mBuyTypes; //actively buy these coins
    QString mUSDStartAmount;
    double mMinUSDBuyAmount;
    double mMinPercentProfit; //DECIMAL VERSION -> 1% = 0.01
private:
    bitProphet *mParent;
private slots:
    void autoTradeCheck();
};

#endif // GDAXAUTOTRADER_H
