#!/bin/sh

# Trying to delete a non-existent version of a backup 

source ./utils.sh

filename=test_file.txt
maxversions=5
mount_point=/mnt/bkpfs/
writefrequency=1
number_of_backups=5

# build user program if not present; mount bkpfs; and create master data and backups 
init_test $mount_point $maxversions $writefrequency $filename $number_of_backups

version_to_delete="all"

# delete the file
./bkpctl -d "$version_to_delete" "$mount_point$filename"
retval=$?

if [ $retval -ne 0 ]; then
        echo "Test Case Failed"
        exit $retval
else
    version_to_restore=3
    ./bkpctl -r "$version_to_restore" "$mount_point$filename"
    retval=$?
    if [ $retval -ne 0 ]; then
        echo "Test Case Passed"
    else
        echo "Test Case Failed"
        exit -1
    fi 
fi

# delete all the backup files and the main file created during the run
teardown_test $filename $mount_point $number_of_backups 
exit 0
