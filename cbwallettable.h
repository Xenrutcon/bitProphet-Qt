#ifndef CBWALLETTABLE_H
#define CBWALLETTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include "cbapihandler.h"
#include "coinbaseaccount.h"

class coinbaseWallet;
class cbApiHandler;

class cbWalletTable : public QObject {
    Q_OBJECT
public:
    cbWalletTable(coinbaseAccount *acc, cbApiHandler *parent);
    ~cbWalletTable();
    QTableWidget *mTableWidget;
    cbApiHandler *mParent;
    void addWallet(coinbaseWallet *w, int r);
signals:

public slots:
};

#endif // CBWALLETTABLE_H
