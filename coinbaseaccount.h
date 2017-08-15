#ifndef COINBASEACCOUNT_H
#define COINBASEACCOUNT_H

#include <QObject>
#include <QString>
#include "bitprophet.h"
#include "cbapihandler.h"
//#include "coinbasewallet.h"
//#include "coinbasepaymentmethod.h"

class bitProphet;
class cbApiHandler;
//class coinbaseWallet;
//class coinbasePaymentMethod;

class coinbaseAccount : public QObject {
    Q_OBJECT
public:
    explicit coinbaseAccount(QObject *parent);
    ~coinbaseAccount();
    QString mPtrName;
    QString mId;
    QString mName;
    QString mExchange;
    QString mApiKey;
    QString mApiSecret;
    bool mDefaultAccount;
//    int addWallet();
//    int addPaymentMethod();
//    coinbaseWallet* getWallet(int index);
//    void clearWallets();
//    int getWalletCount();
//    int getPaymentMethodCount();
//    coinbasePaymentMethod* getPaymentMethod(int index);
private:
    void say(QString sayThis);
    cbApiHandler *mParent;
//    QList<coinbaseWallet*> mWalletList;
//    QList<coinbasePaymentMethod*> mPayMethods;
signals:

public slots:
};

#endif // COINBASEACCOUNT_H
