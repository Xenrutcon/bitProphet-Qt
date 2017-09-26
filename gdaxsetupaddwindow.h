#ifndef GDAXSETUPADDWINDOW_H
#define GDAXSETUPADDWINDOW_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class gdaxSetupAddWindow;
}

class gdaxSetupAddWindow : public QDialog {
    Q_OBJECT
public:
    explicit gdaxSetupAddWindow(QWidget *parent = 0);
    ~gdaxSetupAddWindow();
private:
    Ui::gdaxSetupAddWindow *ui;
};

#endif // GDAXSETUPADDWINDOW_H
