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

import static org.junit.Assert.*;

import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mockito;

import com.coralblocks.coralme.Order.CancelReason;
import com.coralblocks.coralme.Order.ExecuteSide;
import com.coralblocks.coralme.Order.RejectReason;
import com.coralblocks.coralme.Order.Side;
import com.coralblocks.coralme.Order.TimeInForce;
import com.coralblocks.coralme.util.DoubleUtils;


public class OrderBookTest {

	
	private OrderBookListener called(OrderBookListener listener, int times) {
		return Mockito.verify(listener, Mockito.times(times));
	}
	
	private void done(OrderBookListener listener) {
		Mockito.verifyNoMoreInteractions(listener);
	}
	
	private static class OrderExecutedCaptor {
		
		ArgumentCaptor<OrderBook> 		book = ArgumentCaptor.forClass(OrderBook.class);
		ArgumentCaptor<Long> 			time = ArgumentCaptor.forClass(Long.class);
		ArgumentCaptor<Order> 			order = ArgumentCaptor.forClass(Order.class);
		ArgumentCaptor<ExecuteSide>		executeSide = ArgumentCaptor.forClass(ExecuteSide.class);
		ArgumentCaptor<Long> 			executeSize = ArgumentCaptor.forClass(Long.class);
		ArgumentCaptor<Long>  			executePrice = ArgumentCaptor.forClass(Long.class);
		ArgumentCaptor<Long>  			executeId = ArgumentCaptor.forClass(Long.class);
		ArgumentCaptor<Long>  			executeMatchId = ArgumentCaptor.forClass(Long.class);
	}
	
	@Test
	public void test_Order_Accepted() {
		
		OrderBookListener listener = Mockito.mock(OrderBookListener.class);
		
		OrderBook book = new OrderBook("AAPL", listener);
		
		Order order = book.createLimit("1", 1, Side.BUY, 800, 432.12, TimeInForce.DAY);
		
		called(listener, 1).onOrderAccepted(book, order.getAcceptTime(), order);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 1).onOrderRested(book, order.getRestTime(), order, order.getOriginalSize(), order.getPrice());
		
		done(listener);
	}
	
	@Test
	public void test_Order_Rejected_Due_To_Price() {
		
		OrderBookListener listener = Mockito.mock(OrderBookListener.class);
		
		OrderBook book = new OrderBook("AAPL", listener);
		
		// Market order cannot have a price
		Order order = book.createOrder("1", 1, Side.BUY, 800, DoubleUtils.toLong(432.12), Order.Type.MARKET, TimeInForce.DAY);
		
		called(listener, 0).onOrderAccepted(null, 0, null);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 1).onOrderRejected(book, order.getRejectTime(), order, RejectReason.BAD_PRICE);
		called(listener, 0).onOrderRested(null, 0, null, 0, 0);
		
		done(listener);
	}
	
	@Test
	public void test_Order_Rejected_Due_To_Odd_Lot() {
		
		OrderBookListener listener = Mockito.mock(OrderBookListener.class);
		
		OrderBook book = new OrderBook("AAPL", listener) {
			@Override
			protected RejectReason validateOrder(Order order) {
				if (order.getOriginalSize() % 100 != 0) {
					return RejectReason.BAD_LOT;
				}
				return null;
			}
		};
		
		Order order = book.createLimit("1", 1, Side.BUY, 800, 432.12, TimeInForce.DAY);
		
		called(listener, 1).onOrderAccepted(book, order.getAcceptTime(), order);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 1).onOrderRested(book, order.getRestTime(), order, order.getOriginalSize(), order.getPrice());
		
		done(listener);
		
		// Odd lot not accepted...
		order = book.createLimit("2", 2, Side.SELL, 850, 432.12, TimeInForce.DAY);
		
		called(listener, 0).onOrderAccepted(null, 0, null);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 1).onOrderRejected(book, order.getRejectTime(), order, RejectReason.BAD_LOT);
		called(listener, 0).onOrderRested(null, 0, null, 0, 0);
		
		done(listener);
	}
	
	@Test
	public void test_IoC_Order_Misses() {
		
		OrderBookListener listener = Mockito.mock(OrderBookListener.class);
		
		OrderBook book = new OrderBook("AAPL", listener);
		
		Order order = book.createLimit("1", 1, Side.BUY, 800, 432.12, TimeInForce.DAY);
		
		called(listener, 1).onOrderAccepted(book, order.getAcceptTime(), order);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 1).onOrderRested(book, order.getRestTime(), order, order.getOriginalSize(), order.getPrice());
		
		done(listener);
		
		order = book.createLimit("2", 2, Side.SELL, 100, 432.13, TimeInForce.IOC);
		
		called(listener, 1).onOrderAccepted(book, order.getAcceptTime(), order);
		called(listener, 1).onOrderCanceled(book, order.getCancelTime(), order, CancelReason.MISSED);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 0).onOrderRested(null,0, null, 0, 0);
		
		done(listener);
	}
	
	@Test
	public void test_Limit_Order_Rests() {
		
		OrderBookListener listener = Mockito.mock(OrderBookListener.class);
		
		OrderBook book = new OrderBook("AAPL", listener);
		
		Order order = book.createLimit("1", 1, Side.BUY, 800, 432.12, TimeInForce.DAY);
		
		called(listener, 1).onOrderAccepted(book, order.getAcceptTime(), order);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 1).onOrderRested(book, order.getRestTime(), order, order.getOriginalSize(), order.getPrice());
		
		done(listener);
		
		order = book.createLimit("2", 2, Side.SELL, 100, 432.13, TimeInForce.DAY);
		
		called(listener, 1).onOrderAccepted(book, order.getAcceptTime(), order);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 1).onOrderRested(book, order.getRestTime(), order, order.getOriginalSize(), order.getPrice());
		
		done(listener);
		
		assertEquals(1, book.getAskLevels());
		assertEquals(1, book.getBidLevels());
		assertEquals(OrderBook.State.NORMAL, book.getState());
		assertEquals(DoubleUtils.toLong(0.01), book.getSpread());
		assertEquals(DoubleUtils.toLong(432.12), book.getBestBidPrice());
		assertEquals(DoubleUtils.toLong(432.13), book.getBestAskPrice());
		assertEquals(800, book.getBestBidSize());
		assertEquals(100, book.getBestAskSize());
	}
	
	@Test
	public void test_Order_Reduced_Then_Canceled() {
		
		OrderBookListener listener = Mockito.mock(OrderBookListener.class);
		
		OrderBook book = new OrderBook("AAPL", listener);
		
		Order order = book.createLimit("1", 1, Side.BUY, 800, 432.12, TimeInForce.DAY);
		
		called(listener, 1).onOrderAccepted(book, order.getAcceptTime(), order);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 1).onOrderRested(book, order.getRestTime(), order, order.getOriginalSize(), order.getPrice());
		
		done(listener);
		
		order.reduceTo(book.getTimestamper().nanoEpoch(), 300);
		
		called(listener, 0).onOrderAccepted(null, 0, null);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 1).onOrderReduced(book, order.getReduceTime(), order, 300);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 0).onOrderRested(null, 0, null, 0, 0);
		
		done(listener);
		
		order.cancel(book.getTimestamper().nanoEpoch());
		
		called(listener, 0).onOrderAccepted(null, 0, null);
		called(listener, 1).onOrderCanceled(book, order.getCancelTime(), order, CancelReason.USER);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 0).onOrderRested(null, 0, null, 0, 0);
		
		done(listener);
	}
	
	@Test
	public void test_Limit_Order_Fills_With_Price_Improvement() {
		
		OrderBookListener listener = Mockito.mock(OrderBookListener.class);
		
		OrderBook book = new OrderBook("AAPL", listener);
		
		Order buyOrder = book.createLimit("1", 1, Side.BUY, 800, 432.12, TimeInForce.DAY);
		
		called(listener, 1).onOrderAccepted(book, buyOrder.getAcceptTime(), buyOrder);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 0).onOrderExecuted(null, 0, null, null, 0, 0, 0, 0);
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 1).onOrderRested(book, buyOrder.getRestTime(), buyOrder, buyOrder.getOriginalSize(), buyOrder.getPrice());
		
		done(listener);

		OrderExecutedCaptor captor = new OrderExecutedCaptor();
		
		Order sellOrder = book.createLimit("2", 2, Side.SELL, 100, 430.00, TimeInForce.DAY);
		
		called(listener, 1).onOrderAccepted(book, sellOrder.getAcceptTime(), sellOrder);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 2).onOrderExecuted(captor.book.capture(), captor.time.capture(), captor.order.capture(), captor.executeSide.capture(), 
											captor.executeSize.capture(), captor.executePrice.capture(), captor.executeId.capture(), 
											captor.executeMatchId.capture());
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 0).onOrderRested(null, 0, null, 0, 0);
		
		done(listener);
		
		assertEquals(book, captor.book.getAllValues().get(0));
		assertEquals(buyOrder.getExecuteTime(), captor.time.getAllValues().get(0).longValue());
		assertEquals(buyOrder, captor.order.getAllValues().get(0));
		assertEquals(ExecuteSide.MAKER, captor.executeSide.getAllValues().get(0));
		assertEquals(100, captor.executeSize.getAllValues().get(0).longValue());
		assertEquals(DoubleUtils.toLong(432.12), captor.executePrice.getAllValues().get(0).longValue());
		assertEquals(1, captor.executeId.getAllValues().get(0).longValue());
		assertEquals(1, captor.executeMatchId.getAllValues().get(0).longValue());
		
		assertEquals(book, captor.book.getAllValues().get(1));
		assertEquals(sellOrder.getExecuteTime(), captor.time.getAllValues().get(1).longValue());
		assertEquals(sellOrder, captor.order.getAllValues().get(1));
		assertEquals(ExecuteSide.TAKER, captor.executeSide.getAllValues().get(1));
		assertEquals(100, captor.executeSize.getAllValues().get(1).longValue());
		assertEquals(DoubleUtils.toLong(432.12), captor.executePrice.getAllValues().get(1).longValue());
		assertEquals(2, captor.executeId.getAllValues().get(1).longValue());
		assertEquals(1, captor.executeMatchId.getAllValues().get(1).longValue());
		
		Mockito.reset(listener);
		captor = new OrderExecutedCaptor();
		
		sellOrder = book.createLimit("3", 3, Side.SELL, 100, 430.00, TimeInForce.DAY);
		
		called(listener, 1).onOrderAccepted(book, sellOrder.getAcceptTime(), sellOrder);
		called(listener, 0).onOrderCanceled(null, 0, null, null);
		called(listener, 2).onOrderExecuted(captor.book.capture(), captor.time.capture(), captor.order.capture(), captor.executeSide.capture(), 
											captor.executeSize.capture(), captor.executePrice.capture(), captor.executeId.capture(), 
											captor.executeMatchId.capture());
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 0).onOrderRested(null, 0, null, 0, 0);
		
		done(listener);
		
		assertEquals(book, captor.book.getAllValues().get(0));
		assertEquals(buyOrder.getExecuteTime(), captor.time.getAllValues().get(0).longValue());
		assertEquals(buyOrder, captor.order.getAllValues().get(0));
		assertEquals(ExecuteSide.MAKER, captor.executeSide.getAllValues().get(0));
		assertEquals(100, captor.executeSize.getAllValues().get(0).longValue());
		assertEquals(DoubleUtils.toLong(432.12), captor.executePrice.getAllValues().get(0).longValue());
		assertEquals(3, captor.executeId.getAllValues().get(0).longValue());
		assertEquals(2, captor.executeMatchId.getAllValues().get(0).longValue());
		
		assertEquals(book, captor.book.getAllValues().get(1));
		assertEquals(sellOrder.getExecuteTime(), captor.time.getAllValues().get(1).longValue());
		assertEquals(sellOrder, captor.order.getAllValues().get(1));
		assertEquals(ExecuteSide.TAKER, captor.executeSide.getAllValues().get(1));
		assertEquals(100, captor.executeSize.getAllValues().get(1).longValue());
		assertEquals(DoubleUtils.toLong(432.12), captor.executePrice.getAllValues().get(1).longValue());
		assertEquals(4, captor.executeId.getAllValues().get(1).longValue());
		assertEquals(2, captor.executeMatchId.getAllValues().get(1).longValue());
	}
	
	@Test
	public void test_IoC_Partial_Fill() {
		
		OrderBookListener listener = Mockito.mock(OrderBookListener.class);
		
		OrderBook book = new OrderBook("AAPL", listener);
		
		Order buyOrder = book.createLimit("1", 1, Side.BUY, 800, 432.12, TimeInForce.DAY);
		book.createLimit("2", 2, Side.BUY, 400, 432.11, TimeInForce.DAY);
		
		Mockito.reset(listener);
		OrderExecutedCaptor captor = new OrderExecutedCaptor();
		
		Order sellOrder = book.createLimit("3", 3, Side.SELL, 1000, 432.12, TimeInForce.IOC);
		
		called(listener, 1).onOrderAccepted(book, sellOrder.getAcceptTime(), sellOrder);
		called(listener, 1).onOrderCanceled(book, sellOrder.getCancelTime(), sellOrder, CancelReason.MISSED);
		called(listener, 2).onOrderExecuted(captor.book.capture(), captor.time.capture(), captor.order.capture(), captor.executeSide.capture(), 
											captor.executeSize.capture(), captor.executePrice.capture(), captor.executeId.capture(), 
											captor.executeMatchId.capture());
		called(listener, 0).onOrderReduced(null, 0, null, 0);
		called(listener, 0).onOrderRejected(null, 0, null, null);
		called(listener, 0).onOrderRested(null, 0, null, 0, 0);
		
		done(listener);
		
		assertEquals(book, captor.book.getAllValues().get(0));
		assertEquals(buyOrder.getExecuteTime(), captor.time.getAllValues().get(0).longValue());
		assertEquals(buyOrder, captor.order.getAllValues().get(0));
		assertEquals(ExecuteSide.MAKER, captor.executeSide.getAllValues().get(0));
		assertEquals(800, captor.executeSize.getAllValues().get(0).longValue());
		assertEquals(DoubleUtils.toLong(432.12), captor.executePrice.getAllValues().get(0).longValue());
		assertEquals(1, captor.executeId.getAllValues().get(0).longValue());
		assertEquals(1, captor.executeMatchId.getAllValues().get(0).longValue());
		
		assertEquals(book, captor.book.getAllValues().get(1));
		assertEquals(sellOrder.getExecuteTime(), captor.time.getAllValues().get(1).longValue());
		assertEquals(sellOrder, captor.order.getAllValues().get(1));
		assertEquals(ExecuteSide.TAKER, captor.executeSide.getAllValues().get(1));
		assertEquals(800, captor.executeSize.getAllValues().get(1).longValue());
		assertEquals(DoubleUtils.toLong(432.12), captor.executePrice.getAllValues().get(1).longValue());
		assertEquals(2, captor.executeId.getAllValues().get(1).longValue());
		assertEquals(1, captor.executeMatchId.getAllValues().get(1).longValue());
		
		assertEquals(true, sellOrder.isTerminal());
		assertEquals(1, book.getNumberOfOrders());
		assertEquals(200, sellOrder.getCanceledSize());
		assertEquals(OrderBook.State.ONESIDED, book.getState());
		assertEquals(false, book.hasAsks());
	}
}