#include "cbapiresponse.h"

cbApiResponse::cbApiResponse(cbApiRequest *parent,QJsonObject *obj) : QObject(parent) {
    mParent = parent;
    mResponseContent = obj;
    //mParent->getMyHandler()->say("cbApiResponse Created...\n");
}


cbApiResponse::~cbApiResponse() {
    //mParent->getMyHandler()->say("cbApiResponse Fading...\n");
}


///////////
// Methods
///////////

void cbApiResponse::printResponse() {
    mParent->getMyHandler()->say("Request Type: " + mType);
    if (mResponseContent != NULL) {
            printJSONObj(*mResponseContent,"root");
    }
}

void cbApiResponse::printJSONObj(QJsonObject obj,QString key, int depth) {
    //I dont use this...
    //mParent->say("-----   OBJECT!   -----");
    mParent->getMyHandler()->say("-- "+ key +" --");
    //mParent->say("-- Size: " + QVariant(obj.length()).toString() + " --");
    QStringList keyList = obj.keys();
    for(int i=0; i < keyList.count(); ++i){
        QString key=keyList.at(i);
        QString val("");
        if ( obj[key].isString() ) {
            //Value is a STRING
            val = obj[key].toString();
            for(int d=0;d<depth;d++) { key.insert(0,"-"); }
            mParent->getMyHandler()->say(key + " -> " + val);
        } else if ( obj[key].isNull() ) {
            //Value is NULL
            val = "NULL";
            for(int d=0;d<depth;d++) { key.insert(0,"-"); }
           mParent->getMyHandler()->say(key + " -> " + val);
        } else if ( obj[key].isArray() ) {
            //Value is an ARRAY
            depth += 1;
            mParent->getMyHandler()->say("A>>");
            printJSONArr( obj[key].toArray(),key,depth);
            depth -= 1;
            mParent->getMyHandler()->say("<<A");
        } else if ( obj[key].isObject() ) {
            //Value is an OBJECT
            mParent->getMyHandler()->say("O>>");
            depth += 1;
            printJSONObj(obj[key].toObject(),key,depth);
            depth -= 1;
            mParent->getMyHandler()->say("<<O");
        } else {
            //Value is an Integer (or similar)
            val = QVariant(obj[key].toVariant()).toString();
            for(int d=0;d<depth;d++) { key.insert(0,"-"); }
            mParent->getMyHandler()->say(key + " -> " + val);
        }
    }
}

void cbApiResponse::printJSONArr(QJsonArray arr, QString key, int depth) {
    //mParent->say("-----   ARRAY   -----");
    //mParent->say("-- "+ key +" --"); //just for printing where we are,overwrite is allowed.
    //mParent->say("-- Size: " + QVariant(arr.count()).toString() + " --");
    for(int i=0; i < arr.count(); ++i){
        QJsonValue jkey=arr.at(i);
        if ( jkey.isArray() ) {
            //Value is an ARRAY
            depth += 1;
            mParent->getMyHandler()->say("A>>");
            printJSONArr( jkey.toArray(),key,depth);
            depth -= 1;
            mParent->getMyHandler()->say("<<A");
        } else if ( jkey.isObject() ) {
            //Value is an OBJECT
            key = QVariant(i+1).toString();
            depth += 1;
            mParent->getMyHandler()->say("O>>");
            printJSONObj(jkey.toObject(),key,depth);
            depth -= 1;
            mParent->getMyHandler()->say("<<O");
        }
    }
}

void cbApiResponse::setType(QString newType) {
    mType = newType;
}

QString cbApiResponse::getType() {
    return mType;
}

cbApiRequest *cbApiResponse::getParent() {
    return mParent;
}

QJsonObject *cbApiResponse::getResponseContent() {
    return mResponseContent;
}






























