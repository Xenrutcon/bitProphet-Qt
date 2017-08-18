#include "bitprophet.h"

bitProphet::bitProphet(QObject *parent) : QObject(parent), mDb(NULL), mApiHandler(NULL), mAutoRefreshAccount(true), mAutoRefreshAccountInterval(15000),
    mAutoCheckSpotPrices(true), mAutoCheckSpotPricesInterval(5000) {
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
    // Create cbApiHandler
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
    if (mApiHandler != NULL ) { delete mApiHandler; }
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
    say( "BTC Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());
}

void bitProphet::setLtcSpotPrice(cbApiResponse *resp) {
    QLabel *ptr = mParent->getLtcSpotPriceLabel();
    QJsonObject r = *(resp->getResponseContent());
    QJsonObject data  = r["data"].toObject();
    say( "LTC Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());
}

void bitProphet::setEthSpotPrice(cbApiResponse *resp) {
    QLabel *ptr = mParent->getEthSpotPriceLabel();
    QJsonObject r = *(resp->getResponseContent());
    QJsonObject data  = r["data"].toObject();
    say( "ETH Spot Price: " + data["amount"].toString() );
    ptr->setText(data["amount"].toString());
}


/////////
// Slots
/////////
void bitProphet::listAccountSlot() {
    mApiHandler->listAccounts();
}
