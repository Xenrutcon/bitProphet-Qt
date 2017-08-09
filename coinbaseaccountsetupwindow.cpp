#include "coinbaseaccountsetupwindow.h"
#include "ui_coinbaseaccountsetupwindow.h"

coinbaseAccountSetupWindow::coinbaseAccountSetupWindow(QWidget *parent) :
    QDialog(parent),  ui(new Ui::coinbaseAccountSetupWindow), mAddAccountWin(NULL) {
    ui->setupUi(this);    
    mParent = reinterpret_cast<bpWindow*>(parent);
    setWindowFlag(Qt::WindowCloseButtonHint,false);
    setWindowFlag(Qt::WindowContextHelpButtonHint,false);
    mParent->hide();
    //Bind Buttons
    connect(ui->mAddButton,SIGNAL(clicked(bool)),this,SLOT(addAccountClicked()));
    connect(ui->mDoneButton,SIGNAL(clicked(bool)),this,SLOT(doneClicked()));
}

coinbaseAccountSetupWindow::~coinbaseAccountSetupWindow() {
    if ( mAddAccountWin != NULL ) { delete mAddAccountWin; }
    delete ui;
}

void coinbaseAccountSetupWindow::killSetupAddWindow() {
    delete mAddAccountWin;
    mAddAccountWin = NULL;
}

/////////
// Slots
/////////

void coinbaseAccountSetupWindow::addAccountClicked() {
    if ( mAddAccountWin != NULL ) { delete mAddAccountWin; }
    mAddAccountWin = new coinbaseSetupAddWindow(this);
    mAddAccountWin->show();
}

void coinbaseAccountSetupWindow::doneClicked() {
    if ( mAddAccountWin != NULL ) { delete mAddAccountWin; }
    mParent->show();
    mParent->killAccSetupWindow();
}


