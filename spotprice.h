#ifndef SPOTPRICE_H
#define SPOTPRICE_H

#include <QObject>

class spotPrice : public QObject
{
    Q_OBJECT
public:
    explicit spotPrice(QObject *parent);
    QString mId;
    QString mCoin;
    QString mPrice;
    QString mTimestamp;
signals:

public slots:
};

#endif // SPOTPRICE_H
