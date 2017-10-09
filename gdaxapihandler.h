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
    void xferFromCoinbaseToGdax(QString fromAccId, QString amount,QString currency);
    void xferFromGdaxToCoinbase(QString fromAccId, QString amount,QString currency);
    void placeGdaxLimitBuy(QString prodId,QString size, QString price);
    void placeGdaxLimitSell(QString prodId,QString size, QString price);
    void cancelAllGdaxOrders();
    void processResponse( gdaxApiResponse *resp );
    bitProphet *mParent;
private:    
    QString mPtrName;    
    gdaxAccount *mAccount;    
    void listGdaxAccounts();
    void listAccountProcessResponse(gdaxApiResponse *resp );
    void listCoinbaseAccountsAvailableToGdax();
    void listCoinbaseAccountsProcessResponse(gdaxApiResponse *resp );    
    gdaxWalletTable *mWalletTableWidget;
signals:

public slots:    
private slots:
    void listGdaxAccountsSlot();
};

#endif // GDAXAPIHANDLER_H
