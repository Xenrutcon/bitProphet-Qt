#include "coinbasewallet.h"

coinbaseWallet::coinbaseWallet(coinbaseAccount *parent) : QObject(parent) {
    mParent = parent;
    //mParent->say("Wallet Created!");
}

coinbaseWallet::~coinbaseWallet() {
    //mParent->say("Wallet Destroyed!");
}
