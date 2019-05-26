#!/bin/sh
# Mounting of Bkps is successful 

# import some utilities 
source ./utils.sh

number_of_backups=5
mount_path=/mnt/bkpfs/
writefrequency=2

 # unmount bkpfs if already mounted
  if mountpoint -q $mount_path
        then
          status=$(unmount_bkpfs "$mount_path")
          if [ $status -ne 0 ]; then
           exit -1
          fi
  fi

# now do the acout
status=$(mount_bkpfs "$number_of_backups" "$mount_path" "$writefrequency")

if [ "$status" -ne 0 ]; then
   echo "Mouting Failed"
   exit -1
else
  echo "Mounting Succeeded"
fi

exit 0

