#ifndef COINBASEACCOUNT_H
#define COINBASEACCOUNT_H

#include <QObject>
#include <QString>
#include "bitprophet.h"
//#include "coinbasewallet.h"
//#include "coinbasepaymentmethod.h"

class bitProphet;
//class coinbaseWallet;
//class coinbasePaymentMethod;

class coinbaseAccount : public QObject {
    Q_OBJECT
public:
    explicit coinbaseAccount(QObject *parent);
    ~coinbaseAccount();
    QString mPtrName;
//    int addWallet();
//    int addPaymentMethod();
//    coinbaseWallet* getWallet(int index);
//    void clearWallets();
//    int getWalletCount();
//    int getPaymentMethodCount();
//    coinbasePaymentMethod* getPaymentMethod(int index);
private:
    void say(QString sayThis, bool debug=0);
    bitProphet *mParent;
//    QList<coinbaseWallet*> mWalletList;
//    QList<coinbasePaymentMethod*> mPayMethods;
signals:

public slots:
};

#endif // COINBASEACCOUNT_H
