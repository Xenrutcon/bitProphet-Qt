#include "bitprophet.h"

bitProphet::bitProphet(QObject *parent) : QObject(parent),  mAutoRefreshAccount(true),  mAutoRefreshAccountInterval(8500),
    mAutoCheckSpotPrices(true), mAutoCheckSpotPricesInterval(5500), mAutoSpotTrade(1), mAutoSpotTradeInterval(20500),mDb(NULL), mApiHandler(NULL), mAutoSpot(NULL) {
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
    mParent->getDebugLog()->document()->setMaximumBlockCount(200);
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
        mParent->getDebugLog()->append(QString(mPtrName + ">  " + sayThis));
    } else {
        mParent->getStatusOutput()->append(mPtrName + ">  " + sayThis);
        mParent->getDebugLog()->append(QString(mPtrName + ">  " + sayThis));
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

coinbaseAccount *bitProphet::getHandlerAccount() {
    return mApiHandler->mAccount;
}

/////////
// Slots
/////////
void bitProphet::listAccountSlot() {
    mApiHandler->listAccounts();
}
