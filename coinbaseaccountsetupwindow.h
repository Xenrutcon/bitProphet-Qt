#ifndef COINBASEACCOUNTSETUPWINDOW_H
#define COINBASEACCOUNTSETUPWINDOW_H

#include <QDialog>
#include "coinbasesetupaddwindow.h"
#include "bpwindow.h"
#include "bpdatabase.h"

namespace Ui {
class coinbaseAccountSetupWindow;
}

class coinbaseSetupAddWindow;
class bpWindow;
class bpDatabase;

class coinbaseAccountSetupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit coinbaseAccountSetupWindow(QWidget *parent = 0);
    ~coinbaseAccountSetupWindow();    
    void killSetupAddWindow();
    void loadAccountList();
private:
    Ui::coinbaseAccountSetupWindow *ui;
    coinbaseSetupAddWindow *mAddAccountWin;
    bpWindow *mParent;    
    bpDatabase *mDb;
private slots:
    void addAccountClicked();
    void doneClicked();
    void deleteAccountClicked();
};

#endif // COINBASEACCOUNTSETUPWINDOW_H
