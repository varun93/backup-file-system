#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "../fs/bkpfs/ioctl.h"



/* List all file versions */
void list_versions(char *buff, int buff_size)
{
    int index, j;
    char filename[256];
    int version_number = 1;

    for (index = 0; index < buff_size; index++) {
        j = 0;
        while (*(buff + index) != '\0' && index < buff_size) {
            *(filename + j) = *(buff + index);
            index = index + 1;
            j = j + 1;
        }
        *(filename + j) = *(buff + index);
        printf("Backup version %d : %s\n", version_number, filename);
        version_number = version_number + 1;
    }

}

void explain_usage() {
    printf("\n");
    printf("\n");
    printf("================================ Usage ===============\n");
    printf(" Usage  : \n\
        ./bkpctl -[ld:v:r:] FILE \n\n\
           Possible Flags \n\
        -l : List \n\
        -d : Delete; can take newest, oldest, or all \n\
        -r : Restore; newest or N \n\
        -v : View; can take oldest, newest or N \n\
            (where N is a number such as 1, 2, 3, ...) \n\
        -p : Read Offset; offset from where you want to \n\
            start reading the content \n\
        -h : Help  \n\
        ");

    printf("========================== End of Usage ===============\n");
    printf("\n");
}


// Taken from here
// https://stackoverflow.com/questions/24290273/check-if-input-file-is-a-valid-file-in-c
int file_is_regular(char *path) {
    struct stat st;

    if (stat(path, &st) < 0) {
        errno = -EACCES;
        return 0;
    }

    return S_ISREG(st.st_mode);
}


/* Taken from my first assignment */
int validate_path(char *path) {

    if ((errno = access(path, F_OK)) != 0) {
        errno = -ENOENT;
        fprintf(stderr, "Input file not present\n");
        return 0;
    }

    if ((errno = access(path, R_OK)) != 0) {
        errno = -EACCES;
        fprintf(stderr, "Cannot read the input file\n");
        return 0;
    }

    return file_is_regular(path);
}


char *prepare_arguments(bkpfs_ioctl_arguments *arguments, int *operation,
                        int argc, char * const argv[])
{

    int opt;
    int version = 0;
    int read_pos = 0;
    char *delete_arg_src = NULL;
    char *recover_arg_src = NULL;
    char *list_content_arg_src = NULL;
    char *file_path = NULL;

    /* Inialize the argument members */
    arguments->buff = NULL;
    arguments->buff_size = 0;
    arguments->read_pos = 0;
    arguments->version = 0;

    /* cannot have multiple of these flags */
    while ((opt = getopt(argc, argv, "ld:v:r:p:h")) != -1) {
        switch (opt) {
        case 'l':
            *operation |= 1;
            break;
        case 'd' :
            *operation |= 2;
            delete_arg_src = strdup(optarg);
            if (!strcmp(delete_arg_src, "newest"))
                version = -1;
            else if (!strcmp(delete_arg_src, "oldest"))
                version = -2;
            else if (!strcmp(delete_arg_src, "all"))
                version = -3;
            if (!version) {
                fprintf(stderr, "Please specify a valid version");
                explain_usage();
                goto cleanup;
            }

            break;
        case 'v' :
            *operation |= 4;
            list_content_arg_src = strdup(optarg);
            if (!strcmp(list_content_arg_src, "newest"))
                version = -1;
            else if (!strcmp(list_content_arg_src, "oldest"))
                version = -2;
            else
                version = atoi(list_content_arg_src);
            if (!version) {
                fprintf(stderr, "Please specify a valid version");
                explain_usage();
                goto cleanup;
            }
            break;
        case 'p' :
            if (*operation != 0 && *operation != 4) {
                fprintf(stderr, "Read offsets can be used only with read operation");
                explain_usage();
                goto cleanup;
            }
            read_pos = atoi(optarg);
            if (read_pos < 0) {
                errno = -EINVAL;
                fprintf(stderr, "Offsets cannot be negative");
                explain_usage();
                goto cleanup;
            }
            arguments->read_pos = read_pos;
            break;
        case 'r' :
            *operation |= 8;
            recover_arg_src = strdup(optarg);
            if (!strcmp(recover_arg_src, "newest"))
                version = -1;
            else
                version = atoi(recover_arg_src);
            if (!version) {
                fprintf(stderr, "Please specify a valid version");
                explain_usage();
                goto cleanup;
            }
            break;
        case '?':
            if (optopt == 'd' ) {
                fprintf(stderr, "Please Specify a version(s) to delete.\n");
                goto cleanup;
            }
            if (optopt == 'v') {
                fprintf(stderr, "Please Specify a version to view.\n");
                goto cleanup;
            }
            if (optopt == 'r') {
                fprintf(stderr, "Please Specify a version to restore to.\n");
                goto cleanup;
            }
            goto cleanup;
        case 'h' :
            explain_usage();
            goto cleanup;
        default :
            fprintf(stderr, "Please specify a valid operation.\n");
            explain_usage();
            goto cleanup;
        }
    }

    if (*operation == 0) {
        fprintf(stderr, "Please specify atleast one operation \n");
        goto cleanup;
    }

    if (*operation != 1 && *operation != 2 && *operation != 4 && *operation != 8) {
        fprintf(stderr, "Please specify only one operation at a time.\n");
        goto cleanup;
    }

    if (optind < argc) {
        /* The caller should free this  */
        file_path = strdup(argv[optind]);
        if (file_path == 0) {
            file_path = NULL;
            errno = -ENOMEM;
            goto cleanup;
        }
        if (!validate_path(file_path)) {
            errno = -EACCES;
            file_path = NULL;
            fprintf(stderr, "Input file not readable\n");
            goto cleanup;
        }
    }
    else{
        fprintf(stderr, "Input Path is mandatory\n");
        errno = -ENOENT;
        goto cleanup;        
    }

    /* Populate the Version */
    if (version)
        arguments->version = version;

cleanup :

    if (list_content_arg_src)
        free(list_content_arg_src);

    if (delete_arg_src)
        free(delete_arg_src);

    if (recover_arg_src)
        free(recover_arg_src);

    return file_path;

}

int main(int argc, char * const argv[])
{
    int return_value = 0;
    int fd = -1;
    int operation = 0;
    char *buff = NULL;
    char *file_path = NULL;
    bkpfs_ioctl_arguments arguments;

    /* Free the file path */
    file_path = prepare_arguments(&arguments, &operation, argc, argv);

    /* Invalid arguments to the program */
    if (file_path == NULL || !operation){
        return_value = -EINVAL;
        errno = return_value;
        goto out;
    }
        

    fd = open(file_path, O_RDWR, S_IRWXU);

    if (fd == -1) {
        return_value = errno;
        printf("Unable to create file %d\n", errno);
        goto out;
    }

    switch (operation) {
    case 1  :
        arguments.buff_size = 1024;
        buff = malloc(arguments.buff_size);
        if (!buff)
            goto out;
        arguments.buff = buff;
        return_value = ioctl(fd, BKPFS_LIST_VERSIONS, &arguments);
        if (return_value == -ENOENT) {
            fprintf(stderr, "No Backups found\n");
            goto out;
        }
        list_versions(arguments.buff, arguments.buff_size);
        break;
    case 2 :
        return_value = ioctl(fd, BKPFS_DELETE, &arguments);
        if (return_value == -ENOENT) {
            fprintf(stderr, "Trying to delete a non existent backup\n");
            goto out;
        }
        break;
    case 4 :
        arguments.buff_size = 4096;
        buff = malloc(arguments.buff_size);
        if (!buff)
            goto out;
        arguments.buff = buff;
        return_value = ioctl(fd, BKPFS_LIST_CONTENT, &arguments);
        if (return_value == -ENOENT) {
            fprintf(stderr, "Trying to view a non existent backup\n");
            goto out;
        }
        printf("%s", arguments.buff);
        break;
    case 8 :
        return_value = ioctl(fd, BKPFS_RESTORE, &arguments);
        if (return_value == -ENOENT) {
            fprintf(stderr, "Trying to restore from a non existent backup\n");
            goto out;
        }
        break;
    }


out:

    if (buff)
        free(buff);

    if (file_path)
        free(file_path);

    if (fd != -1)
        close(fd);

    exit(return_value);
}
