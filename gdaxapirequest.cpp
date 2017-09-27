#include "gdaxapirequest.h"
#include <iostream>

gdaxApiRequest::gdaxApiRequest(gdaxApiHandler *parent) : QObject(parent), mNetAccMan(NULL), mResponse(NULL) {
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    mParent = parent;
    say("GDax Api Request Created.");
    QDateTime current(QDateTime::currentDateTime());
    uint timestamp = current.toTime_t();
    QString ts = QVariant(timestamp).toString();
    mTimestamp = ts;
}

gdaxApiRequest::~gdaxApiRequest() {
    if (mResponse != NULL) { mResponse->deleteLater(); }
    say("GDax Api Request Destroyed.");
}

///////////
// METHODS
///////////


void gdaxApiRequest::say(QString sayThis) {
    mParent->say( "[" + mPtrName + "] " + sayThis);
}

void gdaxApiRequest::setMethod(QString method) {
    mMethod = method;
}

void gdaxApiRequest::setType(QString type) {
    mType = type;
}

void gdaxApiRequest::setPath(QString path) {
    mPath = path;
}

void gdaxApiRequest::setBody(QString body) {
    mBody = body;
}

gdaxApiHandler *gdaxApiRequest::getMyHandler() {
    return mParent;
}

QString gdaxApiRequest::getAccessSign() {
    QString preString(mTimestamp+mMethod+mPath+mBody);
    QString secret(mParent->getGdaxApiSecret());
    //say("Secret: " + secret);
    QByteArray decodedSecret = QByteArray::fromBase64(QByteArray(secret.toStdString().c_str()));
    //say("Decoded: " + QString(decodedSecret.toStdString().c_str()));
    QString accessKey(QMessageAuthenticationCode::hash(QByteArray(preString.toStdString().c_str()),decodedSecret, QCryptographicHash::Sha256).toBase64());
    return accessKey;
}

void gdaxApiRequest::sendRequest() {
    mAccessKey = getAccessSign();
    mNetAccMan = new QNetworkAccessManager(this);
    QNetworkRequest httpsRequest;
    QSslConfiguration httpsConfig = QSslConfiguration::defaultConfiguration();
    httpsConfig.setProtocol(QSsl::TlsV1_1OrLater);
    httpsRequest.setSslConfiguration(httpsConfig);
    QString url("https://api.gdax.com" + mPath);
    if ( !QSslSocket::supportsSsl() ) {
        mParent->say("You need OpenSSL Installed For Your OS!");
        mParent->say("For best results, match version below: ");
        mParent->say("URL: " + url + "\n-----\nSSL VERSION: " + QSslSocket::sslLibraryBuildVersionString() + "\nSSL Supported? -> "+QString().setNum(QSslSocket::supportsSsl())+"\n----\n");
    }
    httpsRequest.setUrl(QUrl(url));
    say("URL: " + url);
    //say("key: " + mParent->getGdaxApiKey());
    QByteArray version("2016-03-03"); //stop hardcoding this... eventually...
    httpsRequest.setHeader(QNetworkRequest::ServerHeader,"application/json");
    httpsRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    httpsRequest.setRawHeader(QByteArray("CB-ACCESS-KEY"),QByteArray(mParent->getGdaxApiKey().toStdString().c_str()));
    httpsRequest.setRawHeader(QByteArray("CB-ACCESS-SIGN"),QByteArray(mAccessKey.toStdString().c_str()));
    httpsRequest.setRawHeader(QByteArray("CB-ACCESS-TIMESTAMP"),QByteArray(mTimestamp.toStdString().c_str()));
    httpsRequest.setRawHeader(QByteArray("CB-ACCESS-PASSPHRASE"),QByteArray(mParent->getGdaxPassPhrase().toStdString().c_str()));
    httpsRequest.setRawHeader(QByteArray("CB-VERSION"),QByteArray(version));
    if ( mMethod == "POST" ) {
        mNetAccMan->post(httpsRequest, QByteArray(mBody.toStdString().c_str()) );
    } else if ( mMethod == "GET" ) {
        mNetAccMan->get(httpsRequest);
    }
    connect(mNetAccMan,SIGNAL(finished(QNetworkReply*)),this,SLOT(requestFinished(QNetworkReply*)));
}

/////////
// SLOTS
/////////

void gdaxApiRequest::requestFinished(QNetworkReply *reply) {
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    mDone = 1;
    if ( statusCode == 200 || statusCode == 201 ) {
        //mParent->say("Finished Request Type: " + mType);
        QJsonParseError error;
        QByteArray unparsed = reply->readAll();
        reply->deleteLater();
        if ( mType != "listGdaxAccounts" ) {
            //If its not a known type, print unparsed because we are probably creating it.
            mParent->say ("unparsed --- " + unparsed);
        }
        QJsonDocument jsonData = QJsonDocument::fromJson(unparsed,&error);
        QJsonObject jsonObj = jsonData.object();
        //COMING SOON..response parsing.
        mResponse = new gdaxApiResponse(this,&jsonObj);
        mResponse->setType(mType);
        mParent->processResponse(mResponse);
        mParent->mParent->setProphetState("IDLE");
        mNetAccMan->deleteLater();
        mResponse->deleteLater();
        mResponse = NULL;
    } else {
        mParent->say("Bad Response, Aborted Request!");
        mParent->say("Response Status: " + QString().setNum(statusCode));
        mParent->say("Response Type: " + mType);
        QByteArray unparsed = reply->readAll();
        mParent->say ("unparsed --- " + unparsed);
        reply->deleteLater();        
        mNetAccMan->deleteLater();

//        //If this was a special timered call, restart its timer
//        if ( mType == "listAccounts" ) {
//            mParent->processBadListAccountsResponse();
//        }
    }
    this->deleteLater(); //After process reponse (or fail to process (with error) ), this request is gone forever.
}
