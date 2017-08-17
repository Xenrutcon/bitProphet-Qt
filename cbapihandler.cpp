#include "cbapihandler.h"

cbApiHandler::cbApiHandler(QObject *parent) : QObject(parent),mAccount(NULL) {
    mParentProphet = reinterpret_cast<bitProphet*>(parent);
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    say("Api Handler Created...");
    if ( mParentProphet->getDb()->getAccountList().length() > 0 ) {
        QString defCbId = mParentProphet->getDb()->getDefaultAccountId();
        if ( defCbId != "0" ) {
            say("Found Default Account. [" + defCbId +"]");
            mAccount = new coinbaseAccount(this);
            //Load Default Account
            mParentProphet->getDb()->loadAccountById(mAccount,defCbId);
            //Load accounts tab dropdown with accounts
            QList<QString> daList= mParentProphet->getDb()->getAccountList();
            for ( int c=0;c<daList.length();c++) {
                mParentProphet->addAccountToCoinbaseComboBox(daList.at(c));
            }
            //Check balance once
            listAccounts();

            say( "KeyLen: " + QString().setNum(mAccount->mApiKey.length()) );
            say( "SecLen: " + QString().setNum(mAccount->mApiSecret.length()) );
            //start AutoRefreshAccount (if)

            //start Auto Simple Trading (if)
        } else {
            say("Default Account was not found.");
            say("Create one using the Setup Menu.");
        }
    }
}

cbApiHandler::~cbApiHandler() {
    if (mAccount != NULL ) { delete mAccount; }
    say("Api Handler Fading...");
}

///////////
// Methods
///////////

void cbApiHandler::say(QString sayThis) {
    mParentProphet->say( "[" + mPtrName + "] " + sayThis);
}

QString cbApiHandler::getCoinbaseApiSecret() {
    //Return from whichever account is active (one at time)
    return mAccount->mApiSecret;
}

QString cbApiHandler::getCoinbaseApiKey() {
    //Return from whichever account is active (one at time)
    return mAccount->mApiKey;
}

void cbApiHandler::processResponse( cbApiResponse *resp ) {
    QString type = resp->getType();
    if (type == "listAccounts" ) {
        say("Response Type: " + type);
        listAccountProcessResponse(resp);
    } else if (type == "listPaymentMethods" ) {
        //say("Response Type: " + type);
//        loadPayMethods(resp);
    } else if (type == "btcSpotPrice" ) {
        //say("Response Type: " + type);
//        mParentProphet->setBTCSpotPrice(resp);
    } else if (type == "ethSpotPrice" ) {
        //say("Response Type: " + type);
//        mParentProphet->setETHSpotPrice(resp);
    } else if (type == "ltcSpotPrice" ) {
        //say("Response Type: " + type);
//        mParentProphet->setLTCSpotPrice(resp);
    } else {
        say("Unknown Response Type: " + type);
    }
    if ( mParentProphet->mAutoRefreshAccount ) {
        int timer= mParentProphet->mAutoRefreshAccountInterval;
        QTimer::singleShot(5000, mParentProphet, SLOT(listAccountSlot()));
    }
    resp->getParent()->deleteLater();
}

///////////////////////
// RESPONSE PROCESSORS
///////////////////////
void cbApiHandler::listAccountProcessResponse(cbApiResponse *resp) {
      //QJsonObject obj = *(resp->getResponseContent());
//    QJsonObject data  = obj["data"].toObject();
//    QJsonObject paging  = obj["pagination"].toObject();
//    say ( "Found Data: " + data[0].toObject()["id"].toString() + " Entries Found.");
}

///////////
// Slots
///////////


void cbApiHandler::listAccounts() {
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    //                                                            //
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    // URL: https://api.coinbase.com/v2/accounts
    // method: GET
    // requestPath: /v2/accounts
    // body: EMPTY ie: ""
    say("Fetching Account (Wallet List)");


    //Instead of manual, Creating a new coinbaseApiRequest
    cbApiRequest* req = new cbApiRequest(this);
//    mCurrentRequestList.append(req);
    req->setMethod("GET");        //list accounts is a GET
    req->setPath("/v2/accounts");  //the url path,etc..
    req->setBody("");             //no body needed (for this one)
    req->setType("listAccounts");//just for us, forgot why...
    say("Sending Request...");
    req->sendRequest();           //sendRequest has the info/access it needs to do the rest.
    say("Request Sent.");
    //All request bodies should have content type application/json and be valid JSON.
    //CB-ACCESS-KEY The api key as a string
    //CB-ACCESS-SIGN The user generated message signature (see below)
    //CB-ACCESS-TIMESTAMP A timestamp for your request
    //The body is the request body string or omitted if there is no request body (typically for GET requests).
    //The method should be UPPER CASE.
}

