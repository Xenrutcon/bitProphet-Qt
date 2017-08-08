#include "coinbasesetupaddwindow.h"
#include "ui_coinbasesetupaddwindow.h"

coinbaseSetupAddWindow::coinbaseSetupAddWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::coinbaseSetupAddWindow)
{
    ui->setupUi(this);
}

coinbaseSetupAddWindow::~coinbaseSetupAddWindow()
{
    delete ui;
}
