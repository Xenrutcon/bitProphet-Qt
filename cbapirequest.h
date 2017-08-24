#ifndef CBAPIREQUEST_H
#define CBAPIREQUEST_H

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
#include "cbapihandler.h"
#include "cbapiresponse.h"

class cbApiHandler;
class cbApiResponse;

class cbApiRequest : public QObject {
    Q_OBJECT
public:
    explicit cbApiRequest(cbApiHandler *parent);
    ~cbApiRequest();
    QString mPtrName;
    void setType(QString type);
    void setMethod(QString method="GET");
    void setPath(QString path);
    void setBody(QString body);
    void printRequest();
    QString getAccessSign();
    void sendRequest();
    cbApiHandler *getMyHandler();
    QString mAutoSpotTradeId;
private:
    cbApiHandler *mParent;
    QString mTimestamp;
    QString mMethod;
    QString mType;
    QString mPath;
    QString mBody;
    QString mAccessKey;
    bool mDone;
    QNetworkAccessManager* mNetAccMan;
    cbApiResponse *mResponse;
signals:

public slots:
    void requestFinished(QNetworkReply *reply);
};

#endif // CBAPIREQUEST_H
