#!/bin/sh

# User Program Testing; Atleast one operation on backup files is mandatory

make clean
make

retval=$?

if [ $retval -ne 0 ]; then
  exit $retval
fi

filename="/mnt/bkpfs/test_file.txt"
./bkpctl  "$filename"
retval=$?

if [ $retval -ne 0 ]; then
    echo "Test Case Passed"
else 
    echo "Test Case Failed"
    exit -1
fi

make clean
exit 0
