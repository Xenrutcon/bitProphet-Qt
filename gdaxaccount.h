#ifndef GDAXACCOUNT_H
#define GDAXACCOUNT_H

#include <QObject>


#include "gdaxapihandler.h"
#include "gdaxwallet.h"
class gdaxApiHandler;
class gdaxWallet;


class gdaxAccount : public QObject {
    Q_OBJECT
public:
    explicit gdaxAccount(gdaxApiHandler *parent);
    ~gdaxAccount();
    QString mId;
    QString mName;
    QString mExchange;
    QString mApiKey;
    QString mApiSecret;
    QString mPassPhrase;
    bool mDefaultAccount;
    void say(QString sayThis);
    int addWallet();
    gdaxWallet* getWallet(int index);
    int getWalletCount();
    void clearWallets();
private:
    QString mPtrName;
    gdaxApiHandler *mParent;
    QList<gdaxWallet*> mWalletList;
signals:        
public slots:
};

#endif // GDAXACCOUNT_H
