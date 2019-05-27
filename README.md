## Backup File System


**Note**  

This project was undertaken as a part of my Operating System Coursework. I am not supposed to publish the source code or even the design decisions in a public forum. Hence I have committed only the test cases which should hopefully provide you a gist of the project.

If you want to have a peek at the source code please contact me at vhegde@cs.stonybrook.edu.    

**Description**

This is an implementation of a backup file system build using wrapfs, which is a null layer file system. It allows user to automatically create backups of regular files defined by **write frequency** interval.

### Important Features

- Create backups of a file for every n writes; defined by **writefrequency**.
- Make backups invisble to the user.(even with ls -a its not visible)
- List all versions of a backup file.
- Delete a oldest, newest or all versions of the file.
- View Oldest, Newest or Nth version of a file from a given offset.
- Restore newest or Nth version of a file.


### Build

```
cd repo
make clean
make
```

### Mount Filesystem

```
mount -t filesytem
-o maxver=$maxversions
-o writefrequency=$writefrequency
$lower_level_path $mount_path
```

### Unmount Filesystem

```
umount $mount_path
```

### User Program Usage

```
cd CSE-506
make clean
make
```

bkpctl is the executable.

**List all Versions**
`./bkpctl -l $filepath`
**Delete a Specific Version**
`./bkpctl -d $version_to_delete $filepath`
**View a Specific Version for a given offset**
`./bkpctl -v $version_to_view -p $offset $filepath`
**Restore a Specific Version**
`./bkpctl -r $version_to_restore $filepath`

### All Flags

```
    -l : List all Versions
    -d : Delete a Version
        - newest
        - oldest
        - all
    -v : View contens of a version
        - newest
        - oldest
        - N
    -p : offset from where to start reading
    -r : Restore a version
        - newest
        - N
    -h : Help
```

## Test Cases

- All the test cases are self sufficient. The only pre-requisiste is that bkpfs module is loaded using insmod,before executing the test cases.
- Some of the operations like mount unmount writing /mnt/bkpfs require root or relevant privileges.
- Have catalogued all the test cases at the end of the document for easier reference.

### Test Cases Catalogue

utils.sh - This has common scripts shared by all other test cases such as mounting, unmounting,
creating backups, cleanup of backups ones the test cases are over.

1. Mount - test_mount.sh
2. Unmount - test_umount.sh
3. List Versions - test_list_versions.sh
4. List Versions of file with 0 backups - test_no_backups.sh
5. Backup Retention Policy - test_backups_after_max.sh
6. Write Frequency - test_write_frequency.sh
7. Delete Newest - test_delete_versions_newest.sh
8. Delete Oldest - test_delete_versions_oldest.sh
9. Delete All - test_delete_versions_all.sh
10. Delete a Non Existent Backup - test_delete_non_existent.sh
11. Backup Visibility Policy - test_hidden_backups.sh
12. View Contents of a Version N - test_read_content.sh
13. View Contents Oldest - test_read_content_oldest.sh - NA
14. View Content Newest - test_read_content_newest.sh - NA
15. View Contents With offsets - test_read_content_with_offset.sh
16. View Contents of a Non Existent Backup - test_view_non_existent.sh
17. Restore from a Version N - test_restore_n.sh
18. Restore latest Version - test_restore_newest.sh
19. Restore a non existent backup - test_restore_non_existent.sh
    User Program tests
    ***
20. No operation specified - test_no_actions.sh
21. Multiple operations specified - test_multiple_actions.sh
22. No input path provided - test_no_input_path.sh
23. Invalid Delete Params - test_invalid_delete_params.sh
24. Invalid Restore Params - test_invalid_restore_params.sh
25. Invalid View Params - test_invalid_view_params.sh
