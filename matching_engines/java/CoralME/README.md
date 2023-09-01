# CoralME
A simple, fast and garbage-free matching engine order book that you can use as a starting point for your matching engines.

## What is it?
CoralME is an order book data-structure that matches orders based on price-time priority. It maintains limit orders resting in an order book until they are either canceled or filled. Whenever an order changes its state, a callback is issued to registered listeners.

## What people usually mean by the term _Matching Engine_?
Usually when people talk about a _Matching Engine_, what they are really referring to is the full solution for an electronic exchange. That would include gateways, drop copies, market data, balances, reports, monitors, margins, compliance, fees, etc. Plus the _messaging middleware_ to tie all these pieces together. In that context, **the matching engine is really just one of the many parts of an electronic exchange**. It is an important part, the central nervous systems of an exchange, which maintains orders resting inside order books, and match them when liquidity takers meet liquidity providers (i.e. market makers).

For a detailed discussion of how a **first-class electronic exchange** can be built from the ground up using the sequencer architecture you should refer to [this article](https://www.coralblocks.com/index.php/building-a-first-class-exchange-architecture-with-coralsequencer/).

## Quick Start
Refer to [Example.java](https://github.com/coralblocks/CoralME/blob/main/src/main/java/com/coralblocks/coralme/example/Example.java) for a bunch of order matching use-cases.

The [OrderBookTest.java](https://github.com/coralblocks/CoralME/blob/main/src/test/java/com/coralblocks/coralme/OrderBookTest.java) might give you some good ideas as well but I find the [Example.java](https://github.com/coralblocks/CoralME/blob/main/src/main/java/com/coralblocks/coralme/example/Example.java) easier to follow.

## Features
- Fast
- Garbage-free
- Callback oriented
- Price levels
- Price improvement for fills
- MARKET and LIMIT order types
- IOC, GTC and DAY
- MAKER (of liquidity) and TAKER (of liquidity) execution sides
- NORMAL, CROSSED, LOCKED, ONESIDED and EMPTY book states
- ClientOrderID and OrderID
- ExecutionID and ExecutionMatchID

## How can I check that it is zero garbage?
Check [NoGCTest.java](https://github.com/coralblocks/CoralME/blob/main/src/main/java/com/coralblocks/coralme/example/NoGCTest.java) to see that it creates a book and populates this book with 10 orders _one million times_. And on each of these one million times it does a bunch of executions, rejects, cancelations, reduces, etc. Run this test with `-verbose:gc -Xms128m -Xmx256m` and you will always see **zero GC activity**. _No matter how many iterations you perform, the gc activity is always zero_. If you want to see some GC activity, you can turn on a flag that forces the creation of garbage by [producing some strings](https://github.com/coralblocks/CoralME/blob/bb9461313537987db43339e429b7314e58bbb784/src/main/java/com/coralblocks/coralme/example/NoGCTest.java#L103) in the middle of the loop.

##### Creating ZERO garbage
```
$ ./bin/runGCTest.sh
java -verbose:gc -Xms128m -Xmx256m -cp target/classes com.coralblocks.coralme.example.NoGCTest false 1000000
1000000 ... DONE!
```
##### Forcing the creation of garbage (pass true to runGCTest.sh)
```
$ ./bin/runGCTest.sh true
java -verbose:gc -Xms128m -Xmx256m -cp target/classes com.coralblocks.coralme.example.NoGCTest true 1000000
60870[GC (Allocation Failure)  33280K->1224K(125952K), 0.0005392 secs]
146061[GC (Allocation Failure)  34504K->1200K(125952K), 0.0005032 secs]
231254[GC (Allocation Failure)  34480K->1240K(125952K), 0.0003991 secs]
316449[GC (Allocation Failure)  34520K->1208K(125952K), 0.0004686 secs]
401642[GC (Allocation Failure)  34488K->1264K(125952K), 0.0004315 secs]
486832[GC (Allocation Failure)  34544K->1200K(129536K), 0.0004712 secs]
590373[GC (Allocation Failure)  41648K->1128K(129536K), 0.0005286 secs]
693917[GC (Allocation Failure)  41576K->1128K(128512K), 0.0002270 secs]
794836[GC (Allocation Failure)  40552K->1128K(129024K), 0.0002390 secs]
895759[GC (Allocation Failure)  40552K->1128K(129024K), 0.0002202 secs]
996679[GC (Allocation Failure)  40552K->1128K(129024K), 0.0002492 secs]
1000000 ... DONE!
```

## Callbacks Supported
```Java
public interface OrderBookListener {
    
    public void onOrderReduced(OrderBook orderBook, long time, Order order, 
                                 long reduceNewTotalSize);
    
    public void onOrderCanceled(OrderBook orderBook, long time, Order order, 
                                  CancelReason cancelReason);
    
    public void onOrderExecuted(OrderBook orderBook, long time, Order order, 
                                  ExecuteSide executeSide, long executeSize, 
                                  long executePrice, long executeId, long executeMatchId);
    
    public void onOrderAccepted(OrderBook orderBook, long time, Order order);
    
    public void onOrderRejected(OrderBook orderBook, long time, Order order, 
                                  RejectReason rejectReason);
    
    public void onOrderRested(OrderBook orderBook, long time, Order order,
                                long restSize, long restPrice);
    
}
```

## Code Snippet
```Java

    // This OrderBookListener will print all callbacks to System.out
    OrderBookLogger orderBookLogger = new OrderBookLogger();

    OrderBook orderBook = new OrderBook("AAPL", orderBookLogger);

    orderBookLogger.off(); // omit callbacks logging for clarity

    orderBook.createLimit("1", 1, Side.BUY, 200, 150.44, TimeInForce.DAY);
    orderBook.createLimit("2", 2, Side.BUY, 500, 149.44, TimeInForce.DAY);
    orderBook.createLimit("3", 3, Side.BUY, 100, 149.44, TimeInForce.GTC);
    orderBook.createLimit("4", 4, Side.BUY, 100, 148.14, TimeInForce.DAY);
    orderBook.createLimit("5", 5, Side.SELL, 300, 153.24, TimeInForce.GTC);
    orderBook.createLimit("6", 6, Side.SELL, 500, 156.43, TimeInForce.DAY);
    orderBook.createLimit("7", 7, Side.SELL, 1500, 158.54, TimeInForce.DAY);

    orderBook.showLevels();

    /*
       100 @    148.14 (orders=1)
       600 @    149.44 (orders=2)
       200 @    150.44 (orders=1)
    --------      2.80
       300 @    153.24 (orders=1)
       500 @    156.43 (orders=1)
      1500 @    158.54 (orders=1)		
    */

    orderBook.showOrders();

    /*
       100 @    148.14 (id=4)
       500 @    149.44 (id=2)
       100 @    149.44 (id=3)
       200 @    150.44 (id=1)
    --------      2.80
       300 @    153.24 (id=5)
       500 @    156.43 (id=6)
      1500 @    158.54 (id=7)
    */
    
    Order order = orderBook.getOrder(7);
    System.out.println(order);
    
    /*
    Order [id=7, clientOrderId=7, side=SELL, security=AAPL, originalSize=1500, 
            openSize=1500, executedSize=0, canceledSize=0, price=158.54, type=LIMIT, tif=DAY]
    */
    
    order.cancel(orderBook.getTimestamper().nanoEpoch());
    System.out.println(order);
    
    /*
    Order [id=7, clientOrderId=7, side=SELL, security=AAPL, originalSize=1500,
            openSize=0, executedSize=0, canceledSize=1500, price=158.54, type=LIMIT, tif=DAY]
    */
    
    orderBook.showOrders();
    
    /*
       100 @    148.14 (id=4)
       500 @    149.44 (id=2)
       100 @    149.44 (id=3)
       200 @    150.44 (id=1)
    --------      2.80
       300 @    153.24 (id=5)
       500 @    156.43 (id=6)
    */
    
    orderBookLogger.on(); // turn logging back on so we can see the callbacks
	    
    orderBook.createLimit("8", 8, Side.BUY, 1500, 155.00, TimeInForce.DAY);
    
    /*
        -----> onOrderAccepted called:
          orderBook=AAPL
          time=1682765163100000000
          order=Order [id=8, clientOrderId=8, side=BUY, security=AAPL, originalSize=1500, 
	  	openSize=1500, executedSize=0, canceledSize=0, price=155.0, type=LIMIT, tif=DAY]

        -----> onOrderExecuted called:
          orderBook=AAPL
          time=1682765163100000000
          order=Order [id=5, clientOrderId=5, side=SELL, security=AAPL, originalSize=300, 
	  	openSize=0, executedSize=300, canceledSize=0, price=153.24, type=LIMIT, tif=GTC]
          executeSide=MAKER
          executeSize=300
          executePrice=153.24
          executeId=1
          executeMatchId=1

        -----> onOrderExecuted called:
          orderBook=AAPL
          time=1682765163100000000
          order=Order [id=8, clientOrderId=8, side=BUY, security=AAPL, originalSize=1500, 
	  	openSize=1200, executedSize=300, canceledSize=0, price=155.0, type=LIMIT, tif=DAY]
          executeSide=TAKER
          executeSize=300
          executePrice=153.24
          executeId=2
          executeMatchId=1

        -----> onOrderRested called:
          orderBook=AAPL
          time=1682765163101000000
          order=Order [id=8, clientOrderId=8, side=BUY, security=AAPL, originalSize=1500, 
	  	openSize=1200, executedSize=300, canceledSize=0, price=155.0, type=LIMIT, tif=DAY]
          restSize=1200
          restPrice=155.0    
    */
	    
    orderBook.showOrders();
    
    /*
       100 @    148.14 (id=4)
       500 @    149.44 (id=2)
       100 @    149.44 (id=3)
       200 @    150.44 (id=1)
      1200 @    155.00 (id=8)       <==== Your order sat here after hitting some asks...  
    --------      1.43
       500 @    156.43 (id=6)    
    */
    
 ```
 

 
 






