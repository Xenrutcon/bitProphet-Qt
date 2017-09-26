#include "gdaxsetupaddwindow.h"
#include "ui_gdaxsetupaddwindow.h"

gdaxSetupAddWindow::gdaxSetupAddWindow(QWidget *parent) :  QDialog(parent), ui(new Ui::gdaxSetupAddWindow) {
    ui->setupUi(this);
}

gdaxSetupAddWindow::~gdaxSetupAddWindow()
{
    delete ui;
}
