#Compiler
CXX=g++
#Source Directories
SOURCE_DIR=../../source
SOURCE_DIR_CORE= $(SOURCE_DIR)/core
SOURCE_DIR_CORE_MEMORY= $(SOURCE_DIR_CORE)/memory
SOURCE_DIR_CORE_CONCURRENT= $(SOURCE_DIR_CORE)/concurrency
SOURCE_DIR_CORE_LOGGER= $(SOURCE_DIR_CORE)/logger
SOURCE_DIR_CORE_NETWORK= $(SOURCE_DIR_CORE)/network
SOURCE_DIR_CORE_DESIGN_PATTERNS= $(SOURCE_DIR_CORE)/design_patterns
SOURCE_DIR_FIX= $(SOURCE_DIR)/fix
SOURCE_DIR_ORDER_MATCHER= $(SOURCE_DIR)/order_matcher
SOURCE_DIR_SERVER= $(SOURCE_DIR)/server
SOURCES =  $(SOURCE_DIR_CORE_CONCURRENT)/thread.cpp \
		$(SOURCE_DIR_CORE_CONCURRENT)/thread_pool.cpp \
		$(SOURCE_DIR_CORE)/configuration.cpp \
		$(SOURCE_DIR_CORE)/stopwatch.cpp \
		$(SOURCE_DIR_CORE)/single_instance.cpp \
		$(SOURCE_DIR_CORE)/shared_memory.cpp \
		$(SOURCE_DIR_CORE_LOGGER)/logger.cpp \
		$(SOURCE_DIR_CORE_NETWORK)/io_listener_epoll.cpp \
		$(SOURCE_DIR_CORE_NETWORK)/socket.cpp \
		$(SOURCE_DIR_CORE_NETWORK)/tcp_connection.cpp \
		$(SOURCE_DIR_CORE_NETWORK)/tcp_server.cpp \
		$(SOURCE_DIR_CORE_NETWORK)/tcp_server_reactor.cpp \
		$(SOURCE_DIR_CORE_NETWORK)/tcp_server_thread_per_client.cpp \
		$(SOURCE_DIR_FIX)/fix_message.cpp \
		$(SOURCE_DIR_FIX)/fix_session.cpp \
		$(SOURCE_DIR_FIX)/fix_server_reactor.cpp \
		$(SOURCE_DIR_FIX)/fix_receive_cache.cpp \
		$(SOURCE_DIR_ORDER_MATCHER)/order.cpp \
		$(SOURCE_DIR_ORDER_MATCHER)/security_manager.cpp \
		$(SOURCE_DIR_ORDER_MATCHER)/order_book.cpp \
		$(SOURCE_DIR_ORDER_MATCHER)/central_order_book.cpp \
		$(SOURCE_DIR_SERVER)/server_fix.cpp \
		$(SOURCE_DIR_SERVER)/server_offline.cpp \
		$(SOURCE_DIR_SERVER)/server_outgoing_message_processor.cpp \
		$(SOURCE_DIR)/main.cpp
#Include Directories
INCLUDE_DIR = $(SOURCE_DIR)/
#Objects
OBJECTS = $(SOURCES:.cpp=.o)
#Executable
EXECUTABLE = ../../bin/liteExchange
#Compiler flags
CFLAGS= -I$(INCLUDE_DIR) -std=c++11 -c 
#Linker flags
LFLAGS= -lstdc++ -pthread -rdynamic

#Add DEBUG macro , symbol generation and show all warnings
debug: CFLAGS += -DDEBUG -g -Wall -fno-omit-frame-pointer
debug: all
#unresolved-symbols=ignore-in-shared-libs is for sanitizers
#as sanitizers cause additional code to be added
#On CentOS  , you have to do : yum install libasan , yum install libtsan
#you can only enable 1 sanitizer so that why there is conf options
#Debug mode + compile and link with GCC address sanitizer 
debug_with_asan: CFLAGS += -DDEBUG -g -Wall -fno-omit-frame-pointer
debug_with_asan: LFLAGS += -fsanitize=address -unresolved-symbols=ignore-in-shared-libs
debug_with_asan: all
#Debug mode + compile and link with GCC thread sanitizer 
debug_with_tsan: CFLAGS += -DDEBUG -g -Wall -fno-omit-frame-pointer
debug_with_tsan: LFLAGS += -fsanitize=thread -unresolved-symbols=ignore-in-shared-libs
debug_with_tsan: all

#Use O2 as it is the most tested optimisation level in most software projects
release: CFLAGS +=  -O2
release: all
# Relase mode with GCOV code coverage
release_with_gcov: CFLAGS +=  -O2 -fprofile-arcs -ftest-coverage
release_with_gcov: LFLAGS +=  -lgcov --coverage
release_with_gcov: all
# Release mode however meant for profiling winth gprof
# So by using gprof we can see most time taking functions	
release_with_gprof: CFLAGS +=  -O2 -pg -g -fno-omit-frame-pointer
release_with_gprof: LFLAGS +=  -pg
release_with_gprof: all

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
		$(CXX) $(LFLAGS) $(OBJECTS) -o $@ 
	
pch : 
	g++ -c ../../source/precompiled_header.h -o ../../source/precompiled_header.h.gch -std=c++11
	#To use precompiled header in linux :
	#1. Add pch rule as first dependency in rule "all"
	#2. precompiled_header.h should be included in all source files
	
.cpp.o: *.h
	$(CXX) $(CFLAGS) $< -o $@

clean:
	@echo Cleaning
	-rm -f $(OBJECTS) $(EXECUTABLE)
	@echo Cleaning done
	
.PHONY: all clean
