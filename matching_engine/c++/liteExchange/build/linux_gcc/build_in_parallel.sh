#!/bin/bash
BUILD_MODE=$1
NUMBER_OF_PROCESSORS=`nproc`

if [ "$#" -ne 1 ]; then
    echo You have to pass build mode : release, debug or clean
    exit -1
fi

echo Build mode is $BUILD_MODE
echo Number of processors is $NUMBER_OF_PROCESSORS
make -j$NUMBER_OF_PROCESSORS $BUILD_MODE
exit 0