#include "bitprophet.h"

bitProphet::bitProphet(QObject *parent) : QObject(parent), mDb(NULL), mCbAccount(NULL) {
    mParent = reinterpret_cast<bpWindow*>(parent);
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    // Startup
    setProphetState("IDLE");
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

    // Finish startup process
    // Start bitProphet based on saved settings (or defaults)
}

bitProphet::~bitProphet() {
    if (mDb != NULL ) { delete mDb; }
    if (mCbAccount != NULL ) { delete mCbAccount; }
    say("bitProphet fading...");    
}

///////////
// Methods
///////////
void bitProphet::setProphetState(QString newState)  {
    mState = newState;
    say("[-] I am "+ mState +" [-]");
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

/////////
// Slots
/////////
