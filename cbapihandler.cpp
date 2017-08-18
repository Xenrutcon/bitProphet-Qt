#include "cbapihandler.h"

cbApiHandler::cbApiHandler(QObject *parent) : QObject(parent),mAccount(NULL), mWalletTableWidget(NULL) {
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
            //Start Spot Check
            if ( mParentProphet->mAutoCheckSpotPrices ) {
                QTimer::singleShot(mParentProphet->mAutoCheckSpotPricesInterval,this,SLOT(fetchSpotPrices()));
            }
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
    // say("Processing Response Type: " + type);
    if (type == "listAccounts" ) {
        listAccountProcessResponse(resp);
        if ( mParentProphet->mAutoRefreshAccount ) {
            int timer= mParentProphet->mAutoRefreshAccountInterval;
            QTimer::singleShot(timer, mParentProphet, SLOT(listAccountSlot()));
        }
    } else if (type == "listPaymentMethods" ) {
        //loadPayMethods(resp);
    } else if (type == "btcSpotPrice" ) {
          mParentProphet->setBtcSpotPrice(resp);
    } else if (type == "ethSpotPrice" ) {
          mParentProphet->setEthSpotPrice(resp);
    } else if (type == "ltcSpotPrice" ) {
          mParentProphet->setLtcSpotPrice(resp);
    } else {
        say("Unknown Response Type: " + type);
    }
    // say("Done Processing Response Type: " + type);
    resp->getParent()->deleteLater();
}


void cbApiHandler::processBadListAccountsResponse() {
    if ( mParentProphet->mAutoRefreshAccount ) {
        say("Restarting Timer [autoRefreshAccount]");
        int timer= mParentProphet->mAutoRefreshAccountInterval;
        QTimer::singleShot(timer, mParentProphet, SLOT(listAccountSlot()));
    }
}

///////////////////////
// RESPONSE PROCESSORS
///////////////////////
void cbApiHandler::listAccountProcessResponse(cbApiResponse *resp) {
        if ( mAccount->getWalletCount() > 0 ) {
            for ( int a=0;a<mAccount->getWalletCount();a++) {
                mAccount->getWallet(a)->deleteLater();
            }
            mAccount->clearWallets();
        }
        QJsonObject obj = *(resp->getResponseContent());
        QJsonArray data  = obj["data"].toArray();
        QJsonObject paging  = obj["pagination"].toObject();
        //say ( "Found Data: " + QString().setNum(data.count()) + " Entries Found.");
        for (int d=0;d<data.count();d++) {
            // each element here is a wallet each of which has:
            // id , name, primary(bool), type, currency,
            // balance(obj), native_balance(obj), created_at, updated_at, resource, resource_path, ready(bool)
            QJsonObject el = data.at(d).toObject();
            //          say("ID: " + el["id"].toString().mid(0,el["id"].toString().indexOf('-') ) );
            //          say("Name: " + el["name"].toString() );
            //          say("Type: " + el["type"].toString() );
            //          say("Currency: " + el["currency"].toString() );
            QJsonObject balEl = el["balance"].toObject();
            //          say("Balance Amount: " + balEl["amount"].toString() );
            //          say("Currency Type: " + balEl["currency"].toString() );
            QJsonObject natBalEl = el["native_balance"].toObject();
            //          say("Native Balance Amount: $" + natBalEl["amount"].toString() );
            //          say("Native Currency Type: " + natBalEl["currency"].toString() );
            //          say("Resource: " + el["resource"].toString() );
            //          //say("Ready?: " + QString().setNum(el["ready"].toInt()) );  //API DOCS says this exists, it does not.
            //          say("------------------------------------------");

            // COFFEEEEEEEEE

            int newWalletIndex = mAccount->addWallet();
            coinbaseWallet *newWallet = mAccount->getWallet(newWalletIndex);
            newWallet->mId = el["id"].toString();
            newWallet->mName = el["name"].toString();
            newWallet->mType = el["type"].toString();
            newWallet->mCurrency = balEl["currency"].toString();
            newWallet->mAmount = balEl["amount"].toString();
            newWallet->mCurrencyNative = natBalEl["currency"].toString();
            newWallet->mAmountNative = natBalEl["amount"].toString();
            newWallet->mResource = el["resource"].toString();
            newWallet->mCreated = el["created_at"].toString();
            newWallet->mUpdated = el["updated_at"].toString();
            //say ("Added Wallet " + el["id"].toString().mid(0,el["id"].toString().indexOf('-') ));
            if ( mWalletTableWidget != NULL ) {
                mWalletTableWidget->deleteLater();
            }
            mWalletTableWidget = new cbWalletTable(mAccount,this);
        }
        //say ( "Found Paging: " + QString().setNum(paging.count()) + " Entries Found.");

}

///////////
// Slots
///////////


void cbApiHandler::listAccounts() {
    // URL: https://api.coinbase.com/v2/accounts
    // method: GET
    // requestPath: /v2/accounts
    // body: EMPTY ie: ""
    say("Fetching Account (Wallet List)");
    mParentProphet->setProphetState("FETCH");

    //Instead of manual, Creating a new coinbaseApiRequest
    cbApiRequest* req = new cbApiRequest(this);
//    mCurrentRequestList.append(req);
    req->setMethod("GET");        //list accounts is a GET
    req->setPath("/v2/accounts");  //the url path,etc..
    req->setBody("");             //no body needed (for this one)
    req->setType("listAccounts");//just for us, forgot why...
    //say("Sending Request...");
    req->sendRequest();           //sendRequest has the info/access it needs to do the rest.
    //say("Request Sent.");
}

void cbApiHandler::fetchBTCSpotPrice() {
    // https://api.coinbase.com/v2/prices/BTC-USD/spot
    cbApiRequest* req = new cbApiRequest(this);
    req->setMethod("GET");
    req->setPath("/v2/prices/BTC-USD/spot");
    req->setBody("");
    req->setType("btcSpotPrice");
    req->sendRequest();
    return;
}

void cbApiHandler::fetchETHSpotPrice() {
    // https://api.coinbase.com/v2/prices/ETH-USD/spot
    cbApiRequest* req = new cbApiRequest(this);
    req->setMethod("GET");
    req->setPath("/v2/prices/ETH-USD/spot");
    req->setBody("");
    req->setType("ethSpotPrice");
    req->sendRequest();
    return;
}

void cbApiHandler::fetchLTCSpotPrice() {
    // https://api.coinbase.com/v2/prices/LTC-USD/spot
    cbApiRequest* req = new cbApiRequest(this);
    req->setMethod("GET");
    req->setPath("/v2/prices/LTC-USD/spot");
    req->setBody("");
    req->setType("ltcSpotPrice");
    req->sendRequest();
    return;
}

void cbApiHandler::fetchSpotPrices() {
    fetchBTCSpotPrice();
    QTimer::singleShot(700,this,SLOT(fetchLTCSpotPrice()));
    QTimer::singleShot(1000,this,SLOT(fetchETHSpotPrice()));
    if ( mParentProphet->mAutoCheckSpotPrices ) {
        QTimer::singleShot(mParentProphet->mAutoCheckSpotPricesInterval,this,SLOT(fetchSpotPrices()));
    }
}
