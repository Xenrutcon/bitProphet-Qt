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
#include "coinbaseaccount.h"

class bpWindow;
class bitProphet;
class coinbaseAccount;

class gdaxApiHandler : public QObject {
    Q_OBJECT
public:
    explicit gdaxApiHandler(bitProphet *parent);
    ~gdaxApiHandler();
signals:

public slots:
};

#endif // GDAXAPIHANDLER_H
