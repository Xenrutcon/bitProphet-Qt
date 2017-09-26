#include "gdaxaccountsetupwindow.h"
#include "ui_gdaxaccountsetupwindow.h"

gdaxAccountSetupWindow::gdaxAccountSetupWindow(QWidget *parent) : QDialog(parent), ui(new Ui::gdaxAccountSetupWindow) {
    ui->setupUi(this);
}

gdaxAccountSetupWindow::~gdaxAccountSetupWindow()
{
    delete ui;
}
