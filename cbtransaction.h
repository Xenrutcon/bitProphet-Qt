#ifndef CBTRANSACTION_H
#define CBTRANSACTION_H

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

class cbTransaction : public QObject {
    Q_OBJECT
public:
    explicit cbTransaction(QObject *parent = nullptr);
    ~cbTransaction();
    QString mId;
    QString mType;
    QString mStatus;
    QJsonObject mAmountObj;
    QString mAmount;
    QJsonObject mNativeAmountObj;
    QString mAmountNative;
    QString mDescription;
    bool mInstantExchange;
    QString mCreatedAt;
    QString mUpdatedAt;
    QString mResource;
    QString mResourcePath;
    QJsonObject mDetails;
    // optionals that are not always there are: network(hash),to(hash),from(hash),address(hash),application(hash)
signals:

public slots:
};

#endif // CBTRANSACTION_H
