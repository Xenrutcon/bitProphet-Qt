#ifndef BPWINDOW_H
#define BPWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "bitprophet.h"

namespace Ui {
    class bpWindow;
}

class bitProphet;

class bpWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit bpWindow(QWidget *parent = 0);
    ~bpWindow();
    QTextEdit *getStatusOutput();
    QTextEdit *getDebugLog();
private:
    Ui::bpWindow *ui;
    bitProphet *mProphet;
};

#endif // BPWINDOW_H
