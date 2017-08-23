#include "cbtransactiontable.h"

cbTransactionTable::cbTransactionTable(cbApiHandler *parent) : QObject(parent), mTableWidget(NULL) {
    mParent = parent;
    mTableWidget = new QTableWidget(1,6, parent->mParentProphet->mParent->getTransactionTab());
    QWidget *placer = parent->mParentProphet->mParent->getTransactionTablePlacer();
    mTableWidget->setGeometry( placer->geometry() );
    int column = 0;
    mTableWidget->setHorizontalHeaderItem(column,new QTableWidgetItem("Type") );
    mTableWidget->setHorizontalHeaderItem(column+1,new QTableWidgetItem("Amount") );
    mTableWidget->setHorizontalHeaderItem(column+2,new QTableWidgetItem("Amount Native") );
    mTableWidget->setHorizontalHeaderItem(column+3,new QTableWidgetItem("Currency") );
    mTableWidget->setHorizontalHeaderItem(column+4,new QTableWidgetItem("Id") );
    mTableWidget->setHorizontalHeaderItem(column+5,new QTableWidgetItem("Created") );
    placer->hide();
    placer = NULL; //dont delete it, just grab and hide, repeat
    mTableWidget->show();
}


cbTransactionTable::~cbTransactionTable() {
    if (mTableWidget != NULL ) { delete mTableWidget; }
}


void cbTransactionTable::addTransaction(cbTransaction *t, int r) {
    int row = (mTableWidget->rowCount()-1)+r-r; //haha im lazy, bye warning
    mTableWidget->insertRow(row);
    int column = 0;
    QTableWidgetItem *newItem = new QTableWidgetItem(t->mType);
    mTableWidget->setItem(row, column, newItem);
    newItem = new QTableWidgetItem(t->mAmount);
    mTableWidget->setItem(row, column+1, newItem);
    newItem = new QTableWidgetItem(t->mAmountNative);
    mTableWidget->setItem(row, column+2, newItem);
    newItem = new QTableWidgetItem(t->mAmountObj["currency"].toString() );
    mTableWidget->setItem(row, column+3, newItem);
    newItem = new QTableWidgetItem(t->mId);
    mTableWidget->setItem(row, column+4, newItem);
    newItem = new QTableWidgetItem(t->mCreatedAt);
    mTableWidget->setItem(row, column+5, newItem);
}
