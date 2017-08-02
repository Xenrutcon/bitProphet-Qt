#ifndef BITPROPHET_H
#define BITPROPHET_H

#include <iostream>
#include <QObject>
#include "bpwindow.h"
#include "bpdatabase.h"
#include "coinbaseaccount.h"

class bpWindow;
class bpDatabase;
class coinbaseAccount;

class bitProphet : public QObject {
    Q_OBJECT    
public:
    explicit bitProphet(QObject *parent);
    ~bitProphet();    
    QString mPtrName;
    QString mState;
    bpDatabase *mDb;
    coinbaseAccount *mCbAccount;
    void setProphetState(QString newState);
    void say(QString sayThis, bool debug=0);
private:
    bpWindow *mParent;
signals:

public slots:


};

#endif // BITPROPHET_H
