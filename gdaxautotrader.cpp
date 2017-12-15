#include "gdaxautotrader.h"

gdaxAutoTrader::gdaxAutoTrader(bitProphet *parent) : QObject(parent) {
    mParent = parent;
    //Sell Types
    mTradeTypes.append("BTC");
    mTradeTypes.append("LTC");

    //Buy (with USD) Types
    mBuyTypes.append("BTC");
    mBuyTypes.append("LTC");


    mBTCLog = mParent->mParent->getGdaxAutoTraderBTCLog();
    mLTCLog = mParent->mParent->getGdaxAutoTraderLTCLog();
    mETHLog = mParent->mParent->getGdaxAutoTraderETHLog();
    mBTCLog->document()->setMaximumBlockCount(256);
    mLTCLog->document()->setMaximumBlockCount(256);
    mETHLog->document()->setMaximumBlockCount(256);
    mUSDStartAmount = "0.00";
    mMinUSDBuyAmount = 5.00;
    mMinPercentProfit = 0.01; //in DECIMAL
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

void gdaxAutoTrader::checkForBuyFills() {
    // status = 'placed' -> Ignore these, not yet verified by response from server, may not really exist
    // status = 'placed2' -> These are VERIFIED and DO EXIST and MAY OR MAY NOT be partially or completely filled(bought)
    // status = 'posted' -> This is in the process of placing an automatic SELL for this order, not yet verified by response from server, may not really exist
    // status = 'posted2' -> These are SELLING, are VERIFIED and DO EXIST and MAY OR MAY NOT be partially or completely filled(sold);
                //^when switching to posted or posted2 change type from BUY to SELL also....

    //The tricks here all happen in the responseProcessor
    //collect a list of all 'placed2' buys
    //when unwrapping response, updateDb entry as needed with orderId

    //get all 'placed2' status BUYs for each coin
    for (int c=0;c<mBuyTypes.length();c++) {
        QString currCoin = mBuyTypes.at(c);
        sayGdaxAutoTrader("Checking "+currCoin+" Buys For Fills",currCoin);
        //get all 'placed2' status BUYs
        QList<QString> buyList;
        buyList.clear();
        mParent->getDb()->getGdaxAutoBuysPlaced(currCoin,&buyList);
        sayGdaxAutoTrader(currCoin+" Buys: "+QString().setNum(buyList.count()),currCoin);
        //send a /fills/order-id for each placed2 status buy
        for(int o=0;o<buyList.count();o++){
            mParent->getGdaxHandler()->fetchGdaxFillsForOrderId(buyList.at(o));
        }
    }
    QTimer::singleShot(mParent->mAutoGDAXTradeInterval/2,this,SLOT(checkForBuyFills()));
    //chill 2 seconds, then scan the sells
    QTimer::singleShot(2000,this,SLOT(checkForSellFills()));
}

void gdaxAutoTrader::checkForSellFills() {
    // status = 'posted' -> This is in the process of placing an automatic SELL for this order, not yet verified by response from server, may not really exist
    // status = 'posted2' -> These are SELLING, are VERIFIED and DO EXIST and MAY OR MAY NOT be partially or completely filled(sold);

    //get all 'posted2' status SELLs for each coin
    for (int c=0;c<mBuyTypes.length();c++) {
        QString currCoin = mBuyTypes.at(c);
        sayGdaxAutoTrader("Checking "+currCoin+" Sells For Fills",currCoin);
        //get all 'posted2' status Sells
        QList<QString> sellList;
        sellList.clear();
        mParent->getDb()->getGdaxAutoSellsPosted(currCoin,&sellList);
        sayGdaxAutoTrader(currCoin+" Sells: "+QString().setNum(sellList.count()),currCoin);
        //send a /fills/order-id for each placed2 status buy
        for(int o=0;o<sellList.count();o++){
            mParent->getGdaxHandler()->fetchGdaxSellFillsForOrderId(sellList.at(o));
        }
    }

}

void gdaxAutoTrader::autoTradeCheck() {
    if ( mParent->mAutoGDAXTrade == false ) { return; }
    //min LTC Buy is 0.01 LTC
    //min BTC Buy is 0.01 BTC
    //min ETH Buy is 0.01 ETH
    int hourRange = 8;
    QString USDBalance("0.00");
    //Dont forget to check mUSDStartAmount
    for ( int a=0;a<mParent->getGdaxHandlerAccount()->getWalletCount();a++ ) {
        if ( mParent->getGdaxHandlerAccount()->getWallet(a)->mCurrency == "USD" ) {
            USDBalance = mParent->getGdaxHandlerAccount()->getWallet(a)->mAvailable;  //Available is what we care about
        }
    }

    if ( mUSDStartAmount == "0.00" ) {
        mUSDStartAmount = USDBalance;
    }
    for (int c=0;c<mBuyTypes.length();c++) {
        QString currCoin = mBuyTypes.at(c);
        sayGdaxAutoTrader("# Available: " + USDBalance,currCoin);
        //collect historical ranges from db (20s interval = 3 per minute, 180 per hour)
        QList<QString> lastPriceRange;
        QList<QString> bidRange;
        QList<QString> askRange;
        QString curPrice("0.00");
        QString curAsk("0.00");
        QString curBid("0.00");
        mParent->getDb()->getGdaxPriceHistoryLast(currCoin,180*hourRange,&lastPriceRange,&bidRange,&askRange);


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
        sayGdaxAutoTrader("# Price: $" + curPrice,currCoin );
        sayGdaxAutoTrader("# Ask: $" + curAsk,currCoin );
        sayGdaxAutoTrader("# Bid: $" + curBid,currCoin );
        if ( USDBalance.toDouble() < mMinUSDBuyAmount  ) {
            sayGdaxAutoTrader("# Available $USD too low (< $"+QString().setNum(mMinUSDBuyAmount)+")",currCoin);
            continue;
        }
        QString howMuchToSpend("0.00");
        if ( USDBalance.toDouble() * 0.10 > mMinUSDBuyAmount && ((USDBalance.toDouble() * 0.10) / curBid.toDouble()) > 0.01 ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.10);
        } else if ( USDBalance.toDouble() * 0.20 > mMinUSDBuyAmount && ((USDBalance.toDouble() * 0.20) / curBid.toDouble()) > 0.01 ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.20);
        } else if ( USDBalance.toDouble() * 0.30 > mMinUSDBuyAmount && ((USDBalance.toDouble() * 0.30) / curBid.toDouble()) > 0.01 ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.30);
        } else if ( USDBalance.toDouble() * 0.40 > mMinUSDBuyAmount && ((USDBalance.toDouble() * 0.40) / curBid.toDouble()) > 0.01 ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.40);
        } else if ( USDBalance.toDouble() * 0.50 > mMinUSDBuyAmount && ((USDBalance.toDouble() * 0.50) / curBid.toDouble()) > 0.01 ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.50);
        } else if ( USDBalance.toDouble() * 0.75 > mMinUSDBuyAmount && ((USDBalance.toDouble() * 0.75) / curBid.toDouble()) > 0.01 ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.75);        
        } else if ( USDBalance.toDouble() * 0.90 > mMinUSDBuyAmount && ((USDBalance.toDouble() * 0.90) / curBid.toDouble()) > 0.01 ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble() * 0.90);
        } else if ( USDBalance.toDouble() > mMinUSDBuyAmount ) {
            howMuchToSpend = QString().setNum(USDBalance.toDouble());
        } else {
            sayGdaxAutoTrader("# Available $USD too low (< $"+QString().setNum(mMinUSDBuyAmount)+")",currCoin);
            break;
        }
        //shave off more than .00
        if ( howMuchToSpend.indexOf(".",0) != -1 ) {
            //it has at least one deci
            QString pre=howMuchToSpend.mid(0,howMuchToSpend.indexOf(".",0));
            QString post= howMuchToSpend.mid(howMuchToSpend.indexOf(".",0)+1,2);
            howMuchToSpend = pre + "." + post;
        }
        sayGdaxAutoTrader("Can Buy " + QString().setNum(howMuchToSpend.toDouble() / curBid.toDouble()) + " Of " + currCoin + " for $" + howMuchToSpend,currCoin );
        sayGdaxAutoTrader("# Allocated $" + howMuchToSpend +" For " + currCoin,currCoin);
        sayGdaxAutoTrader("#################",currCoin);
        sayGdaxAutoTrader("# Analyzing Price History",currCoin);
        sayGdaxAutoTrader("# Coin: " + currCoin,currCoin);



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
                        //# IfBuy { 0.20004 }
                        //# AtPrice { 49.99 }
                        //# For { 10 }
                        //# MinProfit { $0.025 }
                        //# Sell Total { $10.025 }
                        //# Sell Target { $50.115 }
                            // 12.5 cents.. imagine that... profit sucks though
                        // By changing the minProfit percent, along with minUsdBuyAmount
                        // User could control with precision. (TODO: add spinners & setters)

        //Determine buffer actual
        double highBuffer,lowBuffer,gap;
        gap = highPrice.toDouble() - lowPrice.toDouble();
        //These percentages are critical
        //They need to be configurable in the GUI but for now I am experimenting with them
        // Value Log Over time:
        // .20 (20%) led to $10 = 2 cents profit, $100 = 30cents profit, $200 = 60 cents profit (moderately quick turnaround)
        // .10 (10%) ... led to same profit range per value... and allowed it to buy during a surge at the first quick drop (and sell was immediate) (looks good)
        highBuffer = highPrice.toDouble() - (gap * 0.15);
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
        if ( belowHighest && belowHighBuffer && aboveLowest && aboveLowBuffer && belowLastBuyThisCoin && belowLastSellThisCoin ) {
                    //Determine Sell Price (from minProfitPercent)
                    double totalBuyAmount = howMuchToSpend.toDouble() / curBid.toDouble();
                    if ( totalBuyAmount < 0.01 ) { sayGdaxAutoTrader("# Cannot Afford 0.01 of "+ currCoin,currCoin); continue; }
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
                    //   -> id, coin, type, status, amount, buyPrice,buyTotal, sellTarget, sellTotal, minProfitPct, minProfitUsd, timePlaced, timeBought, timeSellPlaced, timeSold
                        //insert buy into db with SELLTARGET (update when buyresponse comes in(status), and updateAgain when SOLD)
                    int lastId=0;
                    double pct = (mMinPercentProfit * 100.0);
                    mParent->getDb()->insertGdaxAutoTrade(currCoin,"BUY","placed",QString().setNum(totalBuyAmount),curBid,QString().setNum(totalBuyCost),QString().setNum(sellTarget),QString().setNum(sellTotal),QString().setNum(pct),QString().setNum(profNeeded));
                    lastId = mParent->getDb()->getLastIdForTable("gdaxAutoTraderHistory");
                    sayGdaxAutoTrader("TradeId: " + QString().setNum(lastId),currCoin);
                    //PLACE Buy @ curBID (no fee)
//                    if ( currCoin == "BTC") { mLastBuyPriceBTC = curBid.toDouble(); }
//                    if ( currCoin == "LTC") { mLastBuyPriceLTC = curBid.toDouble(); }
//                    if ( currCoin == "ETH") { mLastBuyPriceETH = curBid.toDouble(); }
                    mParent->getGdaxHandler()->placeGdaxAutoTraderLimitBuy(currCoin+"-USD",QString().setNum(totalBuyAmount),curBid,lastId);
                    //Since we just made a buy, lets update our wallets too
                    mParent->getGdaxHandler()->listGdaxAccountsSlot();
                    //Update USDBalance ( -howMuchToSpend )
                    USDBalance = QString().setNum(USDBalance.toDouble() - totalBuyCost);
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
