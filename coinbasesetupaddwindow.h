#ifndef COINBASESETUPADDWINDOW_H
#define COINBASESETUPADDWINDOW_H

#include <QDialog>
#include "coinbaseaccountsetupwindow.h"
#include "bpdatabase.h"

namespace Ui {
    class coinbaseSetupAddWindow;
}

class coinbaseAccountSetupWindow;
class bpDatabase;

class coinbaseSetupAddWindow : public QDialog {
    Q_OBJECT
public:
    explicit coinbaseSetupAddWindow(QWidget *parent = 0);
    ~coinbaseSetupAddWindow();
    void say(QString sayThis);
private:
    Ui::coinbaseSetupAddWindow *ui;
    coinbaseAccountSetupWindow *mParent;
    bpDatabase *mDb;
private slots:
    void doneClicked();
    void saveClicked();
};

#endif // COINBASESETUPADDWINDOW_H
