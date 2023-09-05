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
import com.coralblocks.coralme.OrderBookAdapter;
import com.coralblocks.coralme.OrderBookListener;
import com.coralblocks.coralme.util.Timestamper;

/**
 * <p>Run this test with <b>-verbose:gc</b> and look for any GC activity. <b>You must not see any.</b></p>
 * 
 * <p>Alternatively you can pass <i>true</i> to createGarbage to see a lot of GC activity.</p>
 * 
 * <p>You should also decrease the max size of your heap memory so that if the GC has to kick in, it will do it sooner than later.</p>
 * 
 * <p>A good command-line example is:  <b><code>java -verbose:gc -Xms128m -Xmx256m -cp target/classes com.coralblocks.coralme.example.NoGCTest</code></b></p>
 */
public class NoGCTest {

	private static final boolean USE_BAD_SYSTEM_OUT_PRINT = false; // turn this on and you will see a lot of garbage from System.out.print
	private final static StringBuilder sb = new StringBuilder(1024);
	private static long orderId = 1;
	
	private static CharSequence getClientOrderId() {
		sb.setLength(0);
		sb.append(orderId);
		return sb;
	}
	
	private static void printWithoutGarbage(CharSequence cs) {
		int size = cs.length();
		for(int i = 0; i < size; i++) System.out.write(cs.charAt(i));
		System.out.flush();
	}
	
	private static void printIteration(int x) {
		
		sb.setLength(0);
		sb.append('\r').append(x); // does not produce garbage
		
		if (USE_BAD_SYSTEM_OUT_PRINT) {
			System.out.print(sb); // produces garbage!
		} else {
			printWithoutGarbage(sb);
		}
	}
	
	public static void main(String[] args) {
		
		boolean createGarbage = args.length >= 1 ? Boolean.parseBoolean(args[0]) : false;
		int iterations = args.length >= 2 ? Integer.parseInt(args[1]) : 1000000;
		
		OrderBookListener noOpListener = new OrderBookAdapter();
		
		OrderBook book = new OrderBook("AAPL", noOpListener);
		
		for(int i = 1; i <= iterations; i++) {
			
			printIteration(i);
			
			Timestamper ts = book.getTimestamper();
			
			// Bids:
			book.createLimit(getClientOrderId(),  orderId++,  Side.BUY, 1000, 100.00, TimeInForce.DAY);
			book.createLimit(getClientOrderId(),  orderId++,  Side.BUY,  900,  99.00, TimeInForce.DAY);
			book.createLimit(getClientOrderId(),  orderId++,  Side.BUY,  800,  98.00, TimeInForce.DAY);
			book.createLimit(getClientOrderId(),  orderId++,  Side.BUY,  700,  97.00, TimeInForce.DAY);
			book.createLimit(getClientOrderId(),  orderId++,  Side.BUY,  500,  95.00, TimeInForce.DAY);
			
			// Asks:
			book.createLimit(getClientOrderId(),  orderId++,  Side.SELL,  500, 102.00, TimeInForce.DAY);
			book.createLimit(getClientOrderId(),  orderId++,  Side.SELL,  400, 104.00, TimeInForce.DAY);
			book.createLimit(getClientOrderId(),  orderId++,  Side.SELL,  800, 105.00, TimeInForce.DAY);
			book.createLimit(getClientOrderId(),  orderId++,  Side.SELL,  700, 108.00, TimeInForce.DAY);
			book.createLimit(getClientOrderId(),  orderId++, Side.SELL,  500, 115.00, TimeInForce.DAY);
			
			// Hit top of book with IOCs:
			book.createLimit(getClientOrderId(),  orderId++,  Side.BUY,  600, 103.00, TimeInForce.IOC);
			book.createLimit(getClientOrderId(),  orderId++,  Side.SELL, 900, 96.00,  TimeInForce.IOC);
			
			// Reduce and cancel top of book orders
			Order bidOrder = book.getBestBidOrder();
			Order askOrder = book.getBestAskOrder();
			
			if (createGarbage) {
				// create some garbage for the garbage collector
				sb.setLength(0);
				sb.append("someGarbage"); // appending a CharSequence does not produce garbage
				for(int x = 0; x < 10; x++) sb.toString(); // but this produces garbage
			}
			
			bidOrder.reduceTo(ts.nanoEpoch(), 100);
			askOrder.reduceTo(ts.nanoEpoch(), 100);
			
			bidOrder.cancel(ts.nanoEpoch());
			askOrder.cancel(ts.nanoEpoch());
			
			// Order rejects due odd lot
			book.createLimit(getClientOrderId(),  orderId++,  Side.BUY,  620, 103.00, TimeInForce.DAY);
			book.createLimit(getClientOrderId(),  orderId++,  Side.SELL, 940, 96.00,  TimeInForce.DAY);
			
			// Add a couple of more orders in the middle of the book
			book.createLimit(getClientOrderId(),  orderId++,  Side.BUY,  600, 96.00,  TimeInForce.DAY);
			book.createLimit(getClientOrderId(),  orderId++,  Side.SELL, 990, 111.00, TimeInForce.DAY);
			
			// Now use a market order to remove all liquidity from both sides
			book.createMarket(getClientOrderId(),  orderId++,  Side.BUY,  15000);
			book.createMarket(getClientOrderId(),  orderId++,  Side.SELL, 15000);
			
			// Book must now be empty
			if (!book.isEmpty()) throw new IllegalStateException("Book must be empty here!");
		}
		
		System.out.println(" ... DONE!");
	}
}
