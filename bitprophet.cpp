#include "bitprophet.h"

bitProphet::bitProphet(QObject *parent) : QObject(parent), mDb(NULL), mApiHandler(NULL), mAutoRefreshAccount(true), mAutoRefreshAccountInterval(15000) {
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
    say("[-] I am "+ mState +" [-]");
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

/////////
// Slots
/////////
