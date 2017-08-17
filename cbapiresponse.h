#ifndef CBAPIRESPONSE_H
#define CBAPIRESPONSE_H

#include <QObject>
#include <QString>
#include <QMessageAuthenticationCode>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

#include "cbapirequest.h"

class cbApiRequest;

class cbApiResponse : public QObject {
    Q_OBJECT
public:
    explicit cbApiResponse(cbApiRequest *parent,QJsonObject *obj);
    ~cbApiResponse();
    void printResponse();
    void printJSONObj(QJsonObject obj, QString key="", int depth=0);
    void printJSONArr(QJsonArray arr, QString key="", int depth=0);
    QString getType();
    void setType(QString newType);
    cbApiRequest *getParent();
    QJsonObject *getResponseContent();
private:
    cbApiRequest *mParent;
    QJsonObject *mResponseContent;
    QString mType;
signals:

public slots:
};

#endif // CBAPIRESPONSE_H
