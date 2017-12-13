#ifndef GDAXWALLET_H
#define GDAXWALLET_H

#include <QObject>
#include "gdaxaccount.h"

class gdaxAccount;

class gdaxWallet : public QObject {
    Q_OBJECT
public:
    explicit gdaxWallet(gdaxAccount *parent);
    ~gdaxWallet();
    QString mId;
    QString mCurrency;
    QString mBalance;
    QString mAvailable;
    QString mHold;
    QString mProfileId;
private:
    gdaxAccount *mParent;
    void say(QString sayThis);
signals:

public slots:

};
#endif // GDAXWALLET_H
