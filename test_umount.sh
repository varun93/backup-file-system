#!/bin/sh

# testing unmounting

source ./utils.sh

mount_path=/mnt/bkpfs/

 # unmount bkpfs if already mounted
  if mountpoint -q $mount_path
        then
          status=$(unmount_bkpfs "$mount_path")

          if [ $status -ne 0 ]; then
            echo "Unmounting Failed"
            exit -1
          else
            echo "Unmounting Succeeded"
          fi
       
        else
          echo "No mount point found"
          exit 0
  fi

