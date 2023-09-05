# Ultra Low Latency C++

## Table Of Contents <a name="top"></a>

1. [Low Latency Systems](#low-latency-systems)
2. [Characteristics of automated trading systems](#characteristics-of-automated-trading-systems)
3. [Modern C++](./modern_cpp.md)

### Low Latency Systems

- Low latency trading systems play a crucial role in today's fast-paced financial markets, where nanoseconds can make a significant difference in executing trades and gaining a competitive edge. These systems rely on advanced technology, efficient algorithms, and high-performance programming languages like C++ to achieve the lowest possible latency.
- C++ is a popular choice for developing low latency trading systems due to its ability to deliver high-performance code, direct hardware access, and fine-grained control over system resources. It offers a balance between high-level abstractions and low-level control, allowing developers to optimize critical sections of code for speed and efficiency.
- When designing a low latency trading system in C++, several key considerations come into play:
  - Hardware optimization: C++ provides features such as inline assembly, SIMD (Single Instruction, Multiple Data) instructions, and thread affinity, enabling developers to fine-tune code execution for specific hardware architectures. By leveraging these capabilities, developers can achieve optimal utilization of CPU resources and reduce latency.
  - Memory management: Efficient memory access is critical in low latency systems. C++ offers explicit memory management through features like raw pointers and custom memory allocators. This control over memory allocation and deallocation allows developers to minimize memory overhead, avoid unnecessary copies, and reduce cache misses, resulting in improved performance.
  - Multithreading and concurrency: C++ provides robust support for multithreading and concurrent programming. By utilizing features like threads, mutexes, condition variables, and atomic operations, developers can design trading systems that take advantage of parallelism and exploit the full potential of modern CPUs with multiple cores. Careful synchronization and data sharing strategies are crucial to prevent race conditions and ensure the correctness of the system.
  - Network communication: Low latency trading systems rely on fast and reliable communication with exchanges, brokers, and market data providers. C++ offers a variety of network libraries, such as Boost.Asio and ZeroMQ, which allow developers to implement high-performance networking components. By utilizing techniques like zero-copy data transfer and minimizing protocol overhead, C++ can help reduce network latency.
  - Algorithmic optimization: The algorithms employed in low latency trading systems must be carefully designed and optimized for speed. C++'s expressive syntax, template metaprogramming, and libraries like the Standard Template Library (STL) enable developers to create efficient data structures and algorithms. Techniques such as precomputing results, reducing unnecessary calculations, and using lock-free data structures can further improve performance.

### Characteristics of automated trading systems

- Only a few lines of code in the entire system are important (buy/sell)
- These lines are not exercised often (relative to say graphics rendering)
- Lots of market data events (millions of events per second)
- Jitter is a killer (delays in reaction time can be very costly)
- Very little threading/instruction vectorisation/etc
  - Remember - latency not throughput
- No mistakes (and very good recovery from mistakes)
  - One second is 4 billion CPU instructions
  - Companies have gone bankrupt through system error before, and markets/investors have also been impacted
  - A highly regulated and highly visible industry
