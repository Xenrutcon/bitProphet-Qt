#ifndef GDAXSETUPADDWINDOW_H
#define GDAXSETUPADDWINDOW_H

#include <QWidget>
#include <QDialog>
#include "bpdatabase.h"
#include "gdaxaccountsetupwindow.h"

namespace Ui {
class gdaxSetupAddWindow;
}

class bpDatabase;
class gdaxAccountSetupWindow;

class gdaxSetupAddWindow : public QDialog {
    Q_OBJECT
public:
    explicit gdaxSetupAddWindow(gdaxAccountSetupWindow *parent);
    ~gdaxSetupAddWindow();
private:
    Ui::gdaxSetupAddWindow *ui;
    gdaxAccountSetupWindow *mParent;
    bpDatabase *mDb;
private slots:
    void doneClicked();
    void saveClicked();
};

#endif // GDAXSETUPADDWINDOW_H
