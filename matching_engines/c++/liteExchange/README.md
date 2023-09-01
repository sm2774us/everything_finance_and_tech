# liteExchange
<td><img src="https://img.shields.io/badge/LICENCE-PUBLIC%20DOMAIN-green.svg" alt="Licence badge"></td>

| Linux GCC | Windows MSVC |
|:------:|:------:|
|[![Build Status](https://travis-ci.org/akhin/liteExchange.svg?branch=master)](https://travis-ci.org/akhin/liteExchange)|[![Build status](https://ci.appveyor.com/api/projects/status/hw8s5o46kcgr388l?svg=true)](https://ci.appveyor.com/project/akhin/cpp-multithreaded-order-matching-engine)|

* Sections
	* [1. Introduction and features](#Introduction)
    * [2. Overview of architecture and multithreading system](#Overview)
	* [3. Low latency features , benchmarks and how it can be improved](#LowLatency)
    * [4. Build and runtime dependencies](#Dependencies)
	* [5. How to build](#HowToBuild)
	* [6. Configuring and running the server](#Configuration)
	* [7. Offline order entry mode](#OfflineOrderEntry)
	* [8. Test harness / FIX client automation](#TestHarness)
	* [9. Unit tests](#UnitTesting)
	* [10. Utility scripts](#UtilityScripts)
	* [11. Coding conventions](#CodingConventions)
	* [12. Code map](#CodeMap)
          
## <a name="Introduction"></a>**1. Introduction and features:** 

This project is my "core tech" programming lab / playground.

It is essentially a minimal traditional stock exchange. It uses FIX protocol for order entry and also market data ( highly likely mold udp 64/ ITCH ) and dropcopies will be included in future.

A nice video which explains components of an exchange : https://www.youtube.com/watch?v=b1e4t2k2KJY

For FIX protocol , see https://en.wikipedia.org/wiki/Financial_Information_eXchange

It is cross platform (Linux/Windows) , written in C++11. Its test harness is written in stock Python.

* Named as "lite" because :

	* Does not use any 3rd party library. Only GoogleTest is used for unit testing.
	* No runtime dependency except C++ runtime and socket access. 
	* All configurations are mananaged in one ini file.
	* It is meant to run on a single server , therefore designed to be only vertically-scalable meaning that the more CPU cores you add , a better performance you might get.
	* It supports minimum electronic trading features and also does not do many FIX validations. All existing features can be seen in the tables below.
	

| Electronic trading features   | Details                                                               |
| ----------------------------- |:---------------------------------------------------------------------:|
| FIX order entry				| Using version agnostic custom FIX library			    				|
| Order entry from FIX files	| Providing that mode for debug/dev purposes							|
| Order types                   | Limit                                                 				|
| Order message types           | NewOrder, Cancel                                      				|
| Exec report types			    | Accepted, Filled, PartiallyFilled, Rejected, Canceled 				|
| TIF                           | Not supported       			                        				|
| Matching algorithm			| Price-time priority , assumes 2 orders never arrive at the same time	|
| Securities                    | Supported symbols defined in configuration file	    				|

| Session properties        	| Details                                                  |
| ----------------------------- |:--------------------------------------------------------:|
| Cancel on disconnect			| Always off 				   							   |
| Throttling					| None applied											   |

| Fix order entry session       | Details                                                  |
| ----------------------------- |:--------------------------------------------------------:|
| Versions						| Agnostic , accepts all non-binary ones				   |
| Supported FIX Admin messages	| Heartbeats, test requests, trader logons (no validation) |
| Header tags                   | Only mandatory ones                                      |
| Validations		           	| Compid, sequence number (optional), required tags        |
| Sequence management			| Saves and restores sequence numbers from files		   |
| Symbology						| Uses tag 55, no validations, examples use RIC codes	   |


| Implementation details| Details                                                       |
| ----------------------|:-------------------------------------------------------------:|
| OS                    | Linux ( tested on Ubuntu and CentOS ),Windows ( tested on 10 )|
| Architectures			| 64 bit and 32 bit												|
| C++                   | C++11                                                         |
| C++ Compiler Support  | GCC4.8+ and MSVC 141 (VS2017)									|
| C++ Libraries         | STD, STL						                                |
| C++ Platform APIs     | GNU LibC, POSIX, some POSIX NP ,WinAPI, MS CRT                |
| Build systems         | Makefile, VSCode and VisualStudio2017 for Linux and Windows	|
                        

## <a name="Overview">**2. Overview of architecture and multithreading system:** 

Its architecture overview is as below :

<p align="center">  
<img src="https://github.com/akhin/cpp_multithreaded_order_matching_engine/blob/master/images/architecture.png">       
</p>

The core of order matching layer is called the central order book, which keeps order books per security symbol.

1. Main thread is a single-thread epoll FIX server which passes orders to the central order book`s SPSC lockfree queues per symbol
2. Central order book uses a thread pool in which there is a thread per symbol and ideally pinned to a CPU core. They process messages and push results to SPSC queues of outgoing message processor per symbol.
3. Outgoing message procesor sends execution reports to the FIX clients.

## <a name="LowLatency">**3. Low latency features, benchmarks and how it can be improved:** 

Some features are tunable through the configuration file ( liteExchange.ini ) and some are not.

Non-tunable low latency features are as below : 

| Feature             | Details                                                              |
| --------------------|:--------------------------------------------------------------------:|
| Network/IO model    | Using Epoll to avoid context switching costs					     |
| Memory allocations  | Critical ones aligned to CPU cache line size to avoid false sharing  |
| Contention		  | Design based on bounded SPSC lockfree containers except the logger   |
| Lockfree 			  | SPSC lockfree container uses relaxed memory , uses spinlocks	     |

The tunable low latency features :

| Feature                | Configurable parameters                                                        |
| -----------------------|:------------------------------------------------------------------------------:|
| TCP sockets            | Socket buffer sizes, Nagle algorithm , TCP quick ack, binding to specified NIC |
| TCP Epoll settings     | Max number of events , epoll timeout								              |
| FIX receive cache size | Size of TCP receive buffer cache for FIX messages                              |
| Threads & thread pool  | Pinning to CPU core, stack size, OS level priority, hyperV avoidance           |
| Spinlocks              | You can set the spincount or enable yielding in code   			              |

* Planning to have benchmarks after completing some key performance killers / jitter sources below :

	* Object pools : Currently no pooling for FIX message or order object instances
	
	* Memory allocations : Project can benefit from preallocating everything in critical path.
	
	* Data oriented design : Currently order class in order books suffers from cache misses. If the order class is split into core order ( price-side-symbol ) and other order data,	the matching engine would gain a lot of speed by avoiding cache misses
	
	* Cache-aware algorithms : Order book implementation uses std::multimap which is known to use red-black tree which is controversial when it comes to be cache friendly : https://news.ycombinator.com/item?id=7513896 

## <a name="Dependencies">**4. Build and runtime dependencies:** 

For Linux , the project is built and tested with GCC4.8 and GCC7 on CentOS7 and Ubuntu. 

For running on Linux , make sure you have GNU Libstd C++ 6 runtime in your Linux distribution
	
CentOS :    

	Find out package name for your architecture : yum whatprovides libstdc++.so.6
	Then yum install the package you found
		
Ubuntu :    

	sudo apt-get install libstdc++6

As for Windows you can build with MSVC141(VS2017).

For running on Windows , you need to install MSVC141 ( VS2017 ) C++ runtime :

	Go to https://support.microsoft.com/en-gb/help/2977003/the-latest-supported-visual-c-downloads
	Download and install VS2017 x86 or x64 version

## <a name="HowToBuild">**5. How to build:**
            
**How to build the project on Linux :**
    
    cd build/linux_gcc
    make clean
    make debug  OR make release
    or if you want to make a parallel build :
    ./build_in_parallel.sh release

**How to debug the project on Linux using VisualStudio code :** Build the project for debug mode and open the source directory and VisualStudioCode. Source directory provides a launch.json supporting GCC on Linux and MSVC on Windows. Therefore choose GDB in VSCode debug options before starting debugging and then press F5.

**How to build the project on Linux from Windows with Visual Studio 2017 :** You will need to install Linux C++ feature during VS2017 installation. Then after making sure that your Linux has SSH, you will need your Linux machine SSH details to your Visual Studio. You will also need to copy lieExchange directory to your home directory ( ~/ ). After that you will be able to build and debug on remote Linux. For details please see https://nativecoding.wordpress.com/2018/02/24/visual-studio-for-existing-remote-linux-c-projects/

**How to build the project on Windows with Visual Studio 2017 :** Go to "build/windows_msvc_visual_studio" directory and use SLN file to launch VS with the project
    
**How to build the project on Windows with Visual Studio in command line :** Go to "build/windows_msvc_command_line" directory and execute one of batch files : build_debug.bat or build_release.bat

**Warning levels :** Using -Wall for GCC and /W3 for MSVC
        
**Precompiled header file usage :** On Windows , the project is using /FI ( Force include parameter, therefore no need to include the pch header everywhere ) and specified the pch header to be precompiled_header.h. Note that this breaks edit-and-continue in Visual Studio. Also precompiled header setting of all projects is "use" whereas precompiled_header.cpp is "create".
For Linux , there is pch rule to enable it in the makefile ( build/linux/Makefile) , but currently that rule is not being used since it doesn`t appear as it is doing much improvement as on Windows.

For GCC see https://gcc.gnu.org/onlinedocs/gcc/Precompiled-Headers.html

For MSVC see https://msdn.microsoft.com/en-us/library/8c5ztk84(v=vs.140).aspx

## <a name="Configuration">**6. Configuring and running the server :** 

The engine executable looks for "liteExchange.ini" file. There is a few categories of configuration parameters.


| General and order entry mode						| Description                                                   |
| --------------------------------------------------|:-------------------------------------------------------------:|
| PROCESS_PRIORITY         							| Sets OS level priority of the main process                    |
| ORDER_ENTRY_MODE									| It is either FIX or Offline									|
| OFFLINE_ORDER_ENTRY_FILE							| If offline order entry mode , engine uses this file as input	|
| OFFLINE_ORDER_ENTRY_OUTPUT_FILE					| If offline order entry mode , engine uses this file as output |


| FIX server TCP settings  							| Description                                                   |
| --------------------------------------------------|:-------------------------------------------------------------:|
| TCP_DISABLE_NAGLE									| Disables Nagle algorithm					                    |
| TCP_QUICK_ACK	        							| Enables quick ack , applies to Linux only						|
| TCP_PENDING_CONNECTION_SIZE         				| Used by TCP connection acceptor 								|
| TCP_SOCKET_OPTION_SEND_BUFFER_SIZE     			| Send buffer size for all sockets				       	        |
| TCP_SOCKET_OPTION_RECV_BUFFER_SIZE         		| Receive buffer size for all sockets		                    |
| TCP_POLL_TIMEOUT_MICROSECONDS         			| Epoll timeout	( select in Windows)	                        |
| TCP_POLL_MAX_EVENTS         						| Epoll max number of events , applies only to Linux            |


| FIX server settings      							| Description                                                   |
| --------------------------------------------------|:-------------------------------------------------------------:|
| FIX_SERVER_COMP_ID								| Sender compid for FIX server                     				|
| FIX_SERVER_ADDRESS	        					| Address one of network interface cards on the system			|
| FIX_SERVER_PORT         							| Port to use													|
| FIX_SERVER_SEQUENCE_NUMBER_VALIDATION			    | Can be turned off for rapid development						|
| FIX_SERVER_TIME_PRECISION     					| Applies to tag52 and tag60 : seconds,milliseconds,microseconds|
| FIX_RECEIVE_CACHE_SIZE         					| Size of TCP receive buffer cache			                    |
| REACTOR_THREAD_CPU_ID         					| If non -1, FIX server thread will be pinned to CPU            |
| REACTOR_THREAD_PRIORITY         					| Os-level thread priority of FIX server thread                 |
| REACTOR_THREAD_STACK_SIZE        					| Thread stack size of FIX server thread	                    |

| Central order book settings    					| Description                                                   |
| --------------------------------------------------|:-------------------------------------------------------------:|
| CENTRAL_ORDER_BOOK_MULTITHREADED_ORDER_MATCHING	| Toggles multithreading for order matching                     |
| CENTRAL_ORDER_BOOK_PIN_THREADS_TO_CORES	        | Whether to pin threads of the threadpool to CPU cores			|
| CENTRAL_ORDER_BOOK_HYPER_THREADING         		| If hyperthreading is off it will use cores with an even index |
| CENTRAL_ORDER_BOOK_WORK_QUEUE_SIZE_PER_THREAD     | Queue size per worker thread in the thread pool               |
| CENTRAL_ORDER_BOOK_THREAD_PRIORITY         		| OS-level priority of thread pool threads                      |
| CENTRAL_ORDER_BOOK_THREAD_STACK_SIZE         		| Stack size for thread pool threads                            |


| Outgoing message processor settings    			| Description                                                   |
| --------------------------------------------------|:-------------------------------------------------------------:|
| OUTGOING_MESSAGE_QUEUE_SIZE_PER_THREAD     		| Queue size per worker thread for outgoing messages            |
| OUTGOING_MESSAGE_PROCESSOR_THREAD_PRIORITY     	| OS-level priority of outgoing message processor thread        |
| OUTGOING_MESSAGE_PROCESSOR_CPU_ID     			| If non -1 , outgoing message processor thread will be pinned  |
| OUTGOING_MESSAGE_PROCESSOR_THREAD_STACK_SIZE     	| Thread stack size of outgoing message processor thread        |


| Logger settings          							| Description                                                   |
| --------------------------------------------------|:-------------------------------------------------------------:|
| LOGGER_WRITE_PERIOD_MILLISECONDS 					| Logging period in milliseconds								|
| LOGGER_MEMORY_MAPPED_FILE							| Sets log file													|
| LOGGER_ROTATION_SIZE_IN_BYTES      				| Log rotation size in bytes						 			|
| LOGGER_LOG_LEVEL									| Log level, supported values : FATAL ERROR WARNING INFO DEBUG	|
| LOGGER_COPY_TO_STDOUT   							| If enabled all logs will be printed in console	            |


You will also need to specify security symbols. The order matching engine`s thread pool will create a worker thread for each symbol.
For specifying symbols in ini file, you need to use brackets as  below :

        SYMBOL[]=MSFT
        SYMBOL[]=AAPL
        SYMBOL[]=INTC
        SYMBOL[]=GOOGL
        
Once you start the executable , initially you will see a screen like this :

        06-02-2016 16:22:00 : INFO , Main thread , starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(0) MSFT starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(1) AAPL starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(2) INTC starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(3) GOOGL starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(4) QCOM starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(5) QQQ starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(6) BBRY starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(7) SIRI starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(8) ZNGA starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(9) ARCP starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(10) XIV starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(11) FOXA starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(12) TVIX starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(13) YHOO starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(14) HBAN starting
        06-02-2016 16:22:00 : INFO , Thread pool , Thread(15) BARC starting
        06-02-2016 16:22:00 : INFO , Incoming message dispatcher , Thread starting
        06-02-2016 16:22:00 : INFO , Outgoing message processor , Thread starting
        06-02-2016 16:22:00 : INFO , FIX Engine , Acceptor started

        Available commands :

                display : Shows all order books in the central order book
                quit : Shutdowns the server

## <a name="OfflineOrderEntry">**7. Offline order entry mode:** 

The default mode is FIX server mode. However, if you specify an order file in liteExchange.ini as below :

					ORDER_ENTRY_MODE=OFFLINE
					OFFLINE_ORDER_ENTRY_FILE=sample_offline_order_file.txt
					OFFLINE_ORDER_ENTRY_OUTPUT_FILE=offline_order_entry_output.txt
					
Then the order matcher will process all the orders in that file bypassing FIX protocol and produce offline_order_entry_output.txt as result file.
For an example offline order file see :

https://github.com/akhin/multithreaded_order_matching_engine/blob/master/bin/sample_offline_order_file.txt


The output of offline order matching will have timestamps with microsecond precision and FIX execution report messages as below :


		16-09-2017 03:12:27.739729
		8=FIX.4.29=6835=86=011=14=017=120=037=38=239=054=255=GOOGL150=0151=210=001
		16-09-2017 03:12:27.755355
		8=FIX.4.29=6835=86=011=14=017=220=037=38=139=054=155=GOOGL150=0151=110=255
		16-09-2017 03:12:27.773486
		8=FIX.4.29=7835=86=111=14=117=320=031=132=137=38=139=254=155=GOOGL150=2151=010=173
		16-09-2017 03:12:27.777489
			
## <a name="TestHarness">**8. Test harness / Fix client automation:** 

Under "test_harness" directory :

The project has a Python script which can send orders from FIX files using Fix protocol.

1. Modify test_cases.txt which has the orders to send to the engine as FIX messages.

2. To run it : python ./fix_client_automation.py

3. You can set automation parameters in the commandline :
        
<p align="center">  
<img src="https://github.com/akhin/cpp_multithreaded_order_matching_engine/blob/master/images/fix_client_automation.png">       
</p>
            
## <a name="UnitTesting">**9. Unit testing :** 

The project uses GoogleTest 1.7. You can find a makefile and vcproj under "unit_test" directory.
    
Building and running unit test on Linux : You can use Makefile under "unit_test" directory.

## <a name="UtilityScripts">**10. Utility scripts:**

You can find them under "utility_scripts" directory :

| Script		           			| Functionality		                                                 			|
| ----------------------------------|:-----------------------------------------------------------------------------:|
| source_code_formatter.sh 			| converts EOL to Unix, converts tabs to 4 spaces, removes trailing white space	|
| leak_detector_valgrind.sh			| memory leak detection with Valgrind                             				|
| build_with_thread_sanitizer.sh	| build with GCC thread sanitizer												|
| build_with_address_sanitizer.sh	| build with GCC address sanitizer    											|
| build_for_gprof.sh       			| build for gprof profiling                    									|
| profile_with_callgrind.sh			| profile with Valgrind callgrind          										|
| profiler_windows_visual_studio	| profile with Visual Studio`s vsperf											|
| leak_detector_drmemory.bat		| memory leak detection with Dr.Memory 											|
| tcpdump_capture.sh				| gets a tcpdump capture that you can view with Wireshark               		|
| valgrind_hellgrind.sh				| runs Valgrind concurrency analysis						             		|
| valgrind_cachegrind.sh			| runs Valgrind CPU cache usage analysis					             		|
    

## <a name="CodingConventions">**11. Coding conventions:**

Source code and file/directory naming conventions :

| Category		           		| Naming convention	                               	|
| ------------------------------|:-------------------------------------------------:|
| Directory names 				| lower_case_word									|
| File names					| lower_case_word                             		|
| Include guards				| _HEADER_H_										|
| Macros						| UPPER_CASE_WORD    								|
| Enums       					| UPPER_CASE_WORD                    				|
| Namespace names				| lower_case_word                     				|
| Class names					| CamelCase											|
| Method names					| pascalCase 										|
| Variable names				| pascalCase               							|
| Member variables starts with	| m_               									|
| Directories/Namespaces		| As in Boost, there is a namespace per directory	|


Source code indentations and new line usage :
    
	- 4 spaces, no tabs ( This needs to be setup in VS )
    - By default Netbeans editor uses spaces for tabs
    - Needs to be set in VS2013 : https://msdn.microsoft.com/en-gb/library/ms165330(v=vs.90).aspx
    - New lines : Unix CR only ( \n ) , VisualStudio can handle it even though Windows is \r\n
    
utility/source_code_formatter.sh : It is a Bash script that scans all cpp,h,hpp files in project directory and converts Windows end of lines to Linux, converts tabs to 4 spaces and removes trailing whitespace. It requires dos2unix.
    
Inclusions : Using forward slash as it works for both Linux and Windows :

~~~C++
		#include <core/concurrency/thread.h>
~~~

## <a name="CodeMap">**12. Code map:**

<p align="center">  
<img src="https://github.com/akhin/cpp_multithreaded_order_matching_engine/blob/master/images/code_map.png">       
</p>

**core :** It is the core library. Has subdirectories such as as network, concurrency etc

**fix :** It mainly uses core/network and core/concurrency. It is the FIX implemenatation

**order_matcher :** Order matcher layer is the heart of project. It knows nothing about FIX protocol but has its own order,  incoming message and outgoing message classes.

**server :** This layer is the layer that uses FIX layer to receive FIX orders and submit to the central order book in the order matcher layer. Then it receives results from the central order book and sends them to the outgoing message processor.