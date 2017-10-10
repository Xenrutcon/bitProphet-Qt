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
    mMinPercentProfit = 0.0025; //in DECIMAL
    mLastBuyPriceBTC = "0.00";mLastBuyPriceLTC = "0.00";mLastBuyPriceETH = "0.00";
    mLastSellPriceBTC = "0.00";mLastSellPriceLTC = "0.00";mLastSellPriceETH = "0.00";
    mLastBuyTimeBTC = "0";mLastBuyTimeLTC = "0";mLastBuyTimeETH = "0";
    mLastSellTimeBTC = "0";mLastSellTimeLTC = "0";mLastSellTimeETH = "0";
    mLastBuyExpireAfter = 3600000;
    mLastSellExpireAfter = 3600000*2;
    sayGdaxAutoTrader("gdaxAutoTrader Exists!");

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
    int hourRange = 12;
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
            sayGdaxAutoTrader("# Available $USD too low (< $"+QString().setNum(mMinUSDBuyAmount)+")",currCoin);
            //continue;
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
            sayGdaxAutoTrader("# Available $USD too low (< $"+QString().setNum(mMinUSDBuyAmount)+")",currCoin);
            //break;
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
        sayGdaxAutoTrader("# Price: $" + curPrice,currCoin );
        sayGdaxAutoTrader("# Ask: $" + curAsk,currCoin );
        sayGdaxAutoTrader("# Bid: $" + curBid,currCoin );

        //find highest, find lowest in range
        sayGdaxAutoTrader("#################",currCoin);
        QString highPrice = findHighestValue(lastPriceRange);
        QString highAsk = findHighestValue(askRange);
        QString highBid = findHighestValue(bidRange);

        QString lowPrice = findLowestValue(lastPriceRange);
        QString lowAsk = findLowestValue(askRange);
        QString lowBid = findLowestValue(bidRange);
        sayGdaxAutoTrader("# High Ask: $" + highAsk,currCoin);
        sayGdaxAutoTrader("# Low Ask: $" + lowAsk,currCoin);
        sayGdaxAutoTrader("# High Bid: $" + highBid,currCoin);
        sayGdaxAutoTrader("# Low Bid: $" + lowBid,currCoin);
        sayGdaxAutoTrader("# High: $" + highPrice,currCoin);
        sayGdaxAutoTrader("# Low: $" + lowPrice,currCoin);

        /* Theory (hi/low buffer creation)
         * # High: $50.30
         * # Low: $49.86
         * Difference over 1 hour of PRICE = 50.30 - 49.86 = $0.44
         * $00.44 * 0.100  = 0.044;
         * $50.00 * 0.001 = 0.050;
         * bufferzone: 15% from top and 5% from bottom
         * This might be completely wrong. Testing.
         * below 50.23 (50.30 - (0.44 * 15%) (7cents))
         * &&
         * above 49.88 (49.86 + (0.44 *  5%) (2cents))
         * seems legit, I've bought worse manually. Maybe try 40% from highest and 5% above lowest to stay away from the highest seen
         */
                //        0x24a3f598>  |gdaxAutoTrader| # IfBuy { 1.9992 }
                //        0x24a3f598>  |gdaxAutoTrader| # AtPrice { 50.02 }
                //        0x24a3f598>  |gdaxAutoTrader| # For { 100 }
                //        0x24a3f598>  |gdaxAutoTrader| # MinProfit { $0.25 }
                //        0x24a3f598>  |gdaxAutoTrader| # Sell Total { $100.25 }
                //        0x24a3f598>  |gdaxAutoTrader| # Sell Target { $50.145 }
                            // Required price move of +$0.12 - 12 cents! to make 25 cents (one quarter of 1% return)
                //        0x249ef598>  |gdaxAutoTrader| # IfBuy { 0.9996 }
                //        0x249ef598>  |gdaxAutoTrader| # AtPrice { 50.02 }
                //        0x249ef598>  |gdaxAutoTrader| # For { 50 }
                //        0x249ef598>  |gdaxAutoTrader| # MinProfit { $0.125 }
                //        0x249ef598>  |gdaxAutoTrader| # Sell Total { $50.125 }
                //        0x249ef598>  |gdaxAutoTrader| # Sell Target { $50.145 }
                            // Required price move of +$0.125 - 12.5 cents! to make 12.5 cents (one quarter of 1% return)
                            //Changes like this occur CONSTANTLY
                            // Feelin pretty clever... hope that lasts....

        //Determine buffer actual
        double highBuffer,lowBuffer,gap;
        gap = highPrice.toDouble() - lowPrice.toDouble();
        highBuffer = highPrice.toDouble() - (gap * 0.20);
        lowBuffer = lowPrice.toDouble() + (gap * 0.05);
        sayGdaxAutoTrader("# HighBuffer: " + QString().setNum(highBuffer),currCoin);
        sayGdaxAutoTrader("# LowBuffer :" + QString().setNum(lowBuffer),currCoin);
        // init pass/fail bools
        bool belowHighest(false);
        bool aboveLowest(false);
        bool belowHighBuffer(false);
        bool aboveLowBuffer(false);
        bool belowLastSellThisCoin(false);
        bool belowLastBuyThisCoin(false);
        //Determine the booleans^
        if ( curBid.toDouble() < highPrice.toDouble() ) { belowHighest = true; }
        if ( curBid.toDouble() > lowPrice.toDouble() ) { aboveLowest = true; }
        if ( curBid.toDouble() < highBuffer ) { belowHighBuffer = true; }
        if ( curBid.toDouble() > lowBuffer ) { aboveLowBuffer = true; }
        if ( currCoin == "BTC") {
            if ( mLastBuyPriceBTC.toDouble() > 0.00  ) {
                if ( curBid.toDouble() <= mLastBuyPriceBTC.toDouble() ) {
                    belowLastBuyThisCoin = true;
                }
            } else { belowLastBuyThisCoin = true; }
            if ( mLastSellPriceBTC.toDouble() > 0.00  ) {
                if ( curBid.toDouble() <= mLastSellPriceBTC.toDouble() ) {
                    belowLastSellThisCoin = true;
                }
            } else { belowLastSellThisCoin = true; }
        } else if ( currCoin == "LTC") {
            if ( mLastBuyPriceLTC.toDouble() > 0.00  ) {
                if ( curBid.toDouble() <= mLastBuyPriceLTC.toDouble() ) {
                    belowLastBuyThisCoin = true;
                }
            } else { belowLastBuyThisCoin = true; }
            if ( mLastSellPriceLTC.toDouble() > 0.00  ) {
                if ( curBid.toDouble() <= mLastSellPriceLTC.toDouble() ) {
                    belowLastSellThisCoin = true;
                }
            } else { belowLastSellThisCoin = true; }
        } else if ( currCoin == "ETH") {
            if ( mLastBuyPriceETH.toDouble() > 0.00  ) {
                if ( curBid.toDouble() <= mLastBuyPriceETH.toDouble() ) {
                    belowLastBuyThisCoin = true;
                }
            } else { belowLastBuyThisCoin = true; }
            if ( mLastSellPriceETH.toDouble() > 0.00  ) {
                if ( curBid.toDouble() <= mLastSellPriceETH.toDouble() ) {
                    belowLastSellThisCoin = true;
                }
            } else { belowLastSellThisCoin = true; }
        }
        //pass or fail, buy or quit
        howMuchToSpend = "50.00";
        if ( belowHighest && belowHighBuffer && aboveLowest && aboveLowBuffer && belowLastBuyThisCoin && belowLastSellThisCoin ) {
                    //Determine Sell Price (from minProfitPercent)
                    double totalBuyAmount = howMuchToSpend.toDouble() / curBid.toDouble();
                    double totalBuyCost = howMuchToSpend.toDouble();
                    double profNeeded = mMinPercentProfit * totalBuyCost;
                    double sellTotal = totalBuyCost + profNeeded;
                    sayGdaxAutoTrader("# IfBuy { " + QString().setNum(totalBuyAmount) + " }",currCoin);
                    sayGdaxAutoTrader("# AtPrice { " + curBid + " }",currCoin);
                    sayGdaxAutoTrader("# For { " + QString().setNum(totalBuyCost) + " }",currCoin);
                    sayGdaxAutoTrader("# MinProfit { $" + QString().setNum(profNeeded) + " }",currCoin);
                    sayGdaxAutoTrader("# Sell Total { $" + QString().setNum(sellTotal) + " }",currCoin);
                    double sellTarget = sellTotal / totalBuyAmount;
                    sayGdaxAutoTrader("# Sell Target { $" + QString().setNum(sellTarget) + " }",currCoin);
                    //has sell price been seen in range?
                    if ( sellTarget > highPrice.toDouble() ) {
                        //To get required profit we would have to go higher than we've seen it go this hour, bad bet.
                        continue;
                    }
                    //table: gdaxAutoTraderHistory
                    //   -> id, coin, type, status, amount, buyPrice,buyTotal, sellTarget, sellTotal, ts,
                    //mParent->getGdaxHandler()->placeGdaxLimitBuy(currCoin+"-USD",QString().setNum(totalBuyAmount),curBid);
                    //PLACE Buy @ curBID (no fee)
                        //insert buy into db with SELLTARGET (update when SOLD)
                        //Update USDBalance ( -howMuchToSpend )
            sayGdaxAutoTrader("# Buy on " + currCoin,currCoin);
        } else {
            sayGdaxAutoTrader("# Passing on " + currCoin,currCoin);
        }
    }
    //restart this Slots timer
    QTimer::singleShot(mParent->mAutoGDAXTradeInterval,this,SLOT(autoTradeCheck()));
    //and then.. a hellbeast ate them.
}

QString gdaxAutoTrader::findLowestValue(QList<QString> hayStack) {
    QString lowest = hayStack.at(0);
    for(int c=0;c<hayStack.length();c++) {
        if ( hayStack.at(c).toDouble() < lowest.toDouble() ) {
            lowest = hayStack.at(c);
        }
    }
    return lowest;
}

QString gdaxAutoTrader::findHighestValue(QList<QString> hayStack) {
    QString highest = hayStack.at(0);
    for(int c=0;c<hayStack.length();c++) {
        if ( hayStack.at(c).toDouble() > highest.toDouble() ) {
            highest = hayStack.at(c);
        }
    }
    return highest;
}
