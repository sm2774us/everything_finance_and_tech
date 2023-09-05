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

import java.util.ArrayList;
import java.util.List;

import com.coralblocks.coralme.util.CharEnum;
import com.coralblocks.coralme.util.CharMap;
import com.coralblocks.coralme.util.DoubleUtils;
import com.coralblocks.coralme.util.StringUtils;

public class Order {

	final static String EMPTY_CLIENT_ORDER_ID = "NULL";
	
	public final static int CLIENT_ORDER_ID_MAX_LENGTH = 64;
	
    private final List<OrderListener> listeners = new ArrayList<OrderListener>(64);
    
    private Side side;
    
    private long originalSize;
    
    private long totalSize;
    
    private long executedSize;
    
    private PriceLevel priceLevel;
    
    private final StringBuilder clientOrderId = new StringBuilder(CLIENT_ORDER_ID_MAX_LENGTH);
    
    private long price;
    
    private long acceptTime;
    
    private long restTime;
    
    private long cancelTime;
    
    private long rejectTime;
    
    private long reduceTime;
    
    private long executeTime;
    
    private long id;
    
    private String security;
    
    private TimeInForce tif;
    
    private Type type;
    
    Order next = null;
    
    Order prev = null;
    
    private boolean isResting;
    
    private boolean isPendingCancel;
    
    private long pendingSize;
    
    public Order() {
    	
    }
    
	public void init(CharSequence clientOrderId, long exchangeOrderId, String security, Side side, long size, long price, Type type, TimeInForce tif) {
    	
    	this.clientOrderId.setLength(0);
    	this.clientOrderId.append(clientOrderId);
    	
    	this.side = side;
    	
    	this.type = type;
    	
    	this.originalSize = this.totalSize = size;
    	
    	this.price = price;
    	
    	this.executedSize = 0;
    	
    	this.security = security;
    	
    	this.id = exchangeOrderId;
    	
    	this.acceptTime = -1;
    	
    	this.restTime = -1;
    	
    	this.reduceTime = -1;
    	
    	this.executeTime = -1;
    	
    	this.cancelTime = -1;
    	
    	this.rejectTime = -1;
    	
    	this.priceLevel = null;
    	
    	this.tif = tif;
    	
    	this.isResting = false;
    	
    	this.isPendingCancel = false;
    	
    	this.pendingSize = -1;
    }
	
	public final void setPendingCancel() {
		this.isPendingCancel = true;
	}
	
	public final void setPendingSize(long size) {
		this.pendingSize = size;
	}
	
	public final long getPendingSize() {
		return pendingSize;
	}
	
	public final boolean isPendingCancel() {
		return isPendingCancel;
	}
	
	public final boolean isResting() {
		
		return isResting;
	}
	
	public final double getPriceAsDouble() {
		
		return DoubleUtils.toDouble(price);
	}
	
    public final void setPriceLevel(PriceLevel priceLevel) {
    	
    	this.priceLevel = priceLevel;
    }
    
    public final PriceLevel getPriceLevel() {
    	
    	return priceLevel;
    }
    
    public final Type getType() {
    	
    	return type;
    }
    
    public final boolean isLimit() {
    	
    	return type == Type.LIMIT;
    }
    
    public final boolean isMarket() {
    	
    	return type == Type.MARKET;
    }
    
    public final long getOriginalSize() {
    	
    	return originalSize;
    }
    
    public final long getExecutedSize() {
    	
    	return executedSize;
    }
    
    public final long getFilledSize() {
    	
    	return executedSize;
    }
    
    public final long getOpenSize() {
    	
    	return totalSize - executedSize;
    }
    
    public final long getTotalSize() {
    	
    	return totalSize;
    }
    
    public final long getAcceptTime() {
    	
    	return acceptTime;
    }
    
    public final long getRestTime() {
    	
    	return restTime;
    }
    
    public final long getReduceTime() {
    	
    	return reduceTime;
    }
    
    public final long getExecuteTime() {
    	
    	return executeTime;
    }
    
    public final long getCancelTime() {
    	
    	return cancelTime;
    }
    
    public final long getRejectTime() {
    	
    	return rejectTime;
    }
    
    public final long getCanceledSize() {
    	
    	// originalSize = openSize + canceledSize + executedSize
    	return originalSize - getOpenSize() - executedSize;
    }
    
    public final boolean isTerminal() {
    	
    	return getOpenSize() == 0;
    }
    
    public final TimeInForce getTimeInForce() {
    	
    	return tif;
    }
    
    public final boolean isAccepted() {
    	
    	return id > 0;
    }
    
    public final boolean isIoC() {
    	
    	return tif == TimeInForce.IOC;
    }
    
    public final boolean isDay() {
    	
    	return tif == TimeInForce.DAY;
    }
    
    public final boolean isGTC() {
    	
    	return tif == TimeInForce.GTC;
    }
    
    public final long getPrice() {
    	
    	return price;
    }
    
    public final Side getSide() {
    	
    	return side;
    }
    
    public final Side getOtherSide() {
    	
    	return side == Side.BUY ? Side.SELL : Side.BUY;
    }
    
    public final long getId() {
    	
    	return id;
    }
    
    public final long getExchangeOrderId() {
    	
    	return id;
    }
    
    public final CharSequence getClientOrderId() {
    	
    	return clientOrderId;
    }
    
    public final String getSecurity() {
    	
    	return security;
    }
    
    public final TimeInForce getTif() {
    	
    	return tif;
    }
    
    public void addListener(OrderListener listener) {
    	
    	/*
    	 * It is very important that the OrderListener from OrderBook be executed LAST, in other words, it 
    	 * should be executed AFTER the OrderListener from PriceLevel, so the level can be removed if empty.
    	 * 
    	 * That's why the listeners will be called from last to first.
    	 */
    	
        listeners.add(listener);
    }
    
    public void accept(long time, long id) {
    	
    	this.id = id;
    	
    	this.acceptTime = time;
        
        int x = listeners.size();
        
        for(int i = x - 1; i >= 0; i--) {
        	
        	listeners.get(i).onOrderAccepted(time, this);
        }
    }
    
    public void rest(long time) {
    	
    	this.isResting = true;
    	
    	this.restTime = time;
        
        int x = listeners.size();
        
        for(int i = x - 1; i >= 0; i--) {
        	
        	listeners.get(i).onOrderRested(time, this, getOpenSize(), getPrice());
        }
    }
    
    public void reject(long time, RejectReason reason) {
    	
    	this.totalSize = this.executedSize = 0;
    	
    	this.rejectTime = time;
    	
        int x = listeners.size();
        
        for(int i = x - 1; i >= 0; i--) {
        	
        	listeners.get(i).onOrderRejected(time, this, reason);
        }
        
        listeners.clear();
    }
    
    public void reduceTo(long time, long newTotalSize) {
    	
    	if (newTotalSize <= executedSize) {
    		
    		cancel(time, CancelReason.USER);
    		
    		return;
    	}
    	
    	if (newTotalSize > totalSize) {
    		
    		newTotalSize = totalSize;
    	}
    	
    	this.totalSize = newTotalSize;
    	
    	this.reduceTime = time;
    	
    	int x = listeners.size();
    	
    	for(int i = x - 1; i >= 0; i--) {
    		
    		listeners.get(i).onOrderReduced(time, this, this.totalSize);
    	}
    }
    
    public void cancel(long time, long sizeToCancel) {
    	
    	cancel(time, sizeToCancel, CancelReason.USER);
    }
    
    public void cancel(long time, long sizeToCancel, CancelReason reason) {
    	
    	if (sizeToCancel >= getOpenSize()) {
    		
    		cancel(time, reason);
    		
    		return;
    	}
    	
    	long newSize = getOpenSize() - sizeToCancel + executedSize;
    	
    	this.totalSize = newSize;
    	
    	this.reduceTime = time;
    	
    	int x = listeners.size();
    	
    	for(int i = x - 1; i >= 0; i--) {
    		
    		listeners.get(i).onOrderReduced(time, this, newSize);
    	}
    }
    
    public void cancel(long time) {
    	
    	cancel(time, CancelReason.USER);
    }
    
    public void cancel(long time, CancelReason reason) {
    	
    	this.totalSize = this.executedSize;
    	
    	this.cancelTime = time;
    	
    	int x = listeners.size();
    	
    	for(int i = x - 1; i >= 0; i--) {
    		
    		listeners.get(i).onOrderCanceled(time, this, reason);
    	}
    	
    	listeners.clear();
    }
    
    public void execute(long time, long sizeToExecute) {
    	
    	execute(time, ExecuteSide.TAKER, sizeToExecute, this.price, -1, -1);
    }
    
    public void execute(long time, ExecuteSide execSide, long sizeToExecute, long priceExecuted, long executionId, long matchId) {
    	
    	if (sizeToExecute > getOpenSize()) {
    		
    		sizeToExecute = getOpenSize();
    	}
    	
    	this.executedSize += sizeToExecute;
    	
    	this.executeTime = time;
    	
    	int x = listeners.size();
    	
    	for(int i = x - 1; i >= 0; i--) {
    		
    		listeners.get(i).onOrderExecuted(time, this, execSide, sizeToExecute, priceExecuted, executionId, matchId);
    	}
    	
    	if (isTerminal()) {
    		
    		listeners.clear();
    	}
    }
    
	public static enum TimeInForce implements CharEnum { 

		GTC 			('T', "1"), 
		IOC				('I', "3"),
		DAY				('D', "0");

		private final char b;
		private final String fixCode;
		public final static CharMap<TimeInForce> ALL = new CharMap<TimeInForce>();
		
		static {
			for(TimeInForce tif : TimeInForce.values()) {
				if (ALL.put(tif.getChar(), tif) != null) throw new IllegalStateException("Duplicate: " + tif);
			}
		}
		
		private TimeInForce(char b, String fixCode) {
			this.b = b;
			this.fixCode = fixCode;
		}
		
		public static final TimeInForce fromFixCode(CharSequence sb) {
			for(TimeInForce s : TimeInForce.values()) {
				if (StringUtils.equals(s.getFixCode(), sb)) {
					return s;
				}
			}
			return null;
		}
		
    	@Override
        public final char getChar() {
    	    return b;
        }
    	
    	public final String getFixCode() {
    		return fixCode;
    	}
	}
	
	public static enum RejectReason implements CharEnum { 

		MISSING_FIELD		('1'),
		BAD_TYPE			('2'),
		BAD_TIF				('3'),
		BAD_SIDE			('4'),
		BAD_SYMBOL			('5'),
		
		BAD_PRICE 			('P'), 
		BAD_SIZE			('S'),
		TRADING_HALTED		('H'),
		BAD_LOT				('L'),
		UNKNOWN_SYMBOL		('U'),
		DUPLICATE_ORDER_ID	('D');

		private final char b;
		public final static CharMap<RejectReason> ALL = new CharMap<RejectReason>();
		
		static {
			for(RejectReason rr : RejectReason.values()) {
				if (ALL.put(rr.getChar(), rr) != null) throw new IllegalStateException("Duplicate: " + rr);
			}
		}
		
		private RejectReason(char b) {
			this.b = b;
		}
		
    	@Override
        public final char getChar() {
    	    return b;
        }
	}

	public static enum CancelRejectReason implements CharEnum {
		
		NOT_FOUND		('F');
		
		private final char b;
		public final static CharMap<CancelRejectReason> ALL = new CharMap<CancelRejectReason>();
		
		static {
			for(CancelRejectReason crr : CancelRejectReason.values()) {
				if (ALL.put(crr.getChar(), crr) != null) throw new IllegalStateException("Duplicate: " + crr);
			}
		}
		
		private CancelRejectReason(char b) {
			this.b = b;
		}
		
    	@Override
        public final char getChar() {
    	    return b;
        }
	}
	
	public static enum CancelReason implements CharEnum { 

		MISSED 			('M'), 
		USER			('U'),
		NO_LIQUIDITY	('L'),
		PRICE			('E'),
		PURGED			('P');

		private final char b;
		public final static CharMap<CancelReason> ALL = new CharMap<CancelReason>();
		
		static {
			for(CancelReason cr : CancelReason.values()) {
				if (ALL.put(cr.getChar(), cr) != null) throw new IllegalStateException("Duplicate: " + cr);
			}
		}
		
		private CancelReason(char b) {
			this.b = b;
		}
		
    	@Override
        public final char getChar() {
    	    return b;
        }
	}
	
	public static enum ReduceRejectReason implements CharEnum { 

		ZERO 			('Z'), 
		NEGATIVE		('N'),
		INCREASE		('I'),
		SUPERFLUOUS		('S'),
		NOT_FOUND		('F');

		private final char b;
		public final static CharMap<ReduceRejectReason> ALL = new CharMap<ReduceRejectReason>();
		
		static {
			for(ReduceRejectReason rrr : ReduceRejectReason.values()) {
				if (ALL.put(rrr.getChar(), rrr) != null) throw new IllegalStateException("Duplicate: " + rrr);
			}
		}
		
		private ReduceRejectReason(char b) {
			this.b = b;
		}
		
    	@Override
        public final char getChar() {
    	    return b;
        }
	}
	
	public static enum Type implements CharEnum { 

		MARKET 			('M', "1"), 
		LIMIT			('L', "2");

		private final char b;
		private final String fixCode;
		public final static CharMap<Type> ALL = new CharMap<Type>();
		
		static {
			for(Type t : Type.values()) {
				if (ALL.put(t.getChar(), t) != null) throw new IllegalStateException("Duplicate: " + t);
			}
		}
		
		private Type(char b, String fixCode) {
			this.b = b;
			this.fixCode = fixCode;
		}
		
		public static final Type fromFixCode(CharSequence sb) {
			for(Type s : Type.values()) {
				if (StringUtils.equals(s.getFixCode(), sb)) {
					return s;
				}
			}
			return null;
		}
		
    	@Override
        public final char getChar() {
    	    return b;
        }
    	
    	public final String getFixCode() {
    		return fixCode;
    	}
	}
	
	public static enum ExecuteSide implements CharEnum {
		
		TAKER			('T', "Y"),
		MAKER			('M', "N");
		
		private final char b;
		private final String fixCode;
		public final static CharMap<ExecuteSide> ALL = new CharMap<ExecuteSide>();
		
		static {
			for(ExecuteSide es : ExecuteSide.values()) {
				if (ALL.put(es.getChar(), es) != null) throw new IllegalStateException("Duplicate: " + es);
			}
		}
		
		private ExecuteSide(char b, String fixCode) {
			this.b = b;
			this.fixCode = fixCode;
		}
		
		public static final ExecuteSide fromFixCode(CharSequence sb) {
			for(ExecuteSide s : ExecuteSide.values()) {
				if (StringUtils.equals(s.getFixCode(), sb)) {
					return s;
				}
			}
			return null;
		}
		
    	@Override
        public final char getChar() {
    	    return b;
        }
    	
    	public final String getFixCode() {
    		return fixCode;
    	}
	}
	
	public static enum Side implements CharEnum { 

		BUY 			('B', "1", 0), 
		SELL			('S', "2", 1);

		private final char b;
		private final String fixCode;
		private final int index;
		public final static CharMap<Side> ALL = new CharMap<Side>();
		
		static {
			
			for(Side s : Side.values()) {
				if (ALL.put(s.getChar(), s) != null) throw new IllegalStateException("Duplicate: " + s);
			}
			
			if (ALL.size() != 2) {
				throw new IllegalStateException("Side must have only two values: BUY and SELL!");
			}
		}
		
		private Side(char b, String fixCode, int index) {
			this.b = b;
			this.fixCode = fixCode;
			this.index = index;
		}
		
		public static final Side fromFixCode(CharSequence sb) {
			for(Side s : Side.values()) {
				if (StringUtils.equals(s.getFixCode(), sb)) {
					return s;
				}
			}
			return null;
		}
		
    	@Override
        public final char getChar() {
    	    return b;
        }
    	
    	public final String getFixCode() {
    		return fixCode;
    	}
    
    	public final int index() {
    		return index;
    	}
    	
    	public final int invertedIndex() {
    		return this == BUY ? SELL.index() : BUY.index();
    	}
    	
    	public final boolean isBuy() {
    		return this == BUY;
    	}
    	
    	public final boolean isSell() {
    		return this == SELL;
    	}
    	
    	public final boolean isOutside(long price, long market) {
    		return this == BUY ? price < market : price > market;
    	}
    	
    	public final boolean isInside(long price, long market) {
    		return this == BUY ? price >= market : price <= market;
    	}
	}

	/**
	 * This method of course produces garbage and should be used only for debugging purposes.
	 * Use toCharSequence(StringBuilder) instead in order to avoid producing garbage.
	 * 
	 *  @return a newly created String object containing the information about this order instance
	 */
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder(256);
		return toCharSequence(sb).toString();
	}
	
	/**
	 * This method does not produce garbage. Re-use the same StringBuilder over and over again in order to avoid creating garbage.
	 * 
	 * @param sb the StringBuilder where the information will be written to.
	 * @return a CharSequence (i.e. the StringBuilder passed) containing the order information
	 */
	public CharSequence toCharSequence(StringBuilder sb) {
		sb.append("Order [id=").append(id).append(", clientOrderId=").append(clientOrderId).append(", side=")
			.append(side).append(", security=").append(security).append(", originalSize=").append(originalSize)
			.append(", openSize=").append(getOpenSize()).append(", executedSize=").append(executedSize)
			.append(", canceledSize=").append(getCanceledSize());
		
		if (type != Type.MARKET) {
			sb.append(", price=").append(DoubleUtils.toDouble(price));
		}
		
		sb.append(", type=").append(type);
		
		if (type != Type.MARKET) {
			sb.append(", tif=").append(tif);
		}
			
		sb.append("]");
		
		return sb;
	}
}
