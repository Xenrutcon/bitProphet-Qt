#include "gdaxapihandler.h"

gdaxApiHandler::gdaxApiHandler(bitProphet *parent) : QObject(parent), mAccount(NULL) {
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    mParent=parent;
    say("Created GDAX Api Handler.");    
    if ( 0 ) {
        say("GDAX Api Handler Loaded.");        
    } else {
        say("GDAX Api Handler Has No Default Account.");
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
