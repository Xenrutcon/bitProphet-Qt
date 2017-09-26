#include "gdaxaccount.h"

gdaxAccount::gdaxAccount(gdaxApiHandler *parent) : QObject(parent) {
    mParent = parent;
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    say("gdaxAccount Created...");
}

gdaxAccount::~gdaxAccount() {
    say("gdaxAccount destroyed.");
}

// METHODS
/////////

void gdaxAccount::say(QString sayThis) {
    mParent->say( "[" + mPtrName + "] " + sayThis);
}

