#include "bitprophet.h"

bitProphet::bitProphet(QObject *parent) : QObject(parent) {
    mParent = reinterpret_cast<bpWindow*>(parent);
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    //Startup
    setProphetState("IDLE");
    say("(\\.....\\..........,/)");
    say(".\\(....|\\.........)/");
    say(".//\\...| \\......./\\\\");
    say("(/./\\_#oo#_/\\.\\)");
    say(".\\/\\..####../\\/");
    say("......`##'......");
    say("[!] bitProphet [!]");
}

bitProphet::~bitProphet() {
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
    mParent->getStatusOutput()->append(mPtrName + ">  " + sayThis);
}

/////////
// Slots
/////////
