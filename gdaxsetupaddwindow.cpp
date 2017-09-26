#include "gdaxsetupaddwindow.h"
#include "ui_gdaxsetupaddwindow.h"

gdaxSetupAddWindow::gdaxSetupAddWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gdaxSetupAddWindow)
{
    ui->setupUi(this);
}

gdaxSetupAddWindow::~gdaxSetupAddWindow()
{
    delete ui;
}
