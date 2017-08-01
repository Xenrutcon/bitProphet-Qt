#include "bpwindow.h"
#include "ui_bpwindow.h"

bpWindow::bpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::bpWindow)
{
    ui->setupUi(this);
}

bpWindow::~bpWindow()
{
    delete ui;
}
