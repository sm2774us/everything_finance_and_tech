![workflow](https://github.com/jmsadair/RapidTrader/actions/workflows/linux.yml/badge.svg)
![workflow](https://github.com/jmsadair/RapidTrader/actions/workflows/macos.yml/badge.svg)
[![codecov](https://codecov.io/gh/jmsadair/RapidTrader/branch/dev/graph/badge.svg?token=4QQI0QDVYM)](https://codecov.io/gh/jmsadair/RapidTrader)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/2d9fa4354aac44e9ba97ad18b35e7eb9)](https://www.codacy.com/gh/jmsadair/RapidTrader/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=jmsadair/RapidTrader&amp;utm_campaign=Badge_Grade)

# RapidTrader

RapidTrader is a low-latency, high-throughput [order matching system](https://en.wikipedia.org/wiki/Order_matching_system) that is optimised for high frequency trading. This system is capable of processing upwards of 2-4 million order insertions per second and offers support for the following order types:
- Limit
- Market
- Stop
- Trailing Stop

Be aware that this project is still in development - bugs are expected and pull requests are welcome.

## Requirements
- Linux or MacOS
- gcc or clang
- [python3](https://www.python.org/downloads/)
- [CMake](https://cmake.org/download/)
- [boost](https://github.com/boostorg/boost)

Note that this project may be able to run on other platforms or compilers, but only the ones listed above have been tested at this point. 

## Building
To build and test the project, run the following commands: 
```
  mkdir build && cd build

  cmake ..

  make -j

  make test
```

## Performance
The following are benchmarks of the synchronous and concurrent implementations of RapidTrader. This benchmark measured the performance of the add order operation with a varying number of symbols and orders with a maximum price depth of 15. All benchmarks were ran on an Intel Core i7-8700 processor, which supports up to 12 threads.

```
--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
BM_Market/symbols:1/orders:1000000           241 ms          241 ms            3
BM_Market/symbols:1/orders:2000000           514 ms          513 ms            2
BM_Market/symbols:1/orders:3000000           735 ms          735 ms            1
BM_Market/symbols:1/orders:4000000          1087 ms         1087 ms            1
BM_Market/symbols:100/orders:1000000         242 ms          242 ms            3
BM_Market/symbols:100/orders:2000000         539 ms          539 ms            1
BM_Market/symbols:100/orders:3000000         787 ms          787 ms            1
BM_Market/symbols:100/orders:4000000        1032 ms         1032 ms            1
BM_Market/symbols:1000/orders:1000000        268 ms          268 ms            2
BM_Market/symbols:1000/orders:2000000        985 ms          985 ms            1
BM_Market/symbols:1000/orders:3000000       1292 ms         1292 ms            1
BM_Market/symbols:1000/orders:4000000       1582 ms         1582 ms            1
BM_Market/symbols:2000/orders:1000000        356 ms          356 ms            2
BM_Market/symbols:2000/orders:2000000        654 ms          654 ms            1
BM_Market/symbols:2000/orders:3000000       1856 ms         1855 ms            1
BM_Market/symbols:2000/orders:4000000       2200 ms         2200 ms            1

------------------------------------------------------------------------------------------
Benchmark                                                Time             CPU   Iterations
------------------------------------------------------------------------------------------
BM_ConcurrentMarket/symbols:1/orders:1000000           517 ms          319 ms            2
BM_ConcurrentMarket/symbols:1/orders:2000000           999 ms          581 ms            1
BM_ConcurrentMarket/symbols:1/orders:3000000          1548 ms          976 ms            1
BM_ConcurrentMarket/symbols:1/orders:4000000          2119 ms         1225 ms            1
BM_ConcurrentMarket/symbols:100/orders:1000000         422 ms          419 ms            2
BM_ConcurrentMarket/symbols:100/orders:2000000         853 ms          841 ms            1
BM_ConcurrentMarket/symbols:100/orders:3000000        1279 ms         1267 ms            1
BM_ConcurrentMarket/symbols:100/orders:4000000        1721 ms         1706 ms            1
BM_ConcurrentMarket/symbols:1000/orders:1000000        414 ms          411 ms            2
BM_ConcurrentMarket/symbols:1000/orders:2000000        854 ms          754 ms            1
BM_ConcurrentMarket/symbols:1000/orders:3000000       1178 ms         1166 ms            1
BM_ConcurrentMarket/symbols:1000/orders:4000000       1591 ms         1576 ms            1
BM_ConcurrentMarket/symbols:2000/orders:1000000        417 ms          412 ms            2
BM_ConcurrentMarket/symbols:2000/orders:2000000        832 ms          824 ms            1
BM_ConcurrentMarket/symbols:2000/orders:3000000       1471 ms         1204 ms            1
BM_ConcurrentMarket/symbols:2000/orders:4000000       1746 ms         1501 ms            1
```
As demonstrated by the above data, the synchronous version of RapidTrader tends to offer better performance than the concurrent version until the number of symbols reaches 2000. Profiling has revealed that this is primarily due the synchronisation of memory allocations. A memory pool will likely need to be implemented for the concurrent version in order to eliminate this synchronization.

## Resources

The following are some resources that I have found helpful in the development of this project:

- [How to Build a Fast Limit Order Book](https://web.archive.org/web/20110219163448/http://howtohft.wordpress.com/2011/02/15/how-to-build-a-fast-limit-order-book/)
- [Market Order Matching Engine](https://gist.github.com/jdrew1303/e06361070468f6614d52216fb91b79e5)
- [The Basics of Trading a Stock: Know Your Orders](https://www.investopedia.com/investing/basics-trading-stock-know-your-orders/)
