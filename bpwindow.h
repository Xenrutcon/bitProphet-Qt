#ifndef BPWINDOW_H
#define BPWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QLabel>
#include "bitprophet.h"
#include "coinbaseaccountsetupwindow.h"

namespace Ui {
    class bpWindow;
}

class bitProphet;
class coinbaseAccountSetupWindow;
class QComboBox;

class bpWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit bpWindow(QWidget *parent = 0);
    ~bpWindow();
    QTextEdit *getStatusOutput();
    QTextEdit *getDebugLog();
    QTextEdit *getCoinbaseTabLog();
    QTextEdit* getAutoTraderBTCLog();
    QTextEdit* getAutoTraderLTCLog();
    QTextEdit* getAutoTraderETHLog();
    coinbaseAccountSetupWindow *getAccSetupWindow();
    QComboBox* getAccountsCombo();
    QComboBox* getDepositFromPayMethodCombo();
    QComboBox* getDepositToPayMethodCombo();
    QComboBox* getWithdrawFromPayMethodCombo();
    QComboBox* getWithdrawToPayMethodCombo();
    QComboBox* getBuySpotWithPaymentMethodComboBox();
    QComboBox* getSellSpotWithPaymentMethodComboBox();
    QComboBox* getBuySpotTypeComboBox();
    QComboBox* getSellSpotTypeComboBox();
    QPushButton* getDepositFromPayMethodButton();
    QPushButton* getWithdrawToPayMethodButton();
    QPushButton* getQuoteBuySpotForPaymentMethodButton();
    QPushButton* getQuoteSellSpotForPaymentMethodButton();
    QPushButton* getBuySpotForPaymentMethodButton();
    QPushButton* getSellSpotForPaymentMethodButton();
    QPushButton* getRefreshTransactionsButton();
    QPushButton *getRefreshAutoSpotTraderTransactionsButton();
    QLineEdit* getDepositFromPayMethodAmount();
    QLineEdit* getWithdrawToPayMethodAmount();
    void killAccSetupWindow();
    QWidget *getAccountsTab();
    QWidget *getTransactionTab();
    QTableWidget *getAccountTablePlacer();
    QTableWidget *getTransactionTablePlacer();
    QTableWidget *getCbAutoSpotTraderTransactionTablePlacer();
    QLabel *getAccountsNativeBalanceTotalLabel();
    QLabel *getBtcSpotPriceLabel();
    QLabel *getLtcSpotPriceLabel();
    QLabel *getEthSpotPriceLabel();
    QLabel *getBuySpotFeeLabel();
    QLabel *getBuySpotTotalLabel();
    QLabel *getSellSpotFeeLabel();
    QLabel *getSellSpotTotalLabel();
    QLineEdit *getBuySpotAmount();
    QLineEdit *getSellSpotAmount();
    QFrame *getCbBTCPricePlacer();
    QFrame *getCbLTCPricePlacer();
    QFrame *getCbETHPricePlacer();
    QWidget *getChartsTab();
private:
    Ui::bpWindow *ui;
    bitProphet *mProphet;
    coinbaseAccountSetupWindow *mCbSetupWin;
private slots:
    void accountSetupClicked();
};

#endif // BPWINDOW_H
