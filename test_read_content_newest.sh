#!/bin/sh

# Testing Read content of a Backup version 

source ./utils.sh

filename=test_file.txt
maxversions=5
mount_point=/mnt/bkpfs/
writefrequency=1
number_of_backups=5

# build user program if not present; mount bkpfs; and create master data and backups 
init_test $mount_point $maxversions $writefrequency $filename $number_of_backups

version_to_read="newest"
expected="12345"

actual=$(./bkpctl -v "$version_to_read" "$mount_point$filename")
retval=$?

if [ $retval -ne 0 ]; then
	echo "Test Case Failed"
	exit $retval
else
	actual=$(echo $actual)
	if [[ "$expected" == "$actual" ]]; then
		echo "Test Case Passed"
	else
		echo "Test Case Failed"
		exit -1
	fi
fi

# delete all the backup files and the main file created during the run
teardown_test $filename $mount_point $number_of_backups 
exit 0
