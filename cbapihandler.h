#ifndef CBAPIHANDLER_H
#define CBAPIHANDLER_H

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
#include "cbapirequest.h"
#include "cbapiresponse.h"

//#include "coinbasewallet.h"
//#include "cbwallettable.h"

class bpWindow;
class bitProphet;
class coinbaseAccount;
class cbApiResponse;
//class coinbaseApiRequest;
//class cbWalletTable;



class cbApiHandler : public QObject {
    Q_OBJECT
public:
    explicit cbApiHandler(QObject *parent = nullptr);
    ~cbApiHandler();
    void listWalletAccounts();
    void listPaymentMethods();
    bitProphet *mParentProphet;
    QString getCoinbaseApiKey();
    QString getCoinbaseApiSecret();
    void say( QString sayThis );
    void processResponse( cbApiResponse *resp );
    void listAccountProcessResponse( cbApiResponse *resp );
    void listAccounts();
private:
    QString mPtrName;
    QString mCbApiKey;
    QString mCbApiSecret;
    coinbaseAccount *mAccount; //api handler 'has' only one account(at a time) even if more exist (TODO: make switchable)    
    //QList<coinbaseApiRequest*> mCurrentRequestList;
    //cbWalletTable *mWalletTableWidget;
public slots:
//    void withdrawFiatButtonSlot();
//    void loadWalletAccount( coinbaseApiResponse *resp );
//    void loadPayMethods( coinbaseApiResponse *resp );
//    void processResponse( coinbaseApiResponse *resp );
//      void listWalletAccountSlot();
//    void depositFiatButtonSlot();
//    void apiRequestComplete();
signals:

public slots:


};

#endif // CBAPIHANDLER_H
