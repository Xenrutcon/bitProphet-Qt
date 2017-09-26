#ifndef GDAXACCOUNTSETUPWINDOW_H
#define GDAXACCOUNTSETUPWINDOW_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class gdaxAccountSetupWindow;
}

class gdaxAccountSetupWindow :  public QDialog {
    Q_OBJECT
public:
    explicit gdaxAccountSetupWindow(QWidget *parent);
    ~gdaxAccountSetupWindow();
private:
    Ui::gdaxAccountSetupWindow *ui;
};

#endif // GDAXACCOUNTSETUPWINDOW_H
