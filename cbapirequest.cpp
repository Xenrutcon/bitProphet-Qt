#include "cbapirequest.h"

cbApiRequest::cbApiRequest (cbApiHandler *parent) : mParent(parent), mDone(0), mNetAccMan(NULL) ,mResponse(NULL){
        mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
        //mParent->say("CBApiRequest Created!");
        QDateTime current(QDateTime::currentDateTime());
        uint timestamp = current.toTime_t();
        QString ts = QVariant(timestamp).toString();
        mTimestamp = ts;
}

cbApiRequest::~cbApiRequest() { 
    //mParent->say("CBApiRequest Fading...");
}

///////////
// Methods
///////////

QString cbApiRequest::getAccessSign() {
    QString preString(mTimestamp+mMethod+mPath+mBody);
    QString secret(mParent->getCoinbaseApiSecret());
    QString accessKey(QMessageAuthenticationCode::hash(QByteArray(preString.toStdString().c_str()), QByteArray(secret.toStdString().c_str()), QCryptographicHash::Sha256).toHex());
    mAccessKey = accessKey;
    return accessKey;
}

void cbApiRequest::sendRequest() {
    mAccessKey = getAccessSign();
    mNetAccMan = new QNetworkAccessManager();
    QNetworkRequest httpsRequest;
    QSslConfiguration httpsConfig = QSslConfiguration::defaultConfiguration();
    httpsConfig.setProtocol(QSsl::TlsV1_1OrLater);
    httpsRequest.setSslConfiguration(httpsConfig);
    QString url("https://api.coinbase.com" + mPath);
    if ( !QSslSocket::supportsSsl() ) {
        mParent->say("You need OpenSSL Installed For Your OS!");
        mParent->say("For best results, match version below: ");
        mParent->say("URL: " + url + "\n-----\nSSL VERSION: " + QSslSocket::sslLibraryBuildVersionString() + "\nSSL Supported? -> "+QString().setNum(QSslSocket::supportsSsl())+"\n----\n");
    }
    httpsRequest.setUrl(QUrl(url));
    //mParent->say("URL: " + url);
    QByteArray version("2016-03-03"); //stop hardcoding this... eventually...
    httpsRequest.setHeader(QNetworkRequest::ServerHeader,"application/json");
    httpsRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    httpsRequest.setRawHeader(QByteArray("CB-ACCESS-KEY"),QByteArray(mParent->getCoinbaseApiKey().toStdString().c_str()));
    httpsRequest.setRawHeader(QByteArray("CB-ACCESS-SIGN"),QByteArray(mAccessKey.toStdString().c_str()));
    httpsRequest.setRawHeader(QByteArray("CB-ACCESS-TIMESTAMP"),QByteArray(mTimestamp.toStdString().c_str()));
    httpsRequest.setRawHeader(QByteArray("CB-VERSION"),QByteArray(version));
    if ( mMethod == "POST" ) {
        mNetAccMan->post(httpsRequest, QByteArray(mBody.toStdString().c_str()) );
    } else if ( mMethod == "GET" ) {
        mNetAccMan->get(httpsRequest);
    }
    connect(mNetAccMan,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestFinished(QNetworkReply*)));
}

void cbApiRequest::setMethod(QString method) {
    mMethod = method;
}

void cbApiRequest::setType(QString type) {
    mType = type;
}

void cbApiRequest::setPath(QString path) {
    mPath = path;
}

void cbApiRequest::setBody(QString body) {
    mBody = body;
}

//SLOTS
void cbApiRequest::requestFinished(QNetworkReply *reply) {
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    mDone = 1;
    if ( statusCode == 200 || statusCode == 201 ) {
        //mParent->say("Finished Request Type: " + mType);
        QJsonParseError error;
        QByteArray unparsed = reply->readAll();
        reply->deleteLater();
        if ( mType != "listPaymentMethods" && mType != "listAccounts" && mType != "btcSpotPrice" && mType != "btcSpotBuyPrice" && mType != "ethSpotBuyPrice" && mType != "ltcSpotBuyPrice"
             && mType != "btcSpotSellPrice" && mType != "ethSpotSellPrice" && mType != "ltcSpotSellPrice" &&
             mType != "ethSpotPrice" && mType != "ltcSpotPrice" && mType != "buySpotQuote" && mType != "buySpot" &&
             mType != "sellSpotQuote" && mType != "sellSpot" && mType != "fetchTransactions") {
            //If its not a known type, print unparsed because we are probably creating it.
            mParent->say ("unparsed --- " + unparsed);
        }
        QJsonDocument jsonData = QJsonDocument::fromJson(unparsed,&error);
        QJsonObject jsonObj = jsonData.object();
        //oopz, they work better... when they exist.....shutup
        mResponse = new cbApiResponse(this,&jsonObj);
        mResponse->setType(mType);
//        mResponse->printResponse();
        mParent->mParentProphet->setProphetState("IDLE");
        mParent->processResponse(mResponse); //reply and response are deleted HERE
        mNetAccMan->deleteLater();
        mResponse->deleteLater();
    } else {        
        mParent->say("Bad Response, Aborted Request!");
        mParent->say("Response Status: " + QString().setNum(statusCode));
        mParent->say("Response Type: " + mType);
        QByteArray unparsed = reply->readAll();
        reply->deleteLater();
        mParent->say ("unparsed --- " + unparsed);
        mNetAccMan->deleteLater();
        //If this was a special timered call, restart its timer
        if ( mType == "listAccounts" ) {
            mParent->processBadListAccountsResponse();
        }
    }
}

cbApiHandler *cbApiRequest::getMyHandler() {
    return mParent;
}









//going for coffee.
// apiRequest works.

//    \o/

































