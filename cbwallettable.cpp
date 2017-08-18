#include "cbwallettable.h"

cbWalletTable::cbWalletTable(coinbaseAccount *acc, cbApiHandler *parent) : QObject(parent) {
    mTableWidget = new QTableWidget(4,12, parent->mParentProphet->mParent->getAccountsTab());
    parent->say("Wallets: " + QString().setNum(acc->getWalletCount()) );
    mParent = parent;
    QWidget *placer = parent->mParentProphet->mParent->getAccountTablePlacer();
    mTableWidget->setGeometry( placer->geometry() );
    placer->hide();
    placer = NULL; //dont delete it, just grab and hide, repeat
    mTableWidget->show();
//    mParent->mParentWin->getWalletTotalNativeLabel()->setText( QString().setNum(0.00) );
    for (int w=0;w<acc->getWalletCount();w++){
        addWallet(acc->getWallet(w),w);
    }

}

cbWalletTable::~cbWalletTable() {
    if (mTableWidget != NULL ) { delete mTableWidget; }
}


void cbWalletTable::addWallet(coinbaseWallet *w, int r) {
    int row = r; int column = 0;
    QTableWidgetItem *newItem = new QTableWidgetItem(w->mCurrency);
    mTableWidget->setItem(row, column, newItem);
    mTableWidget->setHorizontalHeaderItem(column,new QTableWidgetItem("Currency") );

    newItem = new QTableWidgetItem(w->mAmount);
    mTableWidget->setItem(row, column+1, newItem);
    mTableWidget->setHorizontalHeaderItem(column+1,new QTableWidgetItem("Amount") );

    newItem = new QTableWidgetItem(w->mAmountNative);
    mTableWidget->setItem(row, column+2, newItem);
    mTableWidget->setHorizontalHeaderItem(column+2,new QTableWidgetItem("Amount Native") );
//    mParent->mParentWin->getWalletTotalNativeLabel()->setText( QString().setNum(mParent->mParentWin->getWalletTotalNativeLabel()->text().toDouble() + w->mAmountNative.toDouble()) );

    newItem = new QTableWidgetItem(w->mCurrencyNative );
    mTableWidget->setItem(row, column+3, newItem);
    mTableWidget->setHorizontalHeaderItem(column+3,new QTableWidgetItem("Currency Native") );

    newItem = new QTableWidgetItem(w->mId);
    mTableWidget->setItem(row, column+4, newItem);
    mTableWidget->setHorizontalHeaderItem(column+4,new QTableWidgetItem("Id") );

    newItem = new QTableWidgetItem(w->mUpdated);
    mTableWidget->setItem(row, column+5, newItem);
    mTableWidget->setHorizontalHeaderItem(column+5,new QTableWidgetItem("Updated") );
}
