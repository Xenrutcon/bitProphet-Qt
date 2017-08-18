#ifndef BPWINDOW_H
#define BPWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QComboBox>
#include <QTableWidget>
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
    coinbaseAccountSetupWindow *getAccSetupWindow();
    QComboBox* getAccountsCombo();
    void killAccSetupWindow();
    QTableWidget *getAccountTablePlacer();
    QWidget *getAccountsTab();
private:
    Ui::bpWindow *ui;
    bitProphet *mProphet;
    coinbaseAccountSetupWindow *mCbSetupWin;
private slots:
    void accountSetupClicked();
};

#endif // BPWINDOW_H
