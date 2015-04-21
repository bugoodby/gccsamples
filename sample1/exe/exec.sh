#!/bin/sh

#export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=../so1:$LD_LIBRARY_PATH
#export LD_DEBUG=files
#export LD_DEBUG=symbols
#export LD_DEBUG=all
exec ./main

