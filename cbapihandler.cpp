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
            //Check for Payment Methods linked to account (Payment methods are used to fund cb and gdax accounts and to withdraw #winnings)
            if ( mAccount->getPaymentMethodCount() < 1 ) {
                say("No Payment Methods Found!");
                say("Fetching Payment Methods from default account [" + defCbId +"]");
                //hrmm...
                listPaymentMethods(); //doh
            }
            //say( "KeyLen: " + QString().setNum(mAccount->mApiKey.length()) );
           // say( "SecLen: " + QString().setNum(mAccount->mApiSecret.length()) );
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

void cbApiHandler::listPaymentMethods() {
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    //                                                            //
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    // URL: https://api.coinbase.com/v2/payment-methods
    // method: GET
    // requestPath: /v2/payment-methods
    // body: EMPTY ie: ""
    mParentProphet->setProphetState("FETCH");
    //Creating a new coinbaseApiRequest
    cbApiRequest* req = new cbApiRequest(this);
    req->setMethod("GET");        //list accounts is a GET
    req->setPath("/v2/payment-methods");  //the url path,etc..
    req->setBody("");             //no body needed (for this one)
    req->setType("listPaymentMethods");//just for us, forgot why...
    req->sendRequest();           //sendRequest has the info/access it needs to do the rest.
    return;
}

///////////////////////
// RESPONSE PROCESSORS
///////////////////////

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
          listPayMethodProcessResponse(resp);
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

void cbApiHandler::listPayMethodProcessResponse( cbApiResponse *resp ) {
    QJsonObject *r = resp->getResponseContent();


    QStringList keys = r->keys();
    int k;
    for(k=0;k<keys.count();k++) {
        //say("rKey " + QString().setNum(k) + ": " + QString(keys.at(k)) );
        if(keys.at(k) == "data") {
            //say("Got It...");
            break;
        }
    }
    QString key=keys.at(k);
    QJsonArray data = r->value(key).toArray();
    //say("Loading PMs From Response...");
    for(k=0;k<data.count();k++) {
        if ( data.at(k).isObject() ) {
            //Each one of these is a Payment Method
            int newPayMethod = mAccount->addPaymentMethod();
            //say("Found PM #" +  QString().setNum(newPayMethod));
            //populate pm
            QJsonObject pmList = data.at(k).toObject();
            keys = pmList.keys();
            for (int pm=0;pm<keys.count();pm++){
                if ( keys.at(pm) == "id" ) {
                    mAccount->getPaymentMethod(newPayMethod)->mId = pmList.value(keys.at(pm)).toString();
                    //say("PM ID -> " + pmList.value(keys.at(pm)).toString().mid(0,pmList.value(keys.at(pm)).toString().indexOf('-')));
                } else if ( keys.at(pm) == "currency" ) {
                    mAccount->getPaymentMethod(newPayMethod)->mCurrency = pmList.value(keys.at(pm)).toString();
                    //say("PM CURRENCY -> " + pmList.value(keys.at(pm)).toString());
                } else if ( keys.at(pm) == "name" ) {
                    mAccount->getPaymentMethod(newPayMethod)->mName = pmList.value(keys.at(pm)).toString();
                    //say("PM NAME -> " + pmList.value(keys.at(pm)).toString());
                }  else if ( keys.at(pm) == "type" ) {
                    mAccount->getPaymentMethod(newPayMethod)->mType = pmList.value(keys.at(pm)).toString();
                    //say("PM TYPE -> " + pmList.value(keys.at(pm)).toString());
                } else if ( keys.at(pm) == "fiat_account" ) {
                    QJsonObject fiat = pmList.value(keys.at(pm)).toObject();
                    QStringList fKeys = fiat.keys();
                    for (int fk=0;fk<fKeys.count();fk++) {
                        if ( fKeys.at(fk) == "resource_path" ) {
                            mAccount->getPaymentMethod(newPayMethod)->mFiatAccountResourcePath = fiat.value(fKeys.at(fk)).toString();
                            //say("PM FIAT REPATH -> " + fiat.value(fKeys.at(fk)).toString());
                        } else if ( fKeys.at(fk) == "id" ) {
                            mAccount->getPaymentMethod(newPayMethod)->mFiatAccountId = fiat.value(fKeys.at(fk)).toString();
                            //say("PM FIAT AccId -> " + fiat.value(fKeys.at(fk)).toString());
                        }
                    }
                }
            }
        }
    }
    //resp->printResponse();
    ///////////////////////////
    /// Next, We need to load our payment methods into the accounts page deposit and withdraw combo boxes
    /// Deposits -> From account can be ach_bank_account, more as I find them.
    /// ----------> To Accounts can be fiat_account, mare added if needed (its your USD wallet)
    ///
    /// Withdrawals -> From account can be type fiat_account, more added when found (its your USD wallet(USD vault,etc))
    /// ----------> To Accounts can be type ach_bank_account, more as I find them.
    ///////////////////////////
    QComboBox *depFrom = mParentProphet->mParent->getDepositFromPayMethodCombo();
    QComboBox *depTo = mParentProphet->mParent->getDepositToPayMethodCombo();
    QComboBox *withdrawFrom = mParentProphet->mParent->getWithdrawFromPayMethodCombo();
    QComboBox *withdrawTo = mParentProphet->mParent->getWithdrawToPayMethodCombo();
    int pmNum = mAccount->getPaymentMethodCount();
    for (int a=0;a<pmNum;a++) {
        QString nickName = mAccount->getPaymentMethod(a)->mCurrency + " " + mAccount->getPaymentMethod(a)->mName.right(6) + " [" + mAccount->getPaymentMethod(a)->mType + "] "
                + "(" + mAccount->getPaymentMethod(a)->mId.mid(0,8) + ")";
        if ( mAccount->getPaymentMethod(a)->mType == "ach_bank_account" ) {
            //add to DepFrom and WithdrawTo list
            depFrom->addItem(nickName);
            depFrom->setItemData(depFrom->findText(nickName),mAccount->getPaymentMethod(a)->mId);
            depFrom->setCurrentIndex(depFrom->findText(nickName));
            withdrawTo->addItem(nickName);
            withdrawTo->setItemData(withdrawTo->findText(nickName),mAccount->getPaymentMethod(a)->mId);
            withdrawTo->setCurrentIndex(withdrawTo->findText(nickName));
        } else if ( mAccount->getPaymentMethod(a)->mType == "fiat_account" ) {
            //add to DepTo and WithdrawFrom list
            depTo->addItem(nickName);
            depTo->setItemData(depTo->findText(nickName),mAccount->getPaymentMethod(a)->mFiatAccountId);
            depTo->setCurrentIndex(depTo->findText(nickName));
            withdrawFrom->addItem(nickName);
            withdrawFrom->setItemData(withdrawFrom->findText(nickName),mAccount->getPaymentMethod(a)->mFiatAccountId);
            withdrawFrom->setCurrentIndex(withdrawFrom->findText(nickName));
        }
    }
//    // Set signal onto button so it works....
    QPushButton *depFromButton = mParentProphet->mParent->getDepositFromPayMethodButton();
    QPushButton *withToButton = mParentProphet->mParent->getWithdrawToPayMethodButton();
    connect(depFromButton, SIGNAL(clicked(bool)),this,SLOT(depositFromButtonSlot()));
    connect(withToButton, SIGNAL(clicked(bool)),this,SLOT(withdrawToButtonSlot()));
    depFromButton->setEnabled(1);
    withToButton->setEnabled(1);
    say("Payment Methods Loaded...");
    say("Manual Withdraw/Deposit/Buy/Sell is Enabled!");
}

///////////
// Slots
///////////


void cbApiHandler::listAccounts() {
    // URL: https://api.coinbase.com/v2/accounts
    // method: GET
    // requestPath: /v2/accounts
    // body: EMPTY ie: ""
    //say("Fetching Account (Wallet List)");
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

void cbApiHandler::withdrawToButtonSlot() {
     say("## WITHDRAW FIAT TO PAYMENT METHOD ##");
     QComboBox *accSelect = mParentProphet->mParent->getWithdrawFromPayMethodCombo();
     QComboBox *accTo = mParentProphet->mParent->getWithdrawToPayMethodCombo();
     QString selected = accSelect->currentData().toString();
     if ( selected.length() > 0 ) {
         QString currency = accSelect->currentText().mid(0,accSelect->currentText().indexOf(" "));
         QString pMethod = selected;
         QString withAmount = mParentProphet->mParent->getWithdrawToPayMethodAmount()->text();
         QString withTo = accTo->currentData().toString();
         QString reqBody = "{ \"amount\": \"" + withAmount + "\", \"currency\": \"" + currency + "\", \"payment_method\": \"" + withTo + "\" }";
         say("## rBody -> " + reqBody);
         //Creating a new coinbaseApiRequest
         cbApiRequest* req = new cbApiRequest(this);
         req->setMethod("POST");
         req->setPath("/v2/accounts/" + pMethod + "/withdrawals");
         req->setBody(reqBody);
         req->setType("WithdrawTo"); //just for us
         req->sendRequest();
     } else {
         say("Payment Method Not Selected...");
         say("Select a Payment Method!");
     }
}

void cbApiHandler::depositFromButtonSlot() {
     say("## DEPOSIT FIAT FROM PAYMENT METHOD ##");
     QComboBox *accSelect = mParentProphet->mParent->getDepositFromPayMethodCombo();
     QComboBox *accTo = mParentProphet->mParent->getDepositToPayMethodCombo();
     QString selected = accSelect->currentData().toString();
     if ( selected.length() > 0 ) {
         QString currency = accSelect->currentText().mid(0,accSelect->currentText().indexOf(" "));
         QString pMethod = selected;
         QString depAmount = mParentProphet->mParent->getDepositFromPayMethodAmount()->text();
         QString depTo = accTo->currentData().toString();
         QString reqBody = "{ \"amount\": \"" + depAmount + "\", \"currency\": \"" + currency + "\", \"payment_method\": \"" + pMethod + "\" }";
         say("## rBody -> " + reqBody);
         //Creating a new coinbaseApiRequest
         cbApiRequest* req = new cbApiRequest(this);
         req->setMethod("POST");
         req->setPath("/v2/accounts/" + depTo + "/deposits");
         req->setBody(reqBody);
         req->setType("DepositFrom"); //just for us
         req->sendRequest();                 //sendRequest has the info/access it needs to do the rest.
     } else {
         say("Payment Method Not Selected...");
         say("Select a Payment Method!");
     }
}


// NEXT IS AUTOTRADING.... I THINK...


