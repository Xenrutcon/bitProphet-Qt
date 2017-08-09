#ifndef COINBASEACCOUNTSETUPWINDOW_H
#define COINBASEACCOUNTSETUPWINDOW_H

#include <QDialog>
#include "coinbasesetupaddwindow.h"
#include "bpwindow.h"

namespace Ui {
class coinbaseAccountSetupWindow;
}

class coinbaseSetupAddWindow;
class bpWindow;

class coinbaseAccountSetupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit coinbaseAccountSetupWindow(QWidget *parent = 0);
    ~coinbaseAccountSetupWindow();    
    void killSetupAddWindow();
private:
    Ui::coinbaseAccountSetupWindow *ui;
    coinbaseSetupAddWindow *mAddAccountWin;
    bpWindow *mParent;
private slots:
    void addAccountClicked();
    void doneClicked();
};

#endif // COINBASEACCOUNTSETUPWINDOW_H
