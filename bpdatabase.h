#ifndef BPDATABASE_H
#define BPDATABASE_H

#include <QObject>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>


#include "bitprophet.h"
#include "coinbaseaccount.h"

class bitProphet;
class coinbaseAccount;

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
    bool createCbSpotPriceHistoryTable();
    void addToCbSpotPriceHistory( QString coin, QString price );
    void insertAccount( QString name, QString apiKey, QString apiSecret, bool defaultAccount=0,  QString exchange="coinbase");
    QList<QString> getAccountList();
    void deleteAccount(QString id );
    QString getDefaultAccountId();
    void loadAccountByName(coinbaseAccount *target, QString accountName);
    void loadAccountById(coinbaseAccount *target, QString id);

private:
    void say(QString sayThis);

signals:

public slots:

};

#endif // BPDATABASE_H
