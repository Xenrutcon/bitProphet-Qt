#ifndef BPDATABASE_H
#define BPDATABASE_H

#include <QObject>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "spotprice.h"
#include "bpsplinechart.h"

#include "bitprophet.h"
#include "coinbaseaccount.h"

class bitProphet;
class coinbaseAccount;
class spotPrice;
class bpSplineChart;

class bpDatabase : public QObject {
    Q_OBJECT
public:
    explicit bpDatabase(QObject *parent);
    ~bpDatabase();
    QString mPtrName;
    bool fileExists();
    bool hasAccountsTable();
    bool hasTable(QString tableName);
    void createDatabase();
    bool createAccountsTable();
    bool createGdaxAccountsTable();
    bool createCbSpotPriceHistoryTable();
    bool createAutoSpotTradeHistoryTable();
    void addToCbSpotPriceHistory( QString coin, QString price );
    void insertAccount( QString name, QString apiKey, QString apiSecret, bool defaultAccount=0,  QString exchange="coinbase");
    void insertGdaxAccount( QString name, QString apiKey, QString apiSecret, bool defaultAccount,  QString exchange="GDAX");
    void insertAutoSpotTrade( QString coin, QString type, QString boughtAt,QString soldAt,QString amount,QString status);
    void updateAutoSpotTradeSoldAt( QString id, QString soldAt);
    QList<QString> getAccountList();
    QList<QString> getGdaxAccountList();
    void deleteAccount(QString id );
    void deleteGdaxAccount(QString id );
    QString getDefaultAccountId();
    void loadAccountByName(coinbaseAccount *target, QString accountName);
    void loadAccountById(coinbaseAccount *target, QString id);
    void getBtcSpotPriceHistoryLast(int howManyMax, bpSplineChart *chart);
    void getLtcSpotPriceHistoryLast(int howManyMax,bpSplineChart *chart);
    void getEthSpotPriceHistoryLast(int howManyMax,bpSplineChart *chart);
    void getSpotPriceHistoryLast(QString coin,int maxResults, QList<QString> *priceList);
    void getAutoSpotBuysForSaleList(QList<QString> *idList,QList<QString> *boughtAtList,QList<QString> *coinAmountList,QString coin);
private:
    void say(QString sayThis);

signals:

public slots:

};

#endif // BPDATABASE_H
