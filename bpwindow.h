#ifndef BPWINDOW_H
#define BPWINDOW_H

#include <QMainWindow>

namespace Ui {
class bpWindow;
}

class bpWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit bpWindow(QWidget *parent = 0);
    ~bpWindow();

private:
    Ui::bpWindow *ui;
};

#endif // BPWINDOW_H
