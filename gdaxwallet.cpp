#include "gdaxwallet.h"

gdaxWallet::gdaxWallet(gdaxAccount *parent) : QObject(parent) {
    mParent = parent;
}

gdaxWallet::~gdaxWallet() {

}

void gdaxWallet::say(QString sayThis) {
    mParent->say("[gdaxWallet] " + sayThis);
}
