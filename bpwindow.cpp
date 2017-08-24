#include "bpwindow.h"
#include "ui_bpwindow.h"

bpWindow::bpWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::bpWindow), mCbSetupWin(NULL) {
    ui->setupUi(this);
    //bpWindow creates the actual bitProphet
    //bitProphet controls everything
    //bpWindow handles display tasks for bitProphet
    mProphet = new bitProphet(this);

    //Bind Menus
    connect(ui->actionAccount,&QAction::triggered,this, &bpWindow::accountSetupClicked);
}

///////////
//decon
///////////
bpWindow::~bpWindow()
{
    //These will always exist.
    delete mProphet; //delete before ui    
    delete ui;
}

///////////
//Methods
///////////
QTextEdit * bpWindow::getStatusOutput() {
    return ui->mStatusOutput;
}

QTextEdit * bpWindow::getDebugLog() {
    return ui->mDebugLog;
}

QTextEdit* bpWindow::getCoinbaseTabLog() { return ui->mCoinbaseTabLog; }

QTextEdit* bpWindow::getAutoTraderBTCLog() { return ui->mAutoSpotBTCLog; }
QTextEdit* bpWindow::getAutoTraderLTCLog() { return ui->mAutoSpotLTCLog; }
QTextEdit* bpWindow::getAutoTraderETHLog() { return ui->mAutoSpotETHLog; }

QTableWidget *bpWindow::getAccountTablePlacer() {
    return ui->cbAccountTablePlacer;
}

QTableWidget *bpWindow::getCbAutoSpotTraderTransactionTablePlacer() {
    return ui->mCbAutoSpotTraderTransactionTablePlacer;
}

QWidget *bpWindow::getAccountsTab() {
    return ui->mCoinbaseTab;
}

QLabel *bpWindow::getAccountsNativeBalanceTotalLabel() {
    return ui->mTotalNativeValue;
}

QLabel *bpWindow::getBtcSpotPriceLabel() {
    return ui->mBtcSpotPriceLabel;
}

QLabel *bpWindow::getLtcSpotPriceLabel() {
    return ui->mLtcSpotPriceLabel;
}

QLabel *bpWindow::getEthSpotPriceLabel() {
    return ui->mEthSpotPriceLabel;
}

QLabel *bpWindow::getBtcSpotBuyPriceLabel() {
    return ui->mBtcSpotBuyPriceLabel;
}

QLabel *bpWindow::getLtcSpotBuyPriceLabel() {
    return ui->mLtcSpotBuyPriceLabel;
}

QLabel *bpWindow::getEthSpotBuyPriceLabel() {
    return ui->mEthSpotBuyPriceLabel;
}

QLabel *bpWindow::getBtcSpotSellPriceLabel() {
    return ui->mBtcSpotSellPriceLabel;
}

QLabel *bpWindow::getLtcSpotSellPriceLabel() {
    return ui->mLtcSpotSellPriceLabel;
}

QLabel *bpWindow::getEthSpotSellPriceLabel() {
    return ui->mEthSpotSellPriceLabel;
}



QLabel *bpWindow::getBuySpotFeeLabel() {
    return ui->mBuySpotFeeLabel;
}

QLabel *bpWindow::getBuySpotTotalLabel() {
    return ui->mBuySpotTotalLabel;
}

QLabel *bpWindow::getSellSpotFeeLabel() {
    return ui->mSellSpotFeeLabel;
}

QLabel *bpWindow::getSellSpotTotalLabel() {
    return ui->mSellSpotTotalLabel;
}

QLineEdit *bpWindow::getBuySpotAmount() {
    return ui->mBuySpotAmount;
}

QLineEdit *bpWindow::getSellSpotAmount() {
    return ui->mSellSpotAmount;
}

QFrame *bpWindow::getCbBTCPricePlacer() {
    return ui->mCbBTCPriceChartPlacer;
}

QFrame *bpWindow::getCbLTCPricePlacer() {
    return ui->mCbLTCPriceChartPlacer;
}

QFrame *bpWindow::getCbETHPricePlacer() {
    return ui->mCbETHPriceChartPlacer;
}

QWidget *bpWindow::getChartsTab() {
    return ui->mChartsTab;
}

QWidget *bpWindow::getTransactionTab() {
    return ui->mTransactionsTab;
}

QTableWidget *bpWindow::getTransactionTablePlacer() {
    return ui->mCbTransactionTablePlacer;
}



coinbaseAccountSetupWindow *bpWindow::getAccSetupWindow() {
    return mCbSetupWin;
}

QComboBox* bpWindow::getAccountsCombo() {
    return ui->mCbAccountComboBox;
}

QComboBox* bpWindow::getDepositFromPayMethodCombo() {
    return ui->mDepFromComboBox;
}

QComboBox* bpWindow::getDepositToPayMethodCombo() {
    return ui->mDepToComboBox;
}

QComboBox* bpWindow::getWithdrawFromPayMethodCombo() {
    return ui->mWithdrawFromComboBox;
}

QComboBox* bpWindow::getWithdrawToPayMethodCombo() {
    return ui->mWithdrawToComboBox;
}

QComboBox* bpWindow::getBuySpotWithPaymentMethodComboBox() {
    return ui->mBuySpotWithPaymentMethodComboBox;
}

QComboBox* bpWindow::getSellSpotWithPaymentMethodComboBox() {
    return ui->mSellSpotWithPaymentMethodComboBox;
}

QComboBox* bpWindow::getBuySpotTypeComboBox() {
    return ui->mBuySpotTypeComboBox;
}

QComboBox* bpWindow::getSellSpotTypeComboBox() {
    return ui->mSellSpotTypeComboBox;
}

QLineEdit* bpWindow::getDepositFromPayMethodAmount() {
    return ui->mDepositFromAmount;
}

QLineEdit* bpWindow::getWithdrawToPayMethodAmount() {
    return ui->mWithdrawToAmount;
}

QPushButton* bpWindow::getDepositFromPayMethodButton() {
    return ui->mDepositFromPaymentMethodButton;
}

QPushButton* bpWindow::getWithdrawToPayMethodButton() {
    return ui->mWithdrawToPaymentMethodButton;
}

QPushButton* bpWindow::getQuoteBuySpotForPaymentMethodButton() {
    return ui->mQuoteBuySpotForPaymentMethodButton;
}

QPushButton* bpWindow::getQuoteSellSpotForPaymentMethodButton() {
    return ui->mQuoteSellSpotForPaymentMethodButton;
}

QPushButton* bpWindow::getBuySpotForPaymentMethodButton() {
    return ui->mBuySpotForPaymentMethodButton;
}

QPushButton* bpWindow::getSellSpotForPaymentMethodButton() {
    return ui->mSellSpotForPaymentMethodButton;
}

QPushButton* bpWindow::getRefreshTransactionsButton() {
    return ui->mRefreshTransactionsButton;
}

QPushButton *bpWindow::getRefreshAutoSpotTraderTransactionsButton() {
    return ui->mRefreshAutoSpotTraderTransactionsButton;
}

void bpWindow::killAccSetupWindow() {
    delete mCbSetupWin;
    mCbSetupWin = NULL;
}

///////////
//Slots
///////////
void bpWindow::accountSetupClicked() {
    if ( mCbSetupWin != NULL ) {
        delete mCbSetupWin;
        mCbSetupWin = new coinbaseAccountSetupWindow(this);
    } else {
        mCbSetupWin = new coinbaseAccountSetupWindow(this);
    }
    mCbSetupWin->show();
}
