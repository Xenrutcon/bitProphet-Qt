#ifndef COINBASEWALLET_H
#define COINBASEWALLET_H

#include <QObject>
#include <QObject>
#include <QString>
#include "coinbaseaccount.h"

class coinbaseAccount;

class coinbaseWallet : public QObject {
    Q_OBJECT
public:
    explicit coinbaseWallet(coinbaseAccount *parent);
    QString mAmount;
    QString mAmountNative;
    QString mCreated;
    QString mCurrency;
    QString mCurrencyNative;
    QString mId;
    QString mName;
    QString mPrimary;
    QString mResource;
    QString mResourcePath;
    QString mType;
    QString mUpdated;
private:
    coinbaseAccount *mParent;
signals:

public slots:
};

#endif // COINBASEWALLET_H
