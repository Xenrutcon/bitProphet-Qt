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
#include "gdaxapirequest.h"
#include "gdaxapiresponse.h"
#include "gdaxwallettable.h"

class bpWindow;
class bitProphet;
class gdaxAccount;
class gdaxApiRequest;
class gdaxApiResponse;
class gdaxWalletTable;

class gdaxApiHandler : public QObject {
    Q_OBJECT
public:
    explicit gdaxApiHandler(bitProphet *parent);
    ~gdaxApiHandler();    
    void say(QString sayThis);    
    QString getGdaxApiKey();
    QString getGdaxApiSecret();
    QString getGdaxPassPhrase();
    void processResponse( gdaxApiResponse *resp );
    void listAccountProcessResponse(gdaxApiResponse *resp );
    bitProphet *mParent;
private:    
    QString mPtrName;    
    gdaxAccount *mAccount;    
    void listGdaxAccounts();
    gdaxWalletTable *mWalletTableWidget;
signals:

public slots:    
};

#endif // GDAXAPIHANDLER_H
