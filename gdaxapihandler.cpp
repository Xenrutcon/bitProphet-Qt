#include "gdaxapihandler.h"

gdaxApiHandler::gdaxApiHandler(bitProphet *parent) : QObject(parent), mAccount(NULL),mWalletTableWidget(NULL) {
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    mParent=parent;
    say("Created GDAX Api Handler.");    
    if ( mParent->getDb()->getGdaxAccountList().length() > 0 ) {
        QString defGdaxId = mParent->getDb()->getDefaultGdaxAccountId();
        if ( defGdaxId != "0" ) {
            say("Found Default GDAX Account. [ " + defGdaxId +" ]");
            mAccount = new gdaxAccount(this);
            //Load Default Account
            mParent->getDb()->loadGdaxAccountById(mAccount,defGdaxId);
            //Load accounts tab dropdown with accounts
            QList<QString> daList= mParent->getDb()->getGdaxAccountList();
            for ( int c=0;c<daList.length();c++) {
                //mParent->addGdaxAccountToGdaxComboBox(daList.at(c));
                say("Gdax Account: " + daList.at(c));
            }
            //Check GDAX balances once
            listGdaxAccounts();
        } else {
            say("GDAX Api Handler Has No Default Account.");
            say("Use Setup Menu to add one.");
        }
    } else {
        say("No GDAX Accounts Exist In Database.");
        say("Use Setup Menu to add one.");
    }
    say("GDAX Api Handler Loaded.");
}


gdaxApiHandler::~gdaxApiHandler() {
    if (mAccount != NULL) { delete mAccount; }
    say("GDAX Api Handler Fading...");
}

///////////
// Methods
///////////
void gdaxApiHandler::say(QString sayThis) {
    mParent->say( "["+mPtrName+"] " + sayThis);
}

void gdaxApiHandler::listGdaxAccounts() {
    // URL: https://api.gdax.com/v2/accounts
    // method: GET
    // requestPath: /accounts
    // body: EMPTY
    say("Fetching GDAX Accounts (GDAX Wallet List)");
    mParent->setProphetState("FETCH");

    gdaxApiRequest* req = new gdaxApiRequest(this);
    req->setMethod("GET");        //list accounts is a GET
    req->setPath("/accounts");  //the url path,etc..
    req->setBody("");             //no body needed (for this one)
    req->setType("listGdaxAccounts"); //just for us
    say("Sending Request...");
    req->sendRequest();           //sendRequest has the info/access it needs to do the rest.
    say("Request Sent.");
}

QString gdaxApiHandler::getGdaxApiKey() {
    return mAccount->mApiKey;
}

QString gdaxApiHandler::getGdaxApiSecret() {
    return mAccount->mApiSecret;
}

QString gdaxApiHandler::getGdaxPassPhrase() {
    return mAccount->mPassPhrase;
}

///////////////////////
// RESPONSE PROCESSORS
///////////////////////

void gdaxApiHandler::processResponse( gdaxApiResponse *resp ) {
    QString type = resp->getType();
    say("Processing Response Type: " + type);
    if (type == "listGdaxAccounts" ) {
        say("Got Gdax Wallets...");
        listAccountProcessResponse(resp);
//        if ( mParent->mAutoRefreshGdaxAccount ) {
//            int timer= mParent->mAutoRefreshGdaxAccountInterval;
//            //QTimer::singleShot(timer, mParent, SLOT(listGdaxAccountSlot()));
//        }
    } else if (type == "gdaxSomething" ) {
        //processBuyAutoSpotResponse(resp);
    } else {
        say("Unknown Response Type: " + type);
    }
    mParent->mParent->getGdaxStatProcessedLabel()->setText(QString().setNum(mParent->mParent->getGdaxStatProcessedLabel()->text().toInt() + 1));
    say("Done Processing Response Type: " + type);
}

void gdaxApiHandler::listAccountProcessResponse(gdaxApiResponse *resp ) {
    QString type = resp->getType();
    say("Processing Response >>> " + type);
    QJsonArray arr = *resp->getResponseArray();
    say("Items: " + QString().setNum(arr.count()) );
    for(int w=0;w<arr.count();w++) {
        int newWalletId = mAccount->addWallet();
        mAccount->getWallet(newWalletId)->mId = arr.at(w).toObject()["id"].toString(); //store entire string
        mAccount->getWallet(newWalletId)->mCurrency = arr.at(w).toObject()["currency"].toString();
        mAccount->getWallet(newWalletId)->mBalance = arr.at(w).toObject()["balance"].toString();
        mAccount->getWallet(newWalletId)->mAvailable = arr.at(w).toObject()["available"].toString();
        mAccount->getWallet(newWalletId)->mHold = arr.at(w).toObject()["hold"].toString();
        mAccount->getWallet(newWalletId)->mProfileId = arr.at(w).toObject()["profile_id"].toString();
//        say("id: " + arr.at(w).toObject()["id"].toString().mid(0,8) ); //display only first 8
//        say("currency: " + arr.at(w).toObject()["currency"].toString() );
//        say("balance: " + arr.at(w).toObject()["balance"].toString() );
//        say("available: " + arr.at(w).toObject()["available"].toString() );
//        say("hold: " + arr.at(w).toObject()["hold"].toString() );
//        say("profile_id: " + arr.at(w).toObject()["profile_id"].toString().mid(0,8) );
    }
    if ( mWalletTableWidget != NULL ) {
        mWalletTableWidget->deleteLater();
    }
    mWalletTableWidget = new gdaxWalletTable(mAccount,this);
    say("Processed " + QString().setNum(mAccount->getWalletCount()) + " Wallets.");
}

