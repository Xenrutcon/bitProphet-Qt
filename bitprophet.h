#ifndef BITPROPHET_H
#define BITPROPHET_H

#include <QObject>
#include "bpwindow.h"

class bpWindow;
class bitProphet : public QObject {
    Q_OBJECT    
public:
    explicit bitProphet(QObject *parent = nullptr);
    ~bitProphet();
    bpWindow *mParent;
    QString mPtrName;
    QString mState;
    void setProphetState(QString newState);
    void say(QString sayThis, bool debug=0);
signals:

public slots:

private:

};

#endif // BITPROPHET_H
