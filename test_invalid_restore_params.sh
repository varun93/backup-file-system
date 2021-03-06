#!/bin/sh

# User Program Testing; Passing invalid restore params  

make clean
make

retval=$?

if [ $retval -ne 0 ]; then
  exit $retval
fi

filename="/mnt/bkpfs/test_file.txt"
version_to_restore="all"
./bkpctl -r "$version_to_restore" "$filename"
retval=$?

if [ $retval -ne 0 ]; then
    echo "Test Case Passed"
else 
    echo "Test Case Failed"
    exit -1
fi

make clean
exit 0