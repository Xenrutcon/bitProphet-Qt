#ifndef GDAXACCOUNTSETUPWINDOW_H
#define GDAXACCOUNTSETUPWINDOW_H

#include <QDialog>
#include <QWidget>
#include "bpwindow.h"
#include "bpdatabase.h"
#include "gdaxsetupaddwindow.h"

namespace Ui {
class gdaxAccountSetupWindow;
}

class bpWindow;
class bpDatabase;
class gdaxSetupAddWindow;

class gdaxAccountSetupWindow :  public QDialog {
    Q_OBJECT
public:
    explicit gdaxAccountSetupWindow(bpWindow *parent);
    ~gdaxAccountSetupWindow();
    void killSetupAddWindow();
    void loadAccountList();
private:
    Ui::gdaxAccountSetupWindow *ui;
    bpWindow *mParent;
    bpDatabase *mDb;
    gdaxSetupAddWindow *mAddAccountWin;
private slots:
    void addAccountClicked();
    void deleteAccountClicked();
    void doneClicked();
};

#endif // GDAXACCOUNTSETUPWINDOW_H
