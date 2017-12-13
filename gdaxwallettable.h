#ifndef GDAXWALLETTABLE_H
#define GDAXWALLETTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include "gdaxapihandler.h"
#include "gdaxaccount.h"
#include "gdaxwallet.h"

class gdaxAccount;
class gdaxApiHandler;
class gdaxWallet;

class gdaxWalletTable : public QObject {
    Q_OBJECT
public:
    explicit gdaxWalletTable(gdaxAccount *acc, gdaxApiHandler *parent);
    ~gdaxWalletTable();
    QTableWidget *mTableWidget;
    gdaxApiHandler *mParent;
    void addWallet(gdaxWallet *w, int r);
signals:

public slots:
};

#endif // GDAXWALLETTABLE_H
