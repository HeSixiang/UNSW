/*
 * Declarations for file handle and file table management.
 */

#ifndef _FILE_H_
#define _FILE_H_

/*
 * Contains some file-related maximum length constants
 */
#include <limits.h>


/*
 * Put your function declarations and data types here ...
 */

typedef struct open_file {
    int reference_num;
    int flag;
    off_t pos;
    struct lock * f_lock;
    struct vnode * vn_pointer;
} File;

int sys_init(void);

int sys_open(userptr_t filename, int flags, mode_t mode, int * retval);
int sys_write(int fd, void * buf, size_t nbytes, size_t *retval);
int sys_close(int fd, int * retval);
int sys_read(int fd, void * buf, size_t buflen, size_t *retval);
int sys_lseek(int fd, off_t pos, int whence, off_t *retval64);
int sys_dup2(int oldfd, int newfd, int * retval);

File * creat_new_file(int flag, struct vnode * vn_pointer, char * lock_name);


#endif /* _FILE_H_ */
