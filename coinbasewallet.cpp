#include "coinbasewallet.h"

coinbaseWallet::coinbaseWallet(coinbaseAccount *parent) : QObject(parent) {
    mParent = parent;
}
