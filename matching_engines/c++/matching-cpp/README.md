# Table of Contents 

[Installation](#Setup)

[Matching Engine](#Matching)

[High-level API](#API)

[Software Design](#Design)

[Transport Layer](#Transport)

[Storage Layer](#Storage)

[Benchmark](#Benchmark)

<a name="Setup"/>

# Project setup

## Docker

Run the image in a container.

Create a fresh container and run in interactive mode:

```bash
docker run --name matching_engine -it nrdwnd/exchange:latest
```

Development mode:

```bash
docker run --name matching_engine_dev -it -p9000:8080 --privileged -v ${PWD}:/opt/matching --cap-add=SYS_PTRACE --security-opt seccomp=unconfined nrdwnd/exchange:latest
```

Notes:

- `unconfined` option disables some kernel security features which allows syscall debugging and program profiling from host machine
- Exposes current working directory on host machine, that allows code editing from host

## Rebuild the project

When you are attached to the container run to recompile sources:

```bash
make build 
```

## Start the engine

```bash
./build/bin/matching_service
```

## Debugging

```bash
gdb --tui ./build/bin/matching_service
```

<a name="Matching"/>

# Matching Engine

Order-matching engine — provides access to liquidity book and coordinates live orders

Matching engine provides guarantees that the orders are sorted by price and time.
For buy (bid) side, orders are sorted in descending order of price. Order with highest bid price is first and lowest bid price is last. If price is same then ordering is by time. The order that arrived earlier gets preference.
For sell (ask) side, orders are sorted in ascending order of price. Order with lowest ask price is first and highest ask price is last. If price is same then ordering is by time. The order that arrived earlier gets preference.
We have done exhaustive testing of our matching engine to ensure that fairness is guaranteed.

<a name="API"/>

# API
High-level interface: 
- `GetOrderBookState(): OrderBook` – Order book snapshot `[ [side, price, depth], ...  ]`.
- `GetBestBid(): Number` – The most expensive buying price available at which an asset might be sold out on the market.
- `GetBestAsk(): Number` – The cheapest selling price available at which an asset might be purchased on the market.
- `GetSpread(): Number` – Returns percent market spread `(b.ask - b.bid) / b.ask * 100`.
- `GetQuote(): Number` – Returns the most recent price on which a buyer and seller agreed and at which some amount of the asset was transacted.
- `OrderLookup(OrderID, Side): Order` – Returns active order details if the order still sits in the book.
- `Buy(Order): Status` – Submits buy order.
- `Sell(Order): Status` – Submits sell order.
- `Cancel(OrderID, Side): Status` – Submits order cancel request.

<a name="Design"/>

# Software Design

The engine is designed to be single-threaded. There's no need to have more than one thread as the underlying system is sequential which implies that latency and throughput exhibit a linear relationship.

Asynchronious IO:

- Handling sockets (accept, read, write, poll).

- Writing/reading data into/from the block device (e.g., state reconstruction, logging).

The interrupts should be handled via circular buffers. There are fixed sized Write Buffers (WB) and Read Buffers (RB).

## Data-Oriented Design

The main motivation behind this design choice is a reduction of bottleneck (pipeline bubble, context switches, cache misses, branch misspredictions, false-sharing etc.) in Von Neumann architecture by accentuating on data layouts, access patterns and transformations.

<a name="Transport"/>

# Transport Layer

## Event Loop Backend

There are two backends which are switched based on traffic: polling and hardware interrupts.

### EPOLL

Currently for monitoring file descriptor events the `epoll` is used. It scales quite well when we are interested in watching multiple file descriptors. But it introduces few trade-offs:

- an unnecessary kernel-userspace copy overhead in reading/writing kernel buffer on event notifications.
- Another drawback of the epoll is the choice of underlying data structure (RB-tree) which is, despite of having a satisfactory `O(log n)` average time complexity, it introduces a bunch of cache misses due to its intrinsic sparse data allignment in memory.
- Pre-emptive interrupts on file descriptor available events which cause context-switches.

### io_uring

Coming soon...

Provides the kernel-userspace communication interface via shared Circular Buffers.

## TCP

### Redis

By using `RPOPLPUSH` we can guarantee that the order will be processed exactly once:

1. The service subscribe (`PSUBSCRIBE`) on `CONSUMER` queue keyspace notifications
```
PSUBSCRIBE "__keyspace@0__:CONSUMER"
```
2. Waits until `LPUSH` keyevent is fired
```
1) "pmessage"
2) "__keyspace@0__:CONSUMER"
3) "__keyspace@0__:CONSUMER"
4) "lpush"  
```
3. Once it's triggered, the service pops the new order from the `CONSUMER` queue and atomically pushes to the `CONSUMER_PROCESSING` queue (`RPOPLPUSH`)
```
RPOPLPUSH "CONSUMER" "CONSUMER_PROCESSING"
```
4. Later, the state of the popped item is saved in the datastore with status `processing`
5. Upon successful persistence of the order the consumer removes (`LREM`) the element from the `CONSUMER_PROCESSING` queue
```
LREM "CONSUMER_PROCESSING" 0 "order_id: 200, market: EUR_USD"
```

Edge cases:

- if the matching engine crashed and failed to save the status after popping the order from the producer queue, on next recovery it loads all orders with `processing` status from the datastore and removes already saved orders from the consumer queue, after cleanup of consumer queue it proceeds operation as normal

### HTTP

Buy/sell POST request:
```
[::]/[BUY|SELL]/[EUR_USD|GBP_USD|USD_JPY|...]/PRICE/QUANTITY
```
Response:
- `200` - success
- `400` - failure

<a name="Storage"/>

### Native TCP

Coming soon

## UDP

Coming soon

# Storage Layer

## RocksDB
  
Coming soon
  
## Postgres

- Persist in the datastore the processing status for every incoming order as a special flag to destinguish orders that have been already handled by the matching engine which later will be used for recovery procedure.
- Pull open orders with processing status from the datastore which will be used only once for recovery
Atomically insert matching transaction between source order and distanation order (later can be done as bulk insert for multiple matched distanation orders).

<a name="Benchmark"/>

# Benchmarks

Below is an example of [Geometric Brownian Motion](https://en.wikipedia.org/wiki/Geometric_Brownian_motion) simulation with 1m samples and `σ = 0.01`

```
============ Order Book ============

-----------------------------------
|        |             |          |
|  Price |    Volume   |    Size  |
|        |             |          |
---------------- Buy --------------
|0.0397  |     44113.18|       519|
|0.0395  |     66039.30|       733|
|0.0394  |     40262.44|       480|
|--------------- Sell ------------|
|0.0398  |       427.56|         7|
|0.0399  |     80162.80|       825|
|0.0400  |      3550.73|        34|
|0.0401  |      4371.03|        40|
-----------------------------------


========== Trading summary =========

-----------------------------------
|Buy volume      |       150414.92|
|Sell volume     |        88512.11|
|Buy total       |         5946.19|
|Sell total      |         3532.82|
|Spread          |           0.25%|
|Turnover        |     90975371.60|
|Commission      |       181950.74|
-----------------------------------

```

- Stress test with 1m synthetic samples simulated every 5s; after 3h of continious order execution generated by simulation process, 2m active orders; the last memory snapshot of the matching process taken claims to hold only 300MB.
