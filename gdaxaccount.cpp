#include "gdaxaccount.h"

gdaxAccount::gdaxAccount(gdaxApiHandler *parent) : QObject(parent) {
    mParent = parent;
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    mWalletList.clear();
    mCbWalletList.clear();
    //say("gdaxAccount Created...");
}

gdaxAccount::~gdaxAccount() {
    say("gdaxAccount destroyed.");
}

// METHODS
/////////

void gdaxAccount::say(QString sayThis) {
    mParent->say( "[" + mPtrName + "] " + sayThis);
}

// Wallet List Methods
int gdaxAccount::addWallet() {
    gdaxWallet *add = new gdaxWallet(this);
    mWalletList.append(add);
    return mWalletList.count()-1; //returns the index of the newly added wallet for reference
}

gdaxWallet* gdaxAccount::getWallet(int index) {
    return mWalletList.at(index);
}

int gdaxAccount::getWalletCount() {
    return mWalletList.count();
}

void gdaxAccount::clearWallets() {
    mWalletList.clear();
}
