#ifndef GDAXAPIHANDLER_H
#define GDAXAPIHANDLER_H

#include <QObject>
#include <QString>
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
#include <QComboBox>
#include <QPushButton>
#include <QTimer>

#include "bpwindow.h"
#include "bitprophet.h"
#include "gdaxaccount.h"

class bpWindow;
class bitProphet;
class gdaxAccount;

class gdaxApiHandler : public QObject {
    Q_OBJECT
public:
    explicit gdaxApiHandler(bitProphet *parent);
    ~gdaxApiHandler();    
    void say(QString sayThis);    
private:    
    QString mPtrName;
    bitProphet *mParent;
    gdaxAccount *mAccount;
signals:

public slots:
};

#endif // GDAXAPIHANDLER_H
