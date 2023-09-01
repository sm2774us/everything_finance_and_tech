/* 
 * Copyright 2023 (c) CoralBlocks - http://www.coralblocks.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 */
package com.coralblocks.coralme.example;

import com.coralblocks.coralme.Order;
import com.coralblocks.coralme.Order.Side;
import com.coralblocks.coralme.Order.TimeInForce;
import com.coralblocks.coralme.OrderBook;
import com.coralblocks.coralme.OrderBookLogger;

public class Example {
	
	public static void main(String[] args) {

		long orderId = 0;
		
		// This OrderBookListener will print all callbacks to System.out
		OrderBookLogger orderBookLogger = new OrderBookLogger();
		
		OrderBook orderBook = new OrderBook("AAPL", orderBookLogger);
		
		orderBook.createLimit(String.valueOf(++orderId), orderId, Side.BUY, 200, 150.44, TimeInForce.DAY);
		
		/*
			-----> onOrderAccepted called:
			  orderBook=AAPL
			  time=1682731006570000000
			  order=Order [id=1, clientOrderId=1, side=BUY, security=AAPL, originalSize=200, openSize=200, 
			  				executedSize=0, canceledSize=0, price=150.44, type=LIMIT, tif=DAY]
			
			-----> onOrderRested called:
			  orderBook=AAPL
			  time=1682731006572000000
			  order=Order [id=1, clientOrderId=1, side=BUY, security=AAPL, originalSize=200, openSize=200, 
			  				executedSize=0, canceledSize=0, price=150.44, type=LIMIT, tif=DAY]
			  restSize=200
			  restPrice=150.44
		*/
		
		orderBook.showLevels();
		
		/*
		   200 @    150.44 (orders=1)
		-------- 
		*/
		
		orderBook.showOrders();
		
		/*
		   200 @    150.44 (id=1)
		-------- 			  
		*/
		
		orderBook.createLimit(String.valueOf(++orderId), orderId, Side.BUY, 500, 149.44, TimeInForce.DAY);
		
		/* 
			-----> onOrderAccepted called:
			  orderBook=AAPL
			  time=1682731006574000000
			  order=Order [id=2, clientOrderId=2, side=BUY, security=AAPL, originalSize=500, openSize=500, 
			  				executedSize=0, canceledSize=0, price=149.44, type=LIMIT, tif=DAY]
			
			-----> onOrderRested called:
			  orderBook=AAPL
			  time=1682731006574000000
			  order=Order [id=2, clientOrderId=2, side=BUY, security=AAPL, originalSize=500, openSize=500, 
			  				executedSize=0, canceledSize=0, price=149.44, type=LIMIT, tif=DAY]
			  restSize=500
			  restPrice=149.44	
		*/

		orderBookLogger.off(); // omit callbacks output for clarity
		
		orderBook.createLimit(String.valueOf(++orderId), orderId, Side.BUY, 100, 149.44, TimeInForce.GTC);
		orderBook.createLimit(String.valueOf(++orderId), orderId, Side.BUY, 100, 148.14, TimeInForce.DAY);
		
		orderBook.createLimit(String.valueOf(++orderId), orderId, Side.SELL, 300, 153.24, TimeInForce.GTC);
		orderBook.createLimit(String.valueOf(++orderId), orderId, Side.SELL, 500, 156.43, TimeInForce.DAY);
		orderBook.createLimit(String.valueOf(++orderId), orderId, Side.SELL, 1500, 158.54, TimeInForce.DAY);
		
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
		
		orderBookLogger.on();
		
		// Buy 100 @ market
		
		orderBook.createMarket(String.valueOf(++orderId), orderId, Side.BUY, 100);
		
		/*
			-----> onOrderAccepted called:
			  orderBook=AAPL
			  time=1682731732570000000
			  order=Order [id=8, clientOrderId=8, side=BUY, security=AAPL, originalSize=100, openSize=100, 
			  				executedSize=0, canceledSize=0, type=MARKET]
			
			-----> onOrderExecuted called:
			  orderBook=AAPL
			  time=1682731732570000000
			  order=Order [id=5, clientOrderId=5, side=SELL, security=AAPL, originalSize=300, openSize=200, 
			  				executedSize=100, canceledSize=0, price=153.24, type=LIMIT, tif=GTC]
			  executeSide=MAKER
			  executeSize=100
			  executePrice=153.24
			  executeId=1
			  executeMatchId=1
			
			-----> onOrderExecuted called:
			  orderBook=AAPL
			  time=1682731732570000000
			  order=Order [id=8, clientOrderId=8, side=BUY, security=AAPL, originalSize=100, openSize=0, 
			  				executedSize=100, canceledSize=0, type=MARKET]
			  executeSide=TAKER
			  executeSize=100
			  executePrice=153.24
			  executeId=2
			  executeMatchId=1
		*/
		
		orderBook.showLevels();
		
		/*
		   100 @    148.14 (orders=1)
		   600 @    149.44 (orders=2)
		   200 @    150.44 (orders=1)
		--------      2.80
		   200 @    153.24 (orders=1)
		   500 @    156.43 (orders=1)
		  1500 @    158.54 (orders=1) 
		*/
		
		// reduce order with id = 1 to 100 shares
		
		Order order = orderBook.getOrder(1);
		order.reduceTo(orderBook.getTimestamper().nanoEpoch(), 100);
		
		/*
			-----> onOrderReduced called:
			  orderBook=AAPL
			  time=1682731732572000000
			  order=Order [id=1, clientOrderId=1, side=BUY, security=AAPL, originalSize=200, openSize=100, 
			  				executedSize=0, canceledSize=100, price=150.44, type=LIMIT, tif=DAY]
			  reduceNewTotalSize=100	
		*/
		
		orderBook.showLevels();
		
		/*
		   100 @    148.14 (orders=1)
		   600 @    149.44 (orders=2)
		   100 @    150.44 (orders=1)
		--------      2.80
		   200 @    153.24 (orders=1)
		   500 @    156.43 (orders=1)
		  1500 @    158.54 (orders=1)	 
		*/
		
		// now cancel the order
		
		order.cancel(orderBook.getTimestamper().nanoEpoch());
		
		/*
			-----> onOrderCanceled called:
			  orderBook=AAPL
			  time=1682731732572000000
			  order=Order [id=1, clientOrderId=1, side=BUY, security=AAPL, originalSize=200, openSize=0, 
			  				executedSize=0, canceledSize=200, price=150.44, type=LIMIT, tif=DAY]
			  cancelReason=USER	 
		*/
		
		orderBook.showLevels();
		
		/*
		   100 @    148.14 (orders=1)
		   600 @    149.44 (orders=2)
		--------      3.80
		   200 @    153.24 (orders=1)
		   500 @    156.43 (orders=1)
		  1500 @    158.54 (orders=1)	
		*/
		
		// hit the sell side of the book with a LIMIT IOC and notice your price improvement
		
		orderBook.createLimit(String.valueOf(++orderId), orderId, Side.BUY, 3000, 155.00, TimeInForce.IOC);
		
		/*
			-----> onOrderAccepted called:
			  orderBook=AAPL
			  time=1682731732573000000
			  order=Order [id=9, clientOrderId=9, side=BUY, security=AAPL, originalSize=3000, openSize=3000, 
			  				executedSize=0, canceledSize=0, price=155.0, type=LIMIT, tif=IOC]
			
			-----> onOrderExecuted called:
			  orderBook=AAPL
			  time=1682731732573000000
			  order=Order [id=5, clientOrderId=5, side=SELL, security=AAPL, originalSize=300, openSize=0, 
			  				executedSize=300, canceledSize=0, price=153.24, type=LIMIT, tif=GTC]
			  executeSide=MAKER
			  executeSize=200
			  executePrice=153.24
			  executeId=3
			  executeMatchId=2
			
			-----> onOrderExecuted called:
			  orderBook=AAPL
			  time=1682731732573000000
			  order=Order [id=9, clientOrderId=9, side=BUY, security=AAPL, originalSize=3000, openSize=2800, 
			  				executedSize=200, canceledSize=0, price=155.0, type=LIMIT, tif=IOC]
			  executeSide=TAKER
			  executeSize=200
			  executePrice=153.24
			  executeId=4
			  executeMatchId=2
			
			-----> onOrderCanceled called:
			  orderBook=AAPL
			  time=1682731732573000000
			  order=Order [id=9, clientOrderId=9, side=BUY, security=AAPL, originalSize=3000, openSize=0, 
			  				executedSize=200, canceledSize=2800, price=155.0, type=LIMIT, tif=IOC]
			  cancelReason=MISSED
		*/
		
		orderBook.showLevels();
		
		/*
		   100 @    148.14 (orders=1)
		   600 @    149.44 (orders=2)
		--------      6.99
		   500 @    156.43 (orders=1)
		  1500 @    158.54 (orders=1)		 
		*/
		
		orderBookLogger.off();
		
		orderBook.createLimit(String.valueOf(++orderId), orderId, Side.SELL, 3000, 160.00, TimeInForce.DAY);
		
		orderBook.showLevels();
		
		/*
		   100 @    148.14 (orders=1)
		   600 @    149.44 (orders=2)
		--------      6.99
		   500 @    156.43 (orders=1)
		  1500 @    158.54 (orders=1)
		  3000 @    160.00 (orders=1)
		*/
		
		// now hit two ask levels, price improve and sit on the book at 159.00
		
		orderBookLogger.on();
		
		orderBook.createLimit(String.valueOf(++orderId), orderId, Side.BUY, 3900, 159.00, TimeInForce.DAY);
		
		/*
			-----> onOrderAccepted called:
			  orderBook=AAPL
			  time=1682731732574000000
			  order=Order [id=11, clientOrderId=11, side=BUY, security=AAPL, originalSize=3900, openSize=3900, executedSize=0, canceledSize=0, price=159.0, type=LIMIT, tif=DAY]
			
			-----> onOrderExecuted called:
			  orderBook=AAPL
			  time=1682731732574000000
			  order=Order [id=6, clientOrderId=6, side=SELL, security=AAPL, originalSize=500, openSize=0, executedSize=500, canceledSize=0, price=156.43, type=LIMIT, tif=DAY]
			  executeSide=MAKER
			  executeSize=500
			  executePrice=156.43
			  executeId=5
			  executeMatchId=3
			
			-----> onOrderExecuted called:
			  orderBook=AAPL
			  time=1682731732574000000
			  order=Order [id=11, clientOrderId=11, side=BUY, security=AAPL, originalSize=3900, openSize=3400, executedSize=500, canceledSize=0, price=159.0, type=LIMIT, tif=DAY]
			  executeSide=TAKER
			  executeSize=500
			  executePrice=156.43
			  executeId=6
			  executeMatchId=3
			
			-----> onOrderExecuted called:
			  orderBook=AAPL
			  time=1682731732574000000
			  order=Order [id=7, clientOrderId=7, side=SELL, security=AAPL, originalSize=1500, openSize=0, executedSize=1500, canceledSize=0, price=158.54, type=LIMIT, tif=DAY]
			  executeSide=MAKER
			  executeSize=1500
			  executePrice=158.54
			  executeId=7
			  executeMatchId=4
			
			-----> onOrderExecuted called:
			  orderBook=AAPL
			  time=1682731732574000000
			  order=Order [id=11, clientOrderId=11, side=BUY, security=AAPL, originalSize=3900, openSize=1900, executedSize=2000, canceledSize=0, price=159.0, type=LIMIT, tif=DAY]
			  executeSide=TAKER
			  executeSize=1500
			  executePrice=158.54
			  executeId=8
			  executeMatchId=4
			
			-----> onOrderRested called:
			  orderBook=AAPL
			  time=1682731732575000000
			  order=Order [id=11, clientOrderId=11, side=BUY, security=AAPL, originalSize=3900, openSize=1900, executedSize=2000, canceledSize=0, price=159.0, type=LIMIT, tif=DAY]
			  restSize=1900
			  restPrice=159.0	 
		*/
		
		orderBook.showOrders();
		
		/*
		   100 @    148.14 (id=4)
		   500 @    149.44 (id=2)
		   100 @    149.44 (id=3)
		  1900 @    159.00 (id=11)    <==== You order sat here after hitting some asks...
		--------      1.00
		  3000 @    160.00 (id=10)	 
		*/
	}
}