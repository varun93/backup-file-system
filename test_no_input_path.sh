#!/bin/sh

# User Program Testing; Input path is necessary 

make clean
make

retval=$?

if [ $retval -ne 0 ]; then
  exit $retval
fi

./bkpctl -l   
retval=$?

if [ $retval -ne 0 ]; then
    echo "Test Case Passed"
else 
    echo "Test Case Failed"
    exit -1
fi

make clean
exit 0