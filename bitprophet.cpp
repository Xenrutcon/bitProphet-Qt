#include "bitprophet.h"

bitProphet::bitProphet(QObject *parent) : QObject(parent),  mAutoRefreshAccount(true),  mAutoRefreshAccountInterval(8500),
    mAutoCheckSpotPrices(true), mAutoCheckSpotPricesInterval(15000), mAutoSpotTrade(0), mAutoSpotTradeInterval(20500),mDb(NULL), mApiHandler(NULL), mAutoSpot(NULL) {
    mParent = reinterpret_cast<bpWindow*>(parent);
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    // Startup    
    say("(\\.....\\..........,/)");
    say(".\\(....|\\.........)/");
    say(".//\\...| \\......./\\\\");
    say("(/./\\_#oo#_/\\.\\)");
    say(".\\/\\..####../\\/");
    say("......`##'......");
    say("[!] bitProphet [!]");
    // Loading
    mDb = new bpDatabase(this);
    if ( mDb->fileExists() ) { // Check for database file (Sqllite)
        // If File does exist, load available data
        say("Found Database...");
        if ( mDb->hasAccountsTable() ) {
            say("Found Accounts Table.");
            // Check for accounts
        } else {
            say("Accounts Table Not Found.");
            if ( mDb->createAccountsTable() ) {
                say("Accounts Table Initialized!");
                say("Click Setup Menu next to enter Api Info.");
            } else { say("Error, Check debug log!"); }
        }
    } else {
        // If File does not exist, create and initialize
        say("No Database Found!");
        mDb->createDatabase();
        say("Database Created...");
        if ( mDb->createAccountsTable() ) {
            say("Accounts Table Initialized!");
            say("Click Setup Menu next to enter Api Info.");
        } else { say("Error, Check debug log!"); }
    }
    //autoSpotTradeHistory
    if ( !mDb->hasTable("autoSpotTradeHistory") ) {
        if ( mDb->createAutoSpotTradeHistoryTable() ) {
            say("autoSpotTradeHistory Table Initialized!");
        } else { say("Error, Check debug log!"); }
    } else {
        say("Found autoSpotTradeHistory Table.");
    }

    //Price history charts
    if ( mDb->hasTable("cbSpotPriceHistory") ) {
        say("Found cbSpotPriceHistory Table.");
    } else {
        if ( mDb->createCbSpotPriceHistoryTable() ) {
            say("Created cbSpotPriceHistory Table.");
        } else {
            say("ERROR creating cbSpotPriceHistory Table.");
        }
    }

    //Spawn Charts on Charts Tab
    mSplineChartList.append(new bpSplineChart(mParent->getChartsTab(),"BTC Spot Price History"));
    mSplineChartList.at(0)->mView->setGeometry(mParent->getCbBTCPricePlacer()->geometry());
    mSplineChartList.at(0)->mView->show();
    mSplineChartList.append(new bpSplineChart(mParent->getChartsTab(),"LTC Spot Price History"));
    mSplineChartList.at(1)->mView->setGeometry(mParent->getCbLTCPricePlacer()->geometry());
    mSplineChartList.at(1)->mView->show();
    mSplineChartList.append(new bpSplineChart(mParent->getChartsTab(),"ETH Spot Price History"));
    mSplineChartList.at(2)->mView->setGeometry(mParent->getCbETHPricePlacer()->geometry());
    mSplineChartList.at(2)->mView->show();

    // Create autoSpot AFTER all db init (or shit will get CRAAAZEEE)
    if ( mAutoSpotTrade ) {
        mAutoSpot = new cbAutoSpotTrader(this);
        QTimer::singleShot(mAutoSpotTradeInterval,mAutoSpot,SLOT(autoTradeCheck()));
    }

    // Create cbApiHandler AFTER all db init (or shit will get CRAAAZEEE)
    mApiHandler = new cbApiHandler(this);
    // Finish startup process
    setProphetState("IDLE");
    // Start bitProphet based on saved settings (or defaults)



    //Prevent QTextEdits from exhausting memory with logged output ( from say() )
    mParent->getStatusOutput()->document()->setMaximumBlockCount(200);
}

bitProphet::~bitProphet() {
    if (mDb != NULL ) { delete mDb; }    
    if (mAutoSpot != NULL ) { delete mAutoSpot; }
    if (mApiHandler != NULL ) { delete mApiHandler; }
    //delete all charts
    for(int c=0;c<mSplineChartList.length();c++) {
        delete mSplineChartList.at(c);
    }
    mSplineChartList.clear();
    say("bitProphet fading...");    
}

///////////
// Methods
///////////
void bitProphet::setProphetState(QString newState)  {
    mState = newState;
    //say("[-] I am "+ mState +" [-]");
    //TODO: shut up, update label instead of say
}

bpDatabase *bitProphet::getDb() {
    return mDb;
}

void bitProphet::say(QString sayThis, bool debug) {            
    if ( debug ) {
        //no longer exists
    } else {
        mParent->getStatusOutput()->append(mPtrName + ">  " + sayThis);
    }
    std::cout<<QString(mPtrName + ">  " + sayThis).toLocal8Bit().toStdString()<<std::endl;
}

void bitProphet::addAccountToCoinbaseComboBox(QString accountName) {
    say("[addAccountToComboBox] - " + accountName ,1);
    mParent->getAccountsCombo()->addItem(accountName);
}

void bitProphet::setBtcSpotPrice(cbApiResponse *resp) {
    QLabel *ptr = mParent->getBtcSpotPriceLabel();
    QJsonObject r = *(resp->getResponseContent());
    QJsonObject data  = r["data"].toObject();
    //say( "BTC Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());
    //Write to DB History
    mDb->addToCbSpotPriceHistory("BTC",data["amount"].toString());
    mSplineChartList.at(0)->reloadBtcSpotPriceHistoryData(mDb);
    // Check for accounts
}

void bitProphet::setLtcSpotPrice(cbApiResponse *resp) {
    QLabel *ptr = mParent->getLtcSpotPriceLabel();
    QJsonObject r = *(resp->getResponseContent());
    QJsonObject data  = r["data"].toObject();
    //say( "LTC Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());
    mDb->addToCbSpotPriceHistory("LTC",data["amount"].toString());
    mSplineChartList.at(1)->reloadLtcSpotPriceHistoryData(mDb);
}

void bitProphet::setEthSpotPrice(cbApiResponse *resp) {
    QLabel *ptr = mParent->getEthSpotPriceLabel();
    QJsonObject r = *(resp->getResponseContent());
    QJsonObject data  = r["data"].toObject();
    //say( "ETH Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());
    mDb->addToCbSpotPriceHistory("ETH",data["amount"].toString());
    mSplineChartList.at(2)->reloadEthSpotPriceHistoryData(mDb);
}

void bitProphet::setBtcSpotBuyPrice(cbApiResponse *resp) {
    QLabel *ptr = mParent->getBtcSpotBuyPriceLabel();
    QJsonObject r = *(resp->getResponseContent());
    QJsonObject data  = r["data"].toObject();
    //say( "BTC Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());
    // Check for accounts
}

void bitProphet::setLtcSpotBuyPrice(cbApiResponse *resp) {
    QLabel *ptr = mParent->getLtcSpotBuyPriceLabel();
    QJsonObject r = *(resp->getResponseContent());
    QJsonObject data  = r["data"].toObject();
    //say( "LTC Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());
}

void bitProphet::setEthSpotBuyPrice(cbApiResponse *resp) {
    QLabel *ptr = mParent->getEthSpotBuyPriceLabel();
    QJsonObject r = *(resp->getResponseContent());
    QJsonObject data  = r["data"].toObject();
    //say( "ETH Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());

}

void bitProphet::setBtcSpotSellPrice(cbApiResponse *resp) {
    QLabel *ptr = mParent->getBtcSpotSellPriceLabel();
    QJsonObject r = *(resp->getResponseContent());
    QJsonObject data  = r["data"].toObject();
    //say( "BTC Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());
    // Check for accounts
}

void bitProphet::setLtcSpotSellPrice(cbApiResponse *resp) {
    QLabel *ptr = mParent->getLtcSpotSellPriceLabel();
    QJsonObject r = *(resp->getResponseContent());
    QJsonObject data  = r["data"].toObject();
    //say( "LTC Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());
}

void bitProphet::setEthSpotSellPrice(cbApiResponse *resp) {
    QLabel *ptr = mParent->getEthSpotSellPriceLabel();
    QJsonObject r = *(resp->getResponseContent());
    QJsonObject data  = r["data"].toObject();
    //say( "ETH Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());

}

void bitProphet::sellAutoBuyId(QString id, QString coin, QString total) {
    say("#auto SELL ID: " + id);
    say("#auto SELL COIN: " + coin);
    say("#auto SELL TOTAL: $" + total);
    QString destAccount;
    QString paidWith("USD");
    QString paymentMethod;
    for ( int c=0;c<mApiHandler->mAccount->getPaymentMethodCount();c++ ){
        if ( mApiHandler->mAccount->getPaymentMethod(c)->mCurrency == paidWith && mApiHandler->mAccount->getPaymentMethod(c)->mType == "fiat_account") {
            paymentMethod = mApiHandler->mAccount->getPaymentMethod(c)->mId;
        }
    }
    // Send Request
    QString selectedPayment = paymentMethod;
    if ( selectedPayment.length() > 0 ) {
        QString currencySold = coin;
        QString currencyPaidTo = paidWith;
        QString reqBody = "{ \"total\": \"" + total + "\", \"currency\": \"" + currencyPaidTo + "\", \"payment_method\": \"" + selectedPayment + "\" , \"commit\": true }";
        //say("## rBody -> " + reqBody);
        mApiHandler->cbTabLog("#auto Sending total: " + total);
        mApiHandler->cbTabLog("#auto Sending Currency: " + currencyPaidTo);
        mApiHandler->cbTabLog("#auto Sending commit: TRUE");
        //Creating a new coinbaseApiRequest
        cbApiRequest* req = new cbApiRequest(mApiHandler);
        req->setMethod("POST");        
        // ie: BTC buy/sell = BTC wallet
        // find selected wallet and get id for URL
        QString destAccount;
        for ( int c=0;c<mApiHandler->mAccount->getWalletCount();c++ ){
            if ( mApiHandler->mAccount->getWallet(c)->mCurrency == currencySold ) {
                destAccount = mApiHandler->mAccount->getWallet(c)->mId;
            }
        }
        req->setPath("/v2/accounts/" + destAccount + "/sells");
        req->setBody(reqBody);
        req->setType("sellAutoSpot"); //just for us
        req->sendRequest();
    }
    mDb->updateAutoSpotTradeSoldAt(id,total);
    if ( coin == "BTC" ) {
        mAutoSpot->mLastSellPriceBtc = mParent->getBtcSpotSellPriceLabel()->text();
        QTimer::singleShot(1800000,this,SLOT(resetLastSellPriceBtc()));
    } else if ( coin == "LTC" ) {
        mAutoSpot->mLastSellPriceLtc = mParent->getLtcSpotSellPriceLabel()->text();
        QTimer::singleShot(1800000,this,SLOT(resetLastSellPriceLtc()));
    } else if ( coin == "ETH" ) {
        mAutoSpot->mLastSellPriceEth = mParent->getEthSpotSellPriceLabel()->text();
        QTimer::singleShot(1800000,this,SLOT(resetLastSellPriceEth()));
    }
}

coinbaseAccount *bitProphet::getHandlerAccount() {
    return mApiHandler->mAccount;
}

cbApiHandler *bitProphet::getHandler() {
    return mApiHandler;
}

QString bitProphet::findCoinbaseFee(QString dollarAmount) {
    //COINBASE SELL and BUY FEES
    //$1-$10 = $0.99
    //$10-$25 = $1.49
    //$25-$50 = $1.99
    //$50-$200 = $2.99
    //$200+ = 1.49%
    if ( dollarAmount.toDouble() < 10.00 ) {
        return "0.99";
    } else if ( dollarAmount.toDouble() > 10.00 && dollarAmount.toDouble() < 25.00 ) {
        return "1.49";
    } else if ( dollarAmount.toDouble() > 25.00 && dollarAmount.toDouble() < 50.00 ) {
        return "1.99";
    } else if ( dollarAmount.toDouble() > 50.00 && dollarAmount.toDouble() < 200.00 ) {
        return "2.99";
    } else if ( dollarAmount.toDouble() > 200.00 ) {
        return QString().setNum( dollarAmount.toDouble() * 0.0149 );
    }
    return "";
}

void bitProphet::disableAutoSpotTrader() {
    mAutoSpotTrade = false;
    mAutoSpot->deleteLater();
    mAutoSpot = NULL;
}

void bitProphet::enableAutoSpotTrader() {
    mAutoSpotTrade = true;
    mAutoSpot = new cbAutoSpotTrader(this);
    QTimer::singleShot(mAutoSpotTradeInterval,mAutoSpot,SLOT(autoTradeCheck()));
}

/////////
// Slots
/////////
void bitProphet::listAccountSlot() {
    mApiHandler->listAccounts();
}

void bitProphet::resetLastSellPriceBtc() {
    mAutoSpot->mLastSellPriceBtc = "0.00";
}

void bitProphet::resetLastSellPriceLtc() {
    mAutoSpot->mLastSellPriceLtc = "0.00";
}

void bitProphet::resetLastSellPriceEth() {
    mAutoSpot->mLastSellPriceEth = "0.00";
}
