#!/bin/sh

# User Program Testing; Multiple Operations on backup files not allowed

make clean
make

retval=$?

if [ $retval -ne 0 ]; then
  exit $retval
fi


filename="/mnt/bkpfs/test_file.txt"
version_to_view="oldest"
# both -l -v present
./bkpctl -l -v "$version_to_view" "$filename"
retval=$?

if [ $retval -ne 0 ]; then
    echo "Test Case Passed"
else 
    echo "Test Case Failed"
    exit -1
fi

make clean
exit 0
