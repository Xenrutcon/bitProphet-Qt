#include "gdaxapihandler.h"

gdaxApiHandler::gdaxApiHandler(bitProphet *parent) : QObject(parent), mAccount(NULL) {
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    mParent=parent;
    say("Created GDAX Api Handler.");
    mAccount = mParent->getHandlerAccount();
    if ( mAccount ) {
        say("GDAX Api Handler Loaded.");
    }
}


gdaxApiHandler::~gdaxApiHandler() {
    say("GDAX Api Handler Fading...");
}

///////////
// Methods
///////////
void gdaxApiHandler::say(QString sayThis) {
    mParent->say( "["+mPtrName+"] " + sayThis);
}
