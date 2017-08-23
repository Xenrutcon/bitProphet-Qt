#include "cbautospottrader.h"

cbAutoSpotTrader::cbAutoSpotTrader(bitProphet *parent) : QObject(parent), mParent(parent) {
    mTradeTypes.append("BTC");
    mTradeTypes.append("LTC");
    mTradeTypes.append("ETH");
    mBTCLog = mParent->mParent->getAutoTraderBTCLog();
    mLTCLog = mParent->mParent->getAutoTraderLTCLog();
    mETHLog = mParent->mParent->getAutoTraderETHLog();
    say("# AutoSpot Trader Exists #");
}

cbAutoSpotTrader::~cbAutoSpotTrader() {
    say("# AutoSpot Trader Fades #");
}

///////////
// METHODS
//////////

void cbAutoSpotTrader::say(QString sayIt,QString coinLog) {
    //std::cout<<"[cbAutoSpotTrader] "<<sayIt.toStdString().c_str()<<std::endl;
    if ( coinLog == "BTC" ) {
        mBTCLog->append(sayIt);
    }else if ( coinLog == "LTC" ) {
        mLTCLog->append(sayIt);
    } else if ( coinLog == "ETH" ) {
        mETHLog->append(sayIt);
    } else {
        mBTCLog->append(sayIt);
        mLTCLog->append(sayIt);
        mETHLog->append(sayIt);
    }
}


/////////
// SLOTS
////////

void cbAutoSpotTrader::autoTradeCheck() {
    for (int c=0;c<mTradeTypes.length();c++){
        QString currCoin = mTradeTypes.at(c);
        QString currPrice;
        say("#################",currCoin);
        say("# Analyzing Price History",currCoin);
        say("# Coin: " + currCoin,currCoin);
        QList<QString> lastPriceRange;
        mParent->getDb()->getSpotPriceHistoryLast(currCoin,600,&lastPriceRange); //600 @ 5.5s is 1hour
        if ( lastPriceRange.length() > 0 ) {
            currPrice = lastPriceRange.last();
        }
        say("# " + QString().setNum(lastPriceRange.count()) + " Prices",currCoin);
        say("# Current: $" + currPrice,currCoin);
        say("#################",currCoin);
        //check for high price (ranged)
        QString high = findHighestPrice(lastPriceRange);
        say("# High: $" + high,currCoin);
        //check for low price (ranged)
        QString low = findLowestPrice(lastPriceRange);
        say("# Low : $" + low,currCoin);
        // Check the gap!
        QString gap = findPriceGap(lastPriceRange);
        say("# Gap : $" + gap,currCoin);

        // 1/10th of the gap is how far from the bottom and top we must be.
        // ie: if gap is 20(lo0hi20), 2 is 1/10th which means valid range is 2 to 18, 1 and
        QString tenthGap = QString().setNum( gap.toDouble() * 0.10 );
        say("# 10%ofGap : $" + tenthGap,currCoin);
        bool hiPass;
        bool loPass;
        // TODO add check for upward run (is prior price slightly lower?)
        //check range from top
        if ( high.toDouble() - tenthGap.toDouble() > lastPriceRange.last().toDouble() ) {
            hiPass = true;
        } else { hiPass = false; }
        //check range from bottom
        if ( low.toDouble() + tenthGap.toDouble() < lastPriceRange.last().toDouble() ) {
            loPass = true;
        } else { loPass = false; }
        //go or no
        if ( loPass && hiPass ) {
            say("# AutoSpot buying " + currCoin,currCoin);
        } else {
            say("# AutoSpot passes on " + currCoin,currCoin);
        }
    }
    //Next check our autoBuys,
        //quote sale price,
            //deduce profit,
            //compare to minProfit
                //sell or hodl

    //Finally, restart timer
    QTimer::singleShot(mParent->mAutoSpotTradeInterval,this,SLOT(autoTradeCheck()));
}

QString cbAutoSpotTrader::findLowestPrice(QList<QString> hayStack) {
    QString lowest = hayStack.at(0);
    for(int c=0;c<hayStack.length();c++) {
        if ( hayStack.at(c).toDouble() < lowest.toDouble() ) {
            lowest = hayStack.at(c);
        }
    }
    return lowest;
}

QString cbAutoSpotTrader::findHighestPrice(QList<QString> hayStack) {
    QString highest = hayStack.at(0);
    for(int c=0;c<hayStack.length();c++) {
        if ( hayStack.at(c).toDouble() > highest.toDouble() ) {
            highest = hayStack.at(c);
        }
    }
    return highest;
}

QString cbAutoSpotTrader::findPriceGap(QList<QString> hayStack) {
    QString gap;
    QString highest = findHighestPrice(hayStack);
    QString lowest = findLowestPrice(hayStack);
    gap = QString().setNum( highest.toDouble() - lowest.toDouble() );
    return gap;
}


//As you can see, its really hard math.... :P
//No it isnt.
