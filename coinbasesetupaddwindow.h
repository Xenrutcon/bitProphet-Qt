#ifndef COINBASESETUPADDWINDOW_H
#define COINBASESETUPADDWINDOW_H

#include <QDialog>

namespace Ui {
class coinbaseSetupAddWindow;
}

class coinbaseSetupAddWindow : public QDialog
{
    Q_OBJECT

public:
    explicit coinbaseSetupAddWindow(QWidget *parent = 0);
    ~coinbaseSetupAddWindow();

private:
    Ui::coinbaseSetupAddWindow *ui;
};

#endif // COINBASESETUPADDWINDOW_H
