# Ultra Low Latency Java

## Table Of Contents <a name="top"></a>

1. [Low Latency Systems](#low-latency-systems)
2. [Characteristics of automated trading systems](#characteristics-of-automated-trading-systems)
3. [LMAX Disruptor](./lmax_disruptor.md)
  - 3.1   [Sequence Barrier](#sequence-barrier)
  - 3.2   [Concurrent Maps](#concurrent-maps)
4. [Eclipse Collections](#eclipse-collections)
  - 4.1   [Primitive Maps](#primitive-maps)
  - 4.2   [Concurrent Maps](#concurrent-maps)
5. [Real Logic Agrona](#real-logic-agrona)
  - 5.1   [Single Threaded Substitutes](#single-threaded-substitutes)
  - 5.2   [Minimal Allocation Collection Objects](#minimal-allocation-collection-objects)
  - 5.3   [Functional Interfaces](#functional-interfaces)
6. [Adaptive Radix Trees](#adaptive-radix-trees)
7. [OpenHFT Chronicle Wire](#openhft-chronicle-wire)
8. [LZ4 Java](#lz4-java)

### Low Latency Systems

- Low latency trading systems play a crucial role in today's fast-paced financial markets, where nanoseconds can make a significant difference in executing trades and gaining a competitive edge. These systems rely on advanced technology, efficient algorithms, and high-performance programming languages like Java to achieve the lowest possible latency.
- Java is a popular choice for developing low latency trading systems due to its ability to deliver high-performance code, direct hardware access, and fine-grained control over system resources. It offers a balance between high-level abstractions and low-level control, allowing developers to optimize critical sections of code for speed and efficiency.
- When designing a low latency trading system in Java, it essentially boils down to two skills:
  - *1) Low Latency Java* : The first is low latency java which just means that you write your code to run as fast as possible but that alone isn't sufficient. It must also be as consistently fast as possible with as few and as small outliers as possible. High percentiles such as the 99th percentile of latency must be acceptable.

    By fast I mean single to double digit micros for any given system producing meaningful units of work. This particular skill goes beyond Java code and into effective utilisation of the underlying hardware such as running on the correct NUMA node, utilising specific native system calls and pinning each thread doing work to its own single core.

  - *2) Zero Allocation* : By fast I mean single to double digit micros for any given system producing meaningful units of work. This particular skill goes beyond Java code and into effective utilisation of the underlying hardware such as running on the correct NUMA node, utilising specific native system calls and pinning each thread doing work to its own single core.
- In terms of what libraries can be used when writing such code options are extremely limited. The vast majority of libraries including the JDK libraries do not meet the combined requirements of low latency and zero allocation. This means that you must reinvent the stack ground up starting from an empty main method and only introducing code that you know meets your requirements. If you are unable to find libraries that do what you need you must write them from scratch. Most good tech stacks will have reinvented their loggers, histograms, fix engines, wire encodings, collections, data structures and more - simply to avoid violating latency or allocation requirements.

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

### LMAX Disruptor
- [Sequence Barrier](https://github.com/LMAX-Exchange/disruptor/blob/master/src/main/java/com/lmax/disruptor/SequenceBarrier.java)
  
  Firstly - a memory barrier is a CPU architecture primitive; a SequenceBarrier is an implementation class in the Disruptor. I will attempt to cover both.

  Memory barrier - allows software to provide instruction to hardware that all instructions before a certain point X must have been completed before X is. This is used because CPUs provide out-of-order execution optimisations. For more details, best to read this paper - See Memory Barriers: a Hardware View for Software Hackers.

  SequenceBarrier is an implementation class in the Disruptor; generally speaking it is a part of the Disruptor and you won't need to use it outside the Disruptor. It actually performs a similar function conceptually to a memory barrier - it prevents consumers getting ahead of a publisher on the ringbuffer. See - [SequenceBarrier on github](https://github.com/LMAX-Exchange/disruptor/blob/master/src/main/java/com/lmax/disruptor/SequenceBarrier.java).

  ```java
  public final class WaitSpinningHelper {
    .
    .
    .
    private final SequenceBarrier sequenceBarrier;
    private final Lock lock;

    public long tryWaitFor(final long seq) throws AlertException, InterruptedException {
        sequenceBarrier.checkAlert();

        long spin = spinLimit;
        long availableSequence;
        while ((availableSequence = sequenceBarrier.getCursor()) < seq && spin > 0) {
            if (spin < yieldLimit && spin > 1) {
                Thread.yield();
            } else if (block) {
                lock.lock();
                try {
                    sequenceBarrier.checkAlert();
                    // lock only if sequence barrier did not progressed since last check
                    if (availableSequence == sequenceBarrier.getCursor()) {
                        processorNotifyCondition.await();
                    }
                } finally {
                    lock.unlock();
                }
            }

            spin--;
        }

        return (availableSequence < seq)
                ? availableSequence
                : sequencer.getHighestPublishedSequence(seq, availableSequence);
    }  
  ```

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Eclipse Collections
- [Eclipse Collections](https://github.com/eclipse/eclipse-collections/) is a collections framework for Java with optimized data structures and a rich, functional and fluent API.
- More information can be found at => [Eclipse Collections Doc Page](http://www.eclipse.org/collections).
- How to install ?
  - Gradle
  ```groovy
  implementation 'org.eclipse.collections:eclipse-collections-api:11.1.0'
  implementation 'org.eclipse.collections:eclipse-collections:11.1.0'
  ```
  - Maven
  ```xml
  <dependency>
    <groupId>org.eclipse.collections</groupId>
    <artifactId>eclipse-collections-api</artifactId>
    <version>11.1.0</version>
  </dependency>

  <dependency>
    <groupId>org.eclipse.collections</groupId>
    <artifactId>eclipse-collections</artifactId>
    <version>11.1.0</version>
  </dependency>
  ```

#### Primitive Maps
- There are three types of primitive maps:
  - Interfaces
    - Primitive To Primitive
      - *org.eclipse.collections.api.map.primitive.MutableIntLongMap*
      - *org.eclipse.collections.api.map.primitive.MutableLongIntMap*
  - Implementations
    - Object To Primitive (`ObjectIntHashMap`, `ObjectFloatHashMap` etc.)
    - Primitive To Object (`IntObjectHashMap`, `FloatObjectHashMap` etc.)
      - *org.eclipse.collections.impl.map.mutable.primitive.IntObjectHashMap*
      - *org.eclipse.collections.impl.map.mutable.primitive.LongObjectHashMap*
    - Primitive To Primitive (`IntIntHashMap`, `IntLongHashMap` etc.)
      - *org.eclipse.collections.impl.map.mutable.primitive.LongLongHashMap*
      - *org.eclipse.collections.impl.map.mutable.primitive.IntIntHashMap*
      - *org.eclipse.collections.impl.map.mutable.primitive.IntLongHashMap*
      - *org.eclipse.collections.impl.map.mutable.primitive.ByteIntHashMap*
    - All the maps use [open addressing](https://www.scaler.com/topics/data-structures/open-addressing/) and [quadratic probing](https://www.scaler.com/topics/quadratic-probing/).

#### Concurrent Maps
- *org.eclipse.collections.impl.map.mutable.ConcurrentHashMap*


<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Real Logic Agrona

[Agrona](https://aeroncookbook.com/agrona/overview/) offers a number of useful general use high performance data structures and common support objects as used in Simple Binary Encoding, Aeron and related projects.

#### Single Threaded Substitutes
- *org.agrona.collections.MutableBoolean* :
  A single threaded substitute for *java.util.concurrent.atomic.AtomicBoolean*.
- *org.agrona.collections.MutableReference* :
  A single threaded substitute for *java.util.concurrent.atomic.AtomicReference*.
- *org.agrona.collections.MutableInteger* :
  A single threaded substitute for *java.util.concurrent.atomic.AtomicInteger*.
- *org.agrona.collections.MutableLong* :
  A single threaded substitute for *java.util.concurrent.atomic.AtomicLong*.

#### Minimal Allocation Collection Objects
- Agrona offers a number of efficient zero and/or minimal allocation collection objects for java primitives that avoid boxing. They typically out perform their JDK counterparts.
  - *org.agrona.collections.Long2ObjectHashMap* :
    Primitive long to object HashMap.

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

#### Functional Interfaces
- *Functional Interface* in Java enables users to implement functional programming in Java.In functional programming, the function is an independent entity.
  - The function can do anything a variable is capable to perform like passing a function as a parameter, a function returned by another function, etc. Functions in Python is an example of functional programming.
  - Functional interfaces were introduced in Java 8. A functional interface can contain only one abstract method and it can contain any number of static and default (non-abstract) methods.
  - Syntax:
  ```java
  // interface
  @FunctionalInterface   // annotation
  interface interfaceName{
      // abstract method 
      abstract returnType methodName( /* parameters */);
    
      // default or static methods
    
      int method1(){
          // ....
      }
      String method2(int x, float y){
          // ....
      }
  }

  // public class
  public class className{
      // main method
      public static void main(String[] args){
          interfaceName temp = (/*parameters*/) -> {
              // perform operations 
          };
        
          temp.methodName(); // call abstract method of the interface
      }
  }
  ```
- *BiConsumer* It takes two arguments one generic, and the other of primitive type. It also doesn't return a value.
  - Example:
  ```java
  import java.util.function.BiConsumer;

  public class Test {
      public static void main(String[] args) {
          BiConsumer<Integer, Integer> eligibility = (Integer age, Integer percentage) -> {
              if (age > 14 && percentage > 75)
                  System.out.println("You're eligible to participate in school elections");
              else
                  System.out.println("The eligibility criteria is Age > 14 and Percentage > 75");
          };

          eligibility.accept(16, 89);
      }  
  }
  ```
  - Output:
  ```bash
  You're eligible to participate in school elections
  ```
  - *Explanation:* To stand in school elections, one must be above the age of 14 and must be at least scoring 75 in the previous grade. To ensure this, we are using Bi-consumer which takes two integer values. It checks for the condition and prints the result. Bi-consumer doesn't return a value just like the Consumer functional interface.
- [Agrona](https://aeroncookbook.com/agrona/overview/) provides some out-of-the-box usefult functional interfaces, like:
  - *org.agrona.collections.LongLongConsumer* :
    This is a functional interface and can therefore be used as the assignment target for a lambda expression or method reference. This is a (long, long) primitive specialisation of a BiConsumer. Accept two values that comes as a tuple of longs. Example:
    ```java
    private CompletableFuture<CommandResultCode> submitPersistCommandAsync(final ApiPersistState apiCommand) {

        final CompletableFuture<CommandResultCode> future1 = new CompletableFuture<>();
        final CompletableFuture<CommandResultCode> future2 = new CompletableFuture<>();

        publishPersistCmd(apiCommand, (seq1, seq2) -> {
            promises.put(seq1, cmd -> future1.complete(cmd.resultCode));
            promises.put(seq2, cmd -> future2.complete(cmd.resultCode));
        });

        return future1.thenCombineAsync(future2, CommandResultCode::mergeToFirstFailed);
    }
    ```
- Reference: To learn more about Functional Interfaces => [Functional Interfaces on Scaler](https://www.scaler.com/topics/functional-interface-in-java/).

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### Adaptive Radix Trees

Main memory capacities have grown up to a point where most databases fit into RAM. For main-memory database systems, index structure performance is a critical bottleneck. Traditional in-memory data structures like balanced binary search trees are not efficient on modern hardware, because they do not optimally utilize on-CPU caches. Hash tables, also often used for main-memory indexes, are fast but only support point queries. To overcome these shortcomings, we present ART, an adaptive radix tree (trie) for efficient indexing in main memory. Its lookup performance surpasses highly tuned, read-only search trees, while supporting very efficient insertions and deletions as well. At the same time, ART is very space efficient and solves the problem of excessive worst-case space consumption, which plagues most radix trees, by adaptively choosing compact and efficient data structures for internal nodes. Even though ART's performance is comparable to hash tables, it maintains the data in sorted order, which enables additional operations like range scan and prefix lookup.

More information can be found in [Harvard Lecture Series](./adaptive_radix_tree.pdf).

[Adaptive Radix Tree Java Implementation on github](https://github.com/LMAX-Exchange/disruptor/blob/master/src/main/java/com/lmax/disruptor/SequenceBarrier.java).

[IEEE - The adaptive radix tree: ARTful indexing for main-memory databases](https://ieeexplore.ieee.org/document/6544812).

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### OpenHFT Chronicle Wire

[Chronicle Wire](https://github.com/OpenHFT/Chronicle-Wire) is a Wire Format abstraction library, The purpose of Chronicle Wire is to address the following concerns in a consistent manner:

  - Application configuration. (Using YAML)
  - Data serialization (YAML, binary YAML, JSON, Raw binary data, CSV)
  - Accessing off-heap memory in a thread-safe manner. (Bind to shared off-heap memory)
  - High performance data exchange using binary formats. Only include as much meta data as you need.

- How to install ?
  - Gradle
  ```groovy
  implementation 'net.openhft:affinity:3.2.2'
  implementation 'net.openhft:chronicle-wire:2.19.1'
  ```
  - Maven
  ```xml
  <dependency>
    <groupId>net.openhft</groupId>
    <artifactId>affinity</artifactId>
    <version>3.2.2</version>    
  </dependency>
  <dependency>
    <groupId>net.openhft</groupId>
    <artifactId>chronicle-wire</artifactId>
    <version>2.19.1</version>    
  </dependency>
  ```

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>

### LZ4 Java

[LZ4 compression for Java](https://github.com/lz4/lz4-java), based on Yann Collet's work available at http://code.google.com/p/lz4/.

This library provides access to two compression methods that both generate a valid LZ4 stream:

  - fast scan (LZ4):
    - low memory footprint (~ 16 KB),
    - very fast (fast scan with skipping heuristics in case the input looks incompressible),
    - reasonable compression ratio (depending on the redundancy of the input).
  - high compression (LZ4 HC):
    - medium memory footprint (~ 256 KB),
    - rather slow (~ 10 times slower than LZ4),
    - good compression ratio (depending on the size and the redundancy of the input).
  - The streams produced by those 2 compression algorithms use the same compression format, are very fast to decompress and can be decompressed by the same decompressor instance.

- How to install ?
  - Gradle
  ```groovy
  implementation 'org.lz4:lz4-java:1.8.0'
  ```
  - Maven
  ```xml
  <dependency>
    <groupId>org.lz4</groupId>
    <artifactId>lz4-java</artifactId>
    <version>1.8.0</version>
  </dependency>
  ```

<div align="right"><a href="#top" target="_blacnk"><img src="https://img.shields.io/badge/Back to up-orange?style=for-the-badge&logo=expo&logoColor=white" /></a></div>
