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

public class OrderBookAdapter implements OrderBookListener {
    
	@Override
    public void onOrderReduced(OrderBook orderBook, long time, Order order, long reduceNewTotalSize) {
    	
    }
    
	@Override
    public void onOrderCanceled(OrderBook orderBook, long time, Order order, CancelReason cancelReason) {
    	
    }
    
	@Override
    public void onOrderExecuted(OrderBook orderBook, long time, Order order, ExecuteSide executeSide, long executeSize, long executePrice, long executeId, long executeMatchId) {
    	
    }
    
	@Override
    public void onOrderAccepted(OrderBook orderBook, long time, Order order) {
    	
    }
    
	@Override
    public void onOrderRejected(OrderBook orderBook, long time, Order order, RejectReason rejectReason) {
    	
    }
    
	@Override
    public void onOrderRested(OrderBook orderBook, long time, Order order, long restSize, long restPrice) {
    	
    }
}
