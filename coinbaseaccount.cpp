#include "coinbaseaccount.h"

coinbaseAccount::coinbaseAccount(QObject *parent) {
    mParent = reinterpret_cast<bitProphet*>(parent);
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    say("[coinbaseAccount] Initialize...",1);

//    mWalletList.clear();
//    mPayMethods.clear();
}

coinbaseAccount::~coinbaseAccount() {
    say("[coinbaseAccount] Dying...",1);
}

////////////
// Methods
////////////
void coinbaseAccount::say(QString sayThis, bool debug) {
    mParent->say( "(" +mPtrName+ ") " + sayThis,debug);
}

//int coinbaseAccount::addWallet() {
//    coinbaseWallet *add = new coinbaseWallet(this);
//    mWalletList.append(add);
//    return mWalletList.count()-1; //returns the index of the newly added wallet for reference
//}

//int coinbaseAccount::addPaymentMethod() {
//    coinbasePaymentMethod *add = new coinbasePaymentMethod(this);
//    mPayMethods.append(add);
//    return mPayMethods.count()-1; //returns the index of the newly added ... for reference
//}

//coinbasePaymentMethod* coinbaseAccount::getPaymentMethod(int index) {
//    return mPayMethods.at(index);
//}

//coinbaseWallet* coinbaseAccount::getWallet(int index) {
//    return mWalletList.at(index);
//}

//int coinbaseAccount::getWalletCount() {
//    return mWalletList.count();
//}

//void coinbaseAccount::clearWallets() {
//    mWalletList.clear();
//}


//int coinbaseAccount::getPaymentMethodCount() {
//    return mPayMethods.count();
//}

/////////
// Slots
/////////
