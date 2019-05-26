#!/bin/sh

# Testing Write Frequency other than 1

source ./utils.sh

filename=test_file.txt
maxversions=5
mount_point=/mnt/bkpfs/
# Setting write frequency of two
writefrequency=2
number_of_backups=6

# build user program if not present; mount bkpfs; and create master data and backups 
init_test $mount_point $maxversions $writefrequency $filename $number_of_backups

versions=(1 2 3)
expected=$(get_versions_list "${versions[@]}")

# test both the versions and contents from each versions
actual=$(./bkpctl -l "$mount_point$filename")
retval=$?
if [ $retval -ne 0 ]; then
  echo "Non zero ret value Test Case Failed"
  exit $retval
else
   actual=$(echo $actual)
   if [[ "$expected" == "$actual" ]]; then
        version_to_read=3
        expected_text="12345"
        actual=$(./bkpctl -v "$version_to_read" "$mount_point$filename")
        retval=$?
        if [ $retval -ne 0 ]; then
            echo "Test Case Failed"
            exit $retval
        else
            actual=$(echo $actual)
            if [[ "$expected_text" == "$actual" ]]; then
                echo "Test Case Passed"
            else
                echo "Test Case Failed"
                exit -1
            fi
        fi
    else
  	echo "Test Case Failed"
    exit -1
  fi
fi

# delete all the backup files and the main file created during the run
teardown_test $filename $mount_point $number_of_backups 
exit 0