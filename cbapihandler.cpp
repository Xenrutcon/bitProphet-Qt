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
            mParentProphet->getDb()->loadAccountById(mAccount,defCbId);
            //Load Default Account
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
