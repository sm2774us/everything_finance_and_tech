# Matching Engines

## Table Of Contents <a name="top"></a>

1. [Introduction](#introduction)  
2. [History](#history)
3. [Proliferation of Matching Engines](#proliferation-of-matching-engines)
4. [Terminology](#terminology)
5. [Order Types](#order-types)
<details>
  <summary>Show contents</summary>
   5.1   [Market Order](#market-order)
   5.2   [Limit Order](#limit-order)
   5.3   [Stop Order](#stop-order)
   5.4   [Cancellation of Limit or Stop Orders](#cancellation-of-limit-or-stop-orders)
</details>
6. [Rules for Matching Engines](#rules-for-matching-engines)
7. [Order Processing](#order-processing)
8. [Matching Engine Architecture](#matching-engine-architecture)
9. [Connecting to the Transaction Router](#connecting-to-the-transaction-router)
10. [Market Book](#market-book)
11. [Market Transactions](#market-transactions)
<details>
  <summary>Show contents</summary>
   11.1   [Order](#order)
   11.2   [Acknowledgment](#acknowledgment)
   11.3   [Order Cancel](#order-cancel)
   11.4   [Order Result](#order-result)
</details>
12. [References](#references)
13. [Matching Engine Implementations](#matching-engine-implementations)
<details>
  <summary>Show contents</summary>
   13.1   [C++](#c++)
   13.2   [Java](#java)
   13.3   [Python](#python)
</details>

### Introduction

The computer driven markets for instruments like stocks and exchange traded stock options, have transformed finance and the flow of capital. These markets are enabled by order matching engines (and the infrastructure that supports this software). Before computer trading networks and matching engines, stocks where traded on cavernous exchange floors and transaction costs where high. When electronic trading fully matured, floor traders were a fading anachronism and transaction costs had been reduced to pennies a share in many cases. Electronic trading could not exist without advanced network infrastructure, but without the software matching engines no shares would change hands. The computer trading networks, the matching engine software has also created a concentrated nexus of potential failure. Failures in these systems have increased as the frequency and volume on the electronic networks has increased. The position of order matching engines in the trading infrastructure makes these systems of interest not only to computer scientists but also to computational finance and risk management. Matching engines match buy and sell orders that are submitted to electronic trading networks, like NASDAQ. The electronic trading networks came being as the stock markets were being deregulated, with trading moving to penny increments.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### History

One of the first modern trading networks was Island (which was eventually purchased by NASDAQ). Josh Levine developed the software for the Island matching engine \[1\]. He originally wrote software to exploit the Small Order Execution System (SOES) and supported the “SOES Bandits” at a day trading firm named Datek. Historically the rise of electronic trading has been controversial because it trading threatened the system of market specialists, higher trading commissions and bid/ask spreads. Island differentiated itself from the other evolving electronic networks (at the time, Reuters Instinet and NASDAQ's SelectNet), by providing open access to any customer who could pay for connectivity, fast order execution and low transaction costs. At the heart of Island was Josh Levine's matching engine (which ran on MS-DOS). Early customers of Island included some of the hedge funds which pioneered high frequency trading, including Automated Trading Desk and Renaissance Technology. The electronic trading networks enabled high frequency trading which fed demand for faster transactions and higher trading volumes. The electronic trading infrastructure has evolved to support distributed parallel matching engines and extremely fast networks utilizing advanced technologies. High Frequency Trading is beyond the scope of this description.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Proliferation of Matching Engines

High frequency trading and sophisticated algorithmic trading has become a feature of the electronic markets \[1\]\[3\]. Some of this software is deployed in an environment that spares no expense when it comes to rapid order execution. The servers that host the software are co-located with the exchanges and specialized computer networks have been constructed to provide millisecond advantage for arbitraging trades between the exchanges in Chicago and New York \[8\]. Large institutional traders feel that they are at a disadvantage in such an environment \[3\]\[8\]. One response to this “arms race” in algorithmic software that exists on the electronic trading networks is to create trading networks that ban algorithmic traders and better support institutional traders. These “dark pool” networks \[1\] allow customers to trade with each other and do not publish trading information, except for trades that are routed to the “public” networks like NASDAQ and the NYSE. These limited subscriber networks make use of order matching engines in the same way that networks like Island and Archipelago have in the past.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Terminology

\*\*Bid\*\* – the price in a buy order  
\*\*Ask\*\* – the price in a sell order  
\*\*Spread\*\* – the gap between the bid and the ask price.  

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Order Types

Electronic trading networks support a variety of order types, some of which are customized to favor certain market participants \[3\]. In order to simplify the model, the Matching Engine discussed here supports only three order types (which account for most orders in the market).

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### Market Order

An order to buy or sell at the best available price at the time the order is submitted. A market order may be filled at multiple prices if the order is not filled in a single match.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### Limit Order

> _A limit order is an order to buy or sell a stock at a specific price or better. A buy limit order can only be executed at the limit price or lower, and a sell limit order can only be executed at the limit price or higher. A limit order is not guaranteed to execute. A limit order can only be filled if the stock’s market price reaches the limit price. \[5\]_

Limit orders may have a “time in force” value, after which the limit order will automatically expire. If this time is not defined, the limit order expires at the end of the trading day.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### Stop Order

A stop order becomes a market order once the market price reaches a point specified in the stop order. A Stop order does not guarantee a specific execution price and may execute significantly away from its stop price. A Sell Stop order is always usually placed below the current market price. Stop orders are sometimes called “stop loss” orders. Like a limit order, a stop order may have a “time in force” value, after which the stop order will automatically expire. If this value is not defined, the order will expire at the end of the trading day.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### Cancellation of limit or stop orders

A cancellation order cancels a limit or stop order (if the stop order has not converted to a limit order, in which case it cannot be canceled). Market orders cannot be canceled because their execution will take place immediately when there is a opposite order. Canceling a market order would be non-deterministic, so this is not supported.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Rules for Matching Engines

The model matching engine supports the following order queues:

*   Cancel order queue
*   Market order queues
    
    *   Buy
    *   Sell
*   Limit order queue
    
    *   Buy
    *   Sell
*   Stop order queue
    
    *   Buy
    *   Sell

Orders are processed in the order listed above. Cancel orders are processed first, followed by market order, limit orders and stop order. This section discusses the rules used by the order matching engine to execute order. These rules are derived from several sources, including \[4\].

*   Orders may be partially filled or not filled at all (in the case of a limit order).
*   After cancel orders, market orders are given the highest priority and will execute immediately when an opposite order exists in the market. Market orders may be partially filled, at different prices.
*   An attempt is always made to match a buy order first, against existing sell orders.
*   Limit orders are sorted:
    
    *   Buy orders are sorted in descending order by their bid price and ascending order by time stamp for orders that have the same price. Orders with the highest bid (buy) price are kept at the top of the queue and will be executed first. For equal priced bids, the order that arrives first is executed first.
    *   Sell orders are sorted in ascending order by their ask price, and like buy orders, by ascending order by time stamp for orders with the same price. Orders with the lowest sell (ask) prices will be sold first. For orders with the same ask price, the order that arrives first will be sold first.

When an order completes, either because it is filled, canceled or expires, a status message is returned to the agent that submitted the order. Whenever there is a market transaction there is a risk that a counter party will default on the transaction. The order exchanges, like NASDAQ and the NYSE, include multiple clearinghouses that act as transaction counter parties. Here the buyer and seller transact with the clearing house. If one party defaults, the clearing house will make good on the transaction. The clearing house collects a fee for this service, either via the bid/ask spread or a direct transaction cost. In the model Matching Engine these complexities are ignored and it is assumed that there is no counter party default, so no clearing houses are necessary. All transactions take place within the order matching engine.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Order Processing

The presence of market and limit orders complicates the trading rules and the process of market price formation. Some articles discuss mathematical market models without carefully defining the rules \[6\]. The result may be a mathematical model that does not actually correspond to real market behavior. In other papers \[7\] the model is incomplete:

> Investors arrive sequentially to trade one share of the risky asset via either a market order or a limit order. A market order can be placed only if a counterpart limit order exists, and in such a case the market order executes with certainty at the counterpart limit order’s price.

Here the authors do not consider the case where there are two opposite (a buy and a sell) market orders. Both of these orders have priority execution over limit orders and will execute at the market price, if this price is better than the limit order price.

Limit order matching is also complicated by the presence of both limit and market orders.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Matching Engine Architecture

The order matching system is connected to an electronic trading network. The participants in this network establish a connection (generally via a secure socket layer or SSL connection) to the market matching system. This system has the following components:

1.  The Transaction Router manages the TCP/IP socket connections to the market actors that submit orders and receive order status. When the Transaction Router receives an order, it submits it to the associated queue (Market, Limit or Cancel).
2.  The cancel queue contains the cancel requests.
3.  The Market Order Queue contains queues the market orders for execution. Internally the market order queue implements a queue for buy and sell orders. A market order is at least partially filled as soon as an opposite order is present in the market. Orders are executed in first in, first executed priority.
4.  The Limit Order Queue contains limit and stop orders. Internally this logical queue supports buy and sell queues for limit orders and stop orders. When the market price point is reached for a stop order, it is converted to a market order and submitted to the market order queue. The Limit Order Queue supports order cancellation and order expiration.
5.  The Order Matching Engine matches orders from the queues following the rules outlines above. When a order is filled or canceled, the status is reported back to the market actor by entering the status in the return status queue.

The “order book” consists of the orders at the heads of the queues. These orders represent the current best prices for bids and asks. Market prices are set either by the market open prices or by limit orders. Market orders have no target price, so they cannot define price in the market. The critical place of order matching engines in financial trading means that reliability and fault tolerance is a critical feature in a production matching engine. If there is hardware or software failure, it is critical that the state of completed orders must be maintained. Generally this means that the state of the matching engine will be stored in a database. The features for reliability and fault tolerance are omitted from the model matching engine discussed here. One of the challenges in designing a matching engine for real market transactions is managing the priority of the various queues. For example, cancel requests might be handled first, before any orders. However, some high frequency traders will flood the market with orders and cancels. In this case it would be possible for the cancel requests to temporarily freeze out market or limit orders. Similarly, market buy orders have the highest priority since they provide liquidity to the market. However, it would be possible for a stream of market orders to freeze out limit orders, which could be counter parties to market orders.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Connecting to the Transaction Router

The connections to the matching engine, via the Transaction Router, take place via TCP/IP sockets. A client connects to the Transaction Router via a known (predefined) TCP/IP port. The Transaction Router then creates a thread to handle the transactions from that client. In a real order matching system the connection process would have two steps:

1.  Create an authentication thread that authenticates the client (e.g., check user name and password or other credentials).
2.  If the authentication passes, an order processing thread would be created. If authentication fails an error would be returned.

The model matching engine does not support the authentication step, since this is only needed in a real production context. All clients are allowed to submit orders.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Market Book

An instance of the matching engine is created for each stock that is traded. This matching engine processes the orders for that stock. The result of the order processing is the order book with the current bid/ask spread. This spread is simply the last buy (bid) and the last sell (ask) price. In electronic matching engines this is no market maker. Traders who act like market makers, providing liquidity for the market and then trading when they can make a small profit may exist in an electronic market, but they are not required. As a result, electronic markets can, in theory, instantaneously have a bid/ask spread of zero.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Market Transactions

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### Order

1.  Customer ID (a unique identifier for the market customer)
2.  Stock ID (usually the CUSIP number)
3.  Order type (buy/sell)
4.  Order class (market, limit, stop)
5.  Unit price (i.e., target price for a limit order)
6.  Number of shares (to be bought or sold)
7.  Partial fill allowed (for limit orders): 1 = partial fill allowed, 0 = no partial fill
8.  Expire time (time the order should be active in the market, in minutes)

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### Acknowledgment

The order acknowledgment or an error status is returned immediately when the order is accepted. The acknowledgment is sent in response to order or cancels.

1.  Status (e.g., order accepted, error, etc...)
2.  Market order ID (identifier assigned by the market for this order)
3.  Time stamp (time the order was accepted)

The market order ID will be the identifier that is used in reporting the order result or in order cancellation.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### Order Cancel

1.  Customer ID
2.  Market order ID

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### Order Result

An order result is returned when an order is filled (or at least partially filled), when the order is canceled or when the order expires. Market orders (or stop orders that convert to market orders) may have more than one fill, at different prices.

1.  Customer ID
2.  Market order ID
3.  Time stamp
4.  Status: filled, expired, canceled
5.  Number of fills

*   {number of shares, fill price}

A limit order that allows a partial fill will complete when the limit order is partially filled.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### References

1. _Dark Pools: The Rise of the Machine Traders and the Rigging of the U.S. Stock Market_, by Scott Patterson, Crown Business, 2012 
2. _Practical .NET for Financial Markets_, Yogesh Shetty and Samir Jayaswal, Apress, 2006 
3. _The Problem of HFT: Collected Writings on High Frequency Tranding & Stock Market Structure Reform_, Haim Bodek, Decimus Capital Markets, 2012. 
4. _Eurex Exchange (http://www.eurexchange.com) Matching Principles_, Eurex Exchange. 
5. _SEC Web page on limit orders_: [http://www.sec.gov/answers/limit.htm](http://www.sec.gov/answers/limit.htm) 
6. _Limit order book models and optimal trading strategies_, Marcel Hoschler, Masters Thesis, University of Berlin, July 7, 2011 
7. _Quote setting and price formation in an order driven market_, Puneet Handa, Robert Schwartz, Ashish Tiwari, Journal of Financial Markets 6 (2003) 461–489 
8. _Stock Traders Find Speed Pays, in Milliseconds_, Charles Duhigg, New York Times, July 23, 2009 
9. _The High-Frequency Trading Arms Race: Frequent Batch Auctions as a Market Design Response_, Eric Budish, Peter Cramton, and John Shim, University of Chicago Booth School of Business, July 7, 2013

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Matching Engine Implementations

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### C++

- [**liteExchange - a minimal traditional stock exchange using FIX protocol**](./matching_engines/c++/liteExchange/README.md)
- [**matching-cpp - Order-matching engine — provides access to liquidity book and coordinates live orders**](./matching_engines/c++/matching-cpp/README.md)
- [**RapidTrader - RapidTrader is a low-latency, high-throughput**](./matching_engines/c++/RapidTrader/README.md)

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### Java

- [**CoralME - CoralME**](./matching_engines/java/CoralME/README.md)
- [**exchange-core - Exchange-core is an open source market exchange core**](./matching_engines/java/exchange-core/README.md)
- [**jLOB - L3 Order Book and Matching Engine**](./matching_engines/java/jLOB/README.md)

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### Python

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>
