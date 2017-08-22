#ifndef CBTRANSACTIONTABLE_H
#define CBTRANSACTIONTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "cbapihandler.h"
#include "cbtransaction.h"

class cbApiHandler;

class cbTransactionTable : public QObject
{
    Q_OBJECT
public:
    explicit cbTransactionTable(cbApiHandler *parent);
    ~cbTransactionTable();
    cbApiHandler *mParent;
    QTableWidget *mTableWidget;
    void addTransaction(cbTransaction *t, int r);
signals:

public slots:
};

#endif // CBTRANSACTIONTABLE_H
