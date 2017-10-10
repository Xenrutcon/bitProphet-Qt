#include "gdaxautotrader.h"

gdaxAutoTrader::gdaxAutoTrader(bitProphet *parent) : QObject(parent) {
    mParent = parent;
    //Sell Types
    mTradeTypes.append("LTC");
    mTradeTypes.append("ETH");

    //Buy (with USD) Types
    mBuyTypes.append("LTC");
    mBuyTypes.append("ETH");

    mBTCLog = mParent->mParent->getGdaxAutoTraderBTCLog();
    mLTCLog = mParent->mParent->getGdaxAutoTraderLTCLog();
    mETHLog = mParent->mParent->getGdaxAutoTraderETHLog();
    mBTCLog->document()->setMaximumBlockCount(256);
    mLTCLog->document()->setMaximumBlockCount(256);
    mETHLog->document()->setMaximumBlockCount(256);
    mUSDStartAmount = "0.00";
    mMinUSDBuyAmount = 50.00;
    mMinPercentProfit = 0.005; //in DECIMAL
    sayGdaxAutoTrader("Created!");

}


gdaxAutoTrader::~gdaxAutoTrader(){
    sayGdaxAutoTrader("Fading...");
}

//////////
// Methods
//////////
void gdaxAutoTrader::say(QString sayThis) {
    mParent->say("[gdaxAutoTrader] " + sayThis);
}

void gdaxAutoTrader::sayGdaxAutoTrader(QString sayThis, QString whichBox) {
    //put this in boxes on this tab (TODO)
    if ( whichBox == "ALL") {
        mBTCLog->append(sayThis);
        mLTCLog->append(sayThis);
        mETHLog->append(sayThis);
    } else if (whichBox == "BTC") {
        mBTCLog->append(sayThis);
    } else if (whichBox == "LTC") {
        mLTCLog->append(sayThis);
    } else if (whichBox == "ETH") {
        mETHLog->append(sayThis);
    }
    mParent->say("|gdaxAutoTrader| " + sayThis);
}


////////
// Slots
////////
void gdaxAutoTrader::autoTradeCheck() {
    if ( mParent->mAutoGDAXTrade == false ) { return; }
    //min LTC Buy is 0.01 LTC
    //min BTC Buy is 0.01 BTC
    //min ETH Buy is 0.01 ETH
    int hourRange = 1;
    QString USDBalance("0.00");
    //Dont forget to check mUSDStartAmount
    for ( int a=0;a<mParent->getGdaxHandlerAccount()->getWalletCount();a++ ) {
        if ( mParent->getHandlerAccount()->getWallet(a)->mCurrency == "USD" ) {
            USDBalance = mParent->getGdaxHandlerAccount()->getWallet(a)->mAvailable;  //Available is what we care about
        }
    }
    if ( mUSDStartAmount == "0.00" ) {
        mUSDStartAmount = USDBalance;
    }
    for (int c=0;c<mBuyTypes.length();c++) {
        QString currCoin = mBuyTypes.at(c);
        if ( USDBalance.toDouble() < mMinUSDBuyAmount ) {
            sayGdaxAutoTrader("Available $USD too low (< $"+QString().setNum(mMinUSDBuyAmount)+")",currCoin);
            continue;
        }
        QString howMuchToSpend("0.00");
        if ( USDBalance.toDouble() * 0.20 > mMinUSDBuyAmount ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.20);
        } else if ( USDBalance.toDouble() * 0.25 > mMinUSDBuyAmount ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.25);
        } else if ( USDBalance.toDouble() * 0.40 > mMinUSDBuyAmount ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.40);
        } else if ( USDBalance.toDouble() * 0.50 > mMinUSDBuyAmount ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.50);
        } else if ( USDBalance.toDouble() * 0.75 > mMinUSDBuyAmount ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.75);
        } else if ( USDBalance.toDouble() > mMinUSDBuyAmount ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.75);
        } else {
            sayGdaxAutoTrader("Available $USD too low (< $"+QString().setNum(mMinUSDBuyAmount)+")",currCoin);
            break;
        }
        sayGdaxAutoTrader("# Allocated $" + howMuchToSpend +" For " + currCoin,currCoin);
        sayGdaxAutoTrader("#################",currCoin);
        sayGdaxAutoTrader("# Analyzing Price History",currCoin);
        sayGdaxAutoTrader("# Coin: " + currCoin,currCoin);
        QList<QString> lastPriceRange;
        QList<QString> bidRange;
        QList<QString> askRange;
        QString curPrice("0.00");
        QString curAsk("0.00");
        QString curBid("0.00");
        if (currCoin == "BTC") {
            curPrice = mParent->mParent->getGdaxBtcPriceLabel()->text();
            curAsk = mParent->mParent->getGdaxBtcAskLabel()->text();
            curBid = mParent->mParent->getGdaxBtcBidLabel()->text();
        } else if (currCoin == "LTC") {
            curPrice = mParent->mParent->getGdaxLtcPriceLabel()->text();
            curAsk = mParent->mParent->getGdaxLtcAskLabel()->text();
            curBid = mParent->mParent->getGdaxLtcBidLabel()->text();
        } else if (currCoin == "ETH") {
            curPrice = mParent->mParent->getGdaxEthPriceLabel()->text();
            curAsk = mParent->mParent->getGdaxEthAskLabel()->text();
            curBid = mParent->mParent->getGdaxEthBidLabel()->text();
        }
        //collect historical ranges from db (20s interval = 3 per minute, 180 per hour)
        mParent->getDb()->getGdaxPriceHistoryLast(currCoin,180*hourRange,&lastPriceRange,&bidRange,&askRange);
        sayGdaxAutoTrader("Price: " + lastPriceRange[0],currCoin );
        //find highest, find lowest in range
        //if current != highest && current != lowest && lastmove == UP && N percent below highest && N percent above lowest
            // if current <= lastBuyPriceThisCoin || lastBuyPriceThisCoinExpired == true
                // if current <= lastSellPriceThisCoin || lastSellPriceThisCoinExpired == true
                    //Determine Sell Price (from minProfitPercent)
                        //has sell price been seen in range? how far from highest is it?
                            //Buy
                                //insert buy into db
                                //place for sale immediately @ sell price determined
                                //insert sell into db (update maybe)
        //Update USDBalance ( -howMuchToSpend )
    }
    //restart this Slots timer
    QTimer::singleShot(mParent->mAutoGDAXTradeInterval,this,SLOT(autoTradeCheck()));
    //and then.. a hellbeast ate them.
}
