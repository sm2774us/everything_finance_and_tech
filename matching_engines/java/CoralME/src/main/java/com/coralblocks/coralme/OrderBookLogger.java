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
package com.coralblocks.coralme;

import com.coralblocks.coralme.Order.CancelReason;
import com.coralblocks.coralme.Order.ExecuteSide;
import com.coralblocks.coralme.Order.RejectReason;
import com.coralblocks.coralme.util.DoubleUtils;

/**
 * This is a simple OrderBookListener that prints its callbacks to System.out for debugging. 
 */
public class OrderBookLogger implements OrderBookListener {
	
	private boolean isOn = true;
	
	/**
	 * Turns on logging to System.out
	 */
	public void on() {
		isOn = true;
	}
	
	/**
	 * Turns off logging to System.out
	 */
	public void off() {
		isOn = false;
	}
	
	/**
	 * Is currently logging to System.out?
	 * 
	 * @return true if logging
	 */
	public boolean isOn() {
		return isOn;
	}
    
	@Override
    public void onOrderReduced(OrderBook orderBook, long time, Order order, long reduceNewTotalSize) {
		if (!isOn) return;
    	System.out.println("-----> onOrderReduced called:");
    	System.out.println("  orderBook=" + orderBook);
    	System.out.println("  time=" + time);
    	System.out.println("  order=" + order);
    	System.out.println("  reduceNewTotalSize=" + reduceNewTotalSize);
    	System.out.println();
    }
    
	@Override
    public void onOrderCanceled(OrderBook orderBook, long time, Order order, CancelReason cancelReason) {
		if (!isOn) return;
    	System.out.println("-----> onOrderCanceled called:");
    	System.out.println("  orderBook=" + orderBook);
    	System.out.println("  time=" + time);
    	System.out.println("  order=" + order);
    	System.out.println("  cancelReason=" + cancelReason);
    	System.out.println();
    }
    
	@Override
    public void onOrderExecuted(OrderBook orderBook, long time, Order order, ExecuteSide executeSide, long executeSize, long executePrice, long executeId, long executeMatchId) {
		if (!isOn) return;
    	System.out.println("-----> onOrderExecuted called:");
    	System.out.println("  orderBook=" + orderBook);
    	System.out.println("  time=" + time);
    	System.out.println("  order=" + order);
    	System.out.println("  executeSide=" + executeSide);
    	System.out.println("  executeSize=" + executeSize);
    	System.out.println("  executePrice=" + DoubleUtils.toDouble(executePrice));
    	System.out.println("  executeId=" + executeId);
    	System.out.println("  executeMatchId=" + executeMatchId);
    	System.out.println();    	
    }
    
	@Override
    public void onOrderAccepted(OrderBook orderBook, long time, Order order) {
		if (!isOn) return;
    	System.out.println("-----> onOrderAccepted called:");
    	System.out.println("  orderBook=" + orderBook);
    	System.out.println("  time=" + time);
    	System.out.println("  order=" + order);
    	System.out.println();
    }
    
	@Override
    public void onOrderRejected(OrderBook orderBook, long time, Order order, RejectReason rejectReason) {
		if (!isOn) return;
    	System.out.println("-----> onOrderRejected called:");
    	System.out.println("  orderBook=" + orderBook);
    	System.out.println("  time=" + time);
    	System.out.println("  order=" + order);
    	System.out.println("  rejectReason=" + rejectReason);
    	System.out.println();
    }
    
	@Override
    public void onOrderRested(OrderBook orderBook, long time, Order order, long restSize, long restPrice) {
		if (!isOn) return;
    	System.out.println("-----> onOrderRested called:");
    	System.out.println("  orderBook=" + orderBook);
    	System.out.println("  time=" + time);
    	System.out.println("  order=" + order);
    	System.out.println("  restSize=" + restSize);
    	System.out.println("  restPrice=" + DoubleUtils.toDouble(restPrice));
    	System.out.println();
    }
}
