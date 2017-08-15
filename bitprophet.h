#ifndef BITPROPHET_H
#define BITPROPHET_H

#include <iostream>
#include <QObject>
#include "bpwindow.h"
#include "bpdatabase.h"
#include "coinbaseaccount.h"
#include "cbapihandler.h"

class bpWindow;
class bpDatabase;
class coinbaseAccount;
class cbApiHandler;

class bitProphet : public QObject {
    Q_OBJECT    
public:
    explicit bitProphet(QObject *parent);
    ~bitProphet();    
    QString mPtrName;
    QString mState;
    bpDatabase *getDb();
    void setProphetState(QString newState);
    void say(QString sayThis, bool debug=0);
private:
    bpWindow *mParent;
    bpDatabase *mDb;
    cbApiHandler *mApiHandler;
    //Account Refresh
    bool mAutoRefreshAccount;
    int mAutoRefreshAccountInterval; //all intervals in milliseconds
    //Spot Prices (ie: not gdax market price, coinbase spot price for coinbase buy/sell (stupid, dont use it except for quick reference) )
    bool mAutoCheckSpotPrices;
    int mAutoCheckSpotPricesInterval;
    //Simple Trading (non-neural net, simple dumb logix)
    bool mAutoSimpleTrade;
    // TODO add autoSimpleTrading (have done it before :P, its slower but works )


signals:

public slots:


};

#endif // BITPROPHET_H
