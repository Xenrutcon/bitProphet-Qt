#ifndef GDAXAPIRESPONSE_H
#define GDAXAPIRESPONSE_H

#include <QObject>
#include <QString>
#include <QMessageAuthenticationCode>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "gdaxapirequest.h"

class gdaxApiRequest;

class gdaxApiResponse : public QObject {
    Q_OBJECT
public:
    explicit gdaxApiResponse(gdaxApiRequest *parent,QJsonObject *obj,QJsonArray *arr);
    ~gdaxApiResponse();
    void say(QString sayThis);
    void setType(QString newType);
    QString getType();
    gdaxApiRequest *getParent();
    QJsonObject *getResponseContent();
    QJsonArray *getResponseArray();
    QString mAutoTradeId; //100% optional, may be useful for autoTrade request/response amount verification
private:
    gdaxApiRequest *mParent;
    QJsonObject *mResponseContent;
    QJsonArray *mResponseArray;
    QString mType;
signals:    

public slots:
};

#endif // GDAXAPIRESPONSE_H
