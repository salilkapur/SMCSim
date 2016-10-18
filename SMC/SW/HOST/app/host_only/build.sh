#!/bin/bash

print_msg "Building App ..."
 ${HOST_CROSS_COMPILE}g++ -static $OFFLOADED_KERNEL_NAME.cc -L./ -lstdc++ -o main -Wall $HOST_OPT_LEVEL


