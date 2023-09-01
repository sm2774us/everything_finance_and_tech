#/bin/bash
DEBUGGEE_DIR=../bin/
DEBUGGEE=./liteExchange
cd $DEBUGGEE_DIR
valgrind --tool=cachegrind $DEBUGGEE