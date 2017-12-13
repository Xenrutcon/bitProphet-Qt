#ifndef GDAXAPIREQUEST_H
#define GDAXAPIREQUEST_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslCipher>
#include <QLoggingCategory>
#include <QSslSocket>
#include <QMessageAuthenticationCode>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QByteArray>
#include "gdaxapihandler.h"
#include "gdaxapiresponse.h"

class gdaxApiHandler;
class gdaxApiResponse;

class gdaxApiRequest : public QObject {
    Q_OBJECT
public:
    explicit gdaxApiRequest(gdaxApiHandler *parent);
    ~gdaxApiRequest();
    QString mPtrName;
    void say(QString sayThis);
    void setMethod(QString method);
    void setType(QString type);
    void setPath(QString path);
    void setBody(QString body);
    gdaxApiHandler *getMyHandler();
    void sendRequest();
    gdaxApiResponse *mResponse;
    QString mAutoTradeId; //100% optional, may be useful for autoTrade request/response amount verification
private:
    QString getAccessSign();
    gdaxApiHandler *mParent;
    QNetworkAccessManager* mNetAccMan;
    QString mTimestamp;
    QString mMethod;
    QString mType;
    QString mPath;
    QString mBody;
    QString mAccessKey;
    bool mDone;        
signals:
public slots:
private slots:
    void requestFinished(QNetworkReply *reply);
};

#endif // GDAXAPIREQUEST_H
