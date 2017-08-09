#include "bpdatabase.h"

bpDatabase::bpDatabase(QObject *parent) : QObject(parent) {    
    mPtrName = QString("0x%1").arg((quintptr)this, QT_POINTER_SIZE * 2, 16, QChar('0'));
    //say("[bpDatabase] Startup...",1);
}

bpDatabase::~bpDatabase() {
    //say("[bpDatabase] Dying...",1);
}


///////////
// Methods
///////////

void bpDatabase::createDatabase() {
   // I know it looks odd...
   {    //trust me.
       QSqlDatabase Db = QSqlDatabase::addDatabase("QSQLITE");
       Db.setDatabaseName("bitProphet.dat");
       if (!Db.open()) {
          say("Error: connecting to database failed!");
       } else {
          say("Database: connection ok.");
       }
       Db.close();
   } //Db is gone now.
   QSqlDatabase::removeDatabase("bitProphet.dat"); //so this actually works (no warnings, no leaks)
   QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

bool bpDatabase::hasAccountsTable() {
    bool retVal = false;
    {
        QSqlDatabase Db = QSqlDatabase::addDatabase("QSQLITE");
        Db.setDatabaseName("bitProphet.dat");
        if (!Db.open()) {
           say("Error: connecting to database failed!");
        } else {
           say("Database: connection ok.");
            QSqlQuery query;
            query.prepare("select COUNT(*) from sqlite_master where type='table' and name='accounts'");
            if (query.exec()) {
               if (query.next()) {
                  int count = query.record().indexOf("COUNT(*)");
                  QString result = query.value(count).toString();
                  if ( result.toInt() ) {
                      retVal = true;
                  }
               }
            }
        }
        Db.close();
    } //Db is gone
    QSqlDatabase::removeDatabase("bitProphet.dat");
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return retVal;
}

bool bpDatabase::createAccountsTable() {
    bool retVal = false;
    {
        QSqlDatabase Db = QSqlDatabase::addDatabase("QSQLITE");
        Db.setDatabaseName("bitProphet.dat");
        if (!Db.open()) {
           say("Error: connecting to database failed!");
        } else {
           say("Database: connection ok.");
           QSqlQuery query;
           query.prepare("CREATE TABLE accounts (id INTEGER PRIMARY KEY AUTOINCREMENT,exchange varchar(64),apiKey varchar(512),apiSecret varchar(512),defaultAccount bool default 0, name VARCHAR(64) );");
           if(query.exec()) {
              retVal = true;
           } else {
              say("createAccountsTable() error:  " + query.lastError().text());
           }
        }
        Db.close();
    }
    QSqlDatabase::removeDatabase("bitProphet.dat");
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    return retVal;
}

void bpDatabase::say(QString sayThis) {
    std::cout<<"("<<mPtrName.toStdString().c_str()<<") "<<sayThis.toStdString().c_str()<<std::endl;
}

bool bpDatabase::fileExists() {
    QFileInfo fCheck("bitProphet.dat");
    if (fCheck.exists() && fCheck.isFile()) {
        return true;
    } else {
        return false;
    }
}

void bpDatabase::insertAccount( QString name, QString apiKey, QString apiSecret, bool defaultAccount,  QString exchange) {
    bool success = false;
    {
        QSqlDatabase Db = QSqlDatabase::addDatabase("QSQLITE");
        Db.setDatabaseName("bitProphet.dat");
        if (!Db.open()) {
           say("Error: connecting to database failed!");
        } else {
           say("Database: connection ok.");
           QSqlQuery query;
           query.prepare("INSERT INTO accounts (exchange, apiKey, apiSecret,defaultAccount,name) VALUES ('" +
                         exchange + "','" + apiKey + "','" + apiSecret + "'," + QString().setNum(QVariant(defaultAccount).toInt()) +
                         ",'" + name + "')");
           if(query.exec()) {
              success = true;
              say("insertAccount() Success");
           } else {
              say("insertAccount() error:  " + query.lastError().text());
           }
        }
        Db.close();
    }
    QSqlDatabase::removeDatabase("bitProphet.dat");
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

/////////
// Slots
/////////
