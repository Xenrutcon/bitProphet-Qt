#include "gdaxapiresponse.h"

gdaxApiResponse::gdaxApiResponse(gdaxApiRequest *parent,QJsonObject *obj) : QObject(parent) {
    mParent = parent;
    mResponseContent = obj;
    say("gdaxApiResponse Created.");
}


gdaxApiResponse::~gdaxApiResponse() {
    say("gdaxApiResponse Destroyed.");
}

///////////
// METHODS
///////////

void gdaxApiResponse::say(QString sayThis) {
    mParent->say(  "[gdaxApiResponse] " + sayThis );
}

void gdaxApiResponse::setType(QString newType) {
    mType = newType;
}

QString gdaxApiResponse::getType() {
    return mType;
}

gdaxApiRequest *gdaxApiResponse::getParent() {
    return mParent;
}

QJsonObject *gdaxApiResponse::getResponseContent() {
    return mResponseContent;
}

/////////
// SLOTS
/////////

