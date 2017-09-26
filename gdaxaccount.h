#ifndef GDAXACCOUNT_H
#define GDAXACCOUNT_H

#include <QObject>


#include "gdaxapihandler.h"
class gdaxApiHandler;


class gdaxAccount : public QObject {
    Q_OBJECT
public:
    explicit gdaxAccount(gdaxApiHandler *parent);
    ~gdaxAccount();
    void say(QString sayThis);
private:
    QString mPtrName;
    gdaxApiHandler *mParent;
signals:        
public slots:
};

#endif // GDAXACCOUNT_H
