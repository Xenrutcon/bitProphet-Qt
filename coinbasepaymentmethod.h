#ifndef COINBASEPAYMENTMETHOD_H
#define COINBASEPAYMENTMETHOD_H

#include <QObject>
#include "coinbaseaccount.h"

class coinbaseAccount;

class coinbasePaymentMethod : public QObject {
    Q_OBJECT
public:
    explicit coinbasePaymentMethod(coinbaseAccount *parent);
    ~coinbasePaymentMethod();
    QString mId;
    QString mType;
    QString mName;
    QString mCurrency;
    bool mPrimaryBuy;
    bool mPrimarySell;
    bool mAllowBuy;
    bool mAllowSell;
    bool mAllowWithdraw;
    bool mInstantBuy;
    bool mInstantSell;
    QString mCreated;
    QString mUpdated;
    QString mResource;
    QString mResourcePath;
    QString mFiatAccountId;
    QString mFiatAccountResource;
    QString mFiatAccountResourcePath;
private:
    coinbaseAccount *mParent;
signals:

public slots:
};

#endif // COINBASEPAYMENTMETHOD_H


//Example from API Docs
//"data": [
//    {
//      "id": "1276ec",
//      "type": "fiat_account",
//      "name": "USD Wallet",
//      "currency": "USD",
//      "primary_buy": false,
//      "primary_sell": false,
//      "allow_buy": true,
//      "allow_sell": true,
//      "allow_deposit": true,
//      "allow_withdraw": true,
//      "instant_buy": true,
//      "instant_sell": true,
//      "created_at": "2015-02-24T14:30:30-08:00",
//      "updated_at": "2015-02-24T14:30:30-08:00",
//      "resource": "payment_method",
//      "resource_path": "/v2/payment-methods/1276ec",
//      "fiat_account": {
//          "id": "388",
//          "resource": "account",
//          "resource_path": "/v2/accounts/27388"
//      }
//    },
