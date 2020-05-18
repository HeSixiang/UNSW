#include <types.h>
#include <kern/errno.h>
#include <kern/fcntl.h>
#include <kern/limits.h>
#include <kern/stat.h>
#include <kern/seek.h>
#include <lib.h>
#include <uio.h>
#include <thread.h>
#include <current.h>
#include <synch.h>
#include <vfs.h>
#include <vnode.h>
#include <file.h>
#include <syscall.h>
#include <copyinout.h>

#include <proc.h>

File * creat_new_file(int flag, struct vnode * vn_pointer, char * lock_name) {
    File * new;
    new = kmalloc(sizeof(File));
    KASSERT(new != NULL);
    if (new == NULL) {
        return NULL;
    }
    new->flag = flag;
    new->reference_num = 1;
    new->pos = 0;
    new->vn_pointer = vn_pointer;
    new->f_lock = lock_create(lock_name);
    if (new->f_lock == NULL) {
        return NULL;
    }
    return new;
}


int sys_init() {
    char c1[] = "con:";
    char c2[] = "con:";

    //For flag, since you want to print to stdout and stderr,
    //and it doesn't make sense to read from it, O_WRONLY.
    int f1 = O_WRONLY;
    int f2 = O_WRONLY;

    //For mode, it only makes sense with O_CREAT, 
    //so it's ignored here. You can just pass in 0.
    int m1 = 0;
    int m2 = 0;

    struct vnode * v1 = NULL;
    struct vnode * v2 = NULL;

    int r1 = 0;
    r1 = vfs_open(c1,f1,m1,&v1); 

    if (r1) {
        vfs_close(v1);
        return r1;
    } 

    int r2 = 0;
    r2 = vfs_open(c2,f2,m2,&v2); 
    if (r2) {
        vfs_close(v1);
        vfs_close(v2);
        return r2;
    } 
    
    char n1[] = "stdout";
    char n2[] = "stderr";

    File *file_1 = creat_new_file(f1, v1, n1);
    File *file_2 = creat_new_file(f2, v2, n2);

    //something wrong
    if (file_1 == NULL || file_2 == NULL) {
        if (file_1 != NULL) {
            lock_destroy(file_1->f_lock);
        }
        if (file_2 != NULL) {
            lock_destroy(file_2->f_lock);
        }
        kfree(file_1);
        kfree(file_2);
        return ENOMEM;
    }
    
    curproc->file_table[1] = file_1;
    curproc->file_table[2] = file_2;

    return 0;
}

int sys_open(userptr_t filename, int flags, mode_t mode, int * retval) {
    //filename was an invalid pointer
    if (filename == NULL) {
        return EFAULT;
    }

    char * file_name;
    int result;

    int start_index = 3; //0, 1, 2 for std

    // check filename size should use PATH_MAX not NAME_MAX
    // becasue this file name is path included parent dir. 
    file_name = kmalloc(PATH_MAX * sizeof(char));
	if (file_name == NULL) {
		return ENOMEM;
	}

    result = copyinstr(filename, file_name, NAME_MAX, NULL);
    if (result) {
        kfree(file_name);
        return result;
    }

    //initial to error
    *retval = -1;
    //find the free space
    for (int i = start_index; i < OPEN_MAX; i++) {
        if (curproc->file_table[i] == NULL) { 
            *retval = i;
            break;
        }
    }
    //do not find the free index, all full
    if (*retval == -1) {
        kfree(file_name);
        return EMFILE;
    }
    struct vnode * v = NULL;
    int r = 0;
    
    // lots of checking should be happened in vfs_open
    // ENODEV	The device prefix of filename did not exist
    // ENOTDIR	A non-final component of filename was not a directory
    // ENOENT	A non-final component of filename did not exist.
    // ...
    r = vfs_open(file_name, flags, mode, &v);
    if (r) {
        vfs_close(v);
        return r;
    } 
    File *file = creat_new_file(flags, v, file_name);
    if (file == NULL) {
        vfs_close(v);
        if (file->f_lock != NULL) {
            lock_destroy(file->f_lock);
        }
        kfree(file);
        return ENOMEM;
    }
    curproc->file_table[*retval] = file;
    return 0; //success
}

int sys_write(int fd, void * buf, size_t nbytes, size_t *retval) {
    // fd is not a valid file descriptor,
    if (fd < 0 || fd >= OPEN_MAX || curproc->file_table[fd] == NULL) {
        return EBADF;
    }
    
    //or was not opened for writing
    int fd_flag = curproc->file_table[fd]->flag & O_ACCMODE;
    if (fd_flag != O_WRONLY && fd_flag != O_RDWR) {
        return EBADF;
    }

    int result = 0;

    // get lock
    lock_acquire(curproc->file_table[fd]->f_lock);
    

    struct iovec iov;
    struct uio u;

    uio_uinit(&iov, &u, buf, nbytes, curproc -> file_table[fd]->pos, UIO_WRITE);
    
    struct vnode *vn = curproc->file_table[fd]->vn_pointer;

    // I think the VOP_WRITE will check for the error
    // EFAULT	Part or all of the address space pointed to by buf is invalid.
    // ENOSPC	There is no free space remaining on the filesystem containing the file.
    // EIO	A hardware I/O error occurred writing the data.
    result = VOP_WRITE(vn, &u);
    if (result) {
        lock_release(curproc->file_table[fd]->f_lock);
        return result;
    }
    curproc->file_table[fd]->pos = u.uio_offset; 
    *retval = nbytes - u.uio_resid;
    lock_release(curproc->file_table[fd]->f_lock);
    return 0; //success
}

int sys_close(int fd, int * retval) {
    *retval = 0;
    //fd is not a valid file handle
    if (fd < 0 || fd >= OPEN_MAX || curproc->file_table[fd] == NULL) {
        *retval = EBADF;
        return EBADF;
    }
    lock_acquire(curproc->file_table[fd]->f_lock);
    // According to POSIX, even if the underlying operation fails, 
    // the file is closed anyway and the file handle becomes invalid
    if (curproc->file_table[fd]->reference_num == 1) {
        // vfs_close VOP_DECREF doesn't return an error.
        // we ignore EIO A hard I/O error occurred.
        vfs_close(curproc->file_table[fd]->vn_pointer);
        lock_release(curproc->file_table[fd]->f_lock);
        lock_destroy(curproc->file_table[fd]->f_lock);
        kfree(curproc->file_table[fd]);
        curproc->file_table[fd] = NULL;
        return 0; //success
    }
    curproc->file_table[fd]->reference_num -= 1;
    lock_release(curproc->file_table[fd]->f_lock);
    curproc->file_table[fd] = NULL;
    return 0; //success
}

int sys_read(int fd, void * buf, size_t buflen, size_t *retval) {
    //fd is not a valid file descriptor,
    if (fd < 0 || fd >= OPEN_MAX || curproc->file_table[fd] == NULL) {
        return EBADF;
    }

    // or was not opened for reading.
    int fd_flag = curproc->file_table[fd]->flag & O_ACCMODE;
    if (fd_flag != O_RDONLY && fd_flag != O_RDWR) {
        return EBADF;
    }

    int result = 0;
    
    lock_acquire(curproc->file_table[fd]->f_lock);

    struct iovec iov;
    struct uio u;

    uio_uinit(&iov, &u, buf, buflen, curproc -> file_table[fd]->pos, UIO_READ);
    
    struct vnode *vn = curproc->file_table[fd]->vn_pointer;
    
    // I think the VOP_READ will check error for
    // EFAULT Part or all of the address space pointed to by buf is invalid.
    // EIO	A hardware I/O error occurred reading the data.
    result = VOP_READ(vn, &u);
    if (result) {
        lock_release(curproc->file_table[fd]->f_lock);
        return result;
    }
    curproc->file_table[fd]->pos = u.uio_offset; 
    *retval = buflen - u.uio_resid;
    lock_release(curproc->file_table[fd]->f_lock);
    return 0; //success
}

int sys_lseek(int fd, off_t pos, int whence, off_t *retval64) {
    if (fd < 0 || fd >= OPEN_MAX || curproc->file_table[fd] == NULL) {
        return EBADF;
    }
    int result;
    lock_acquire(curproc->file_table[fd]->f_lock);

    if (!VOP_ISSEEKABLE(curproc->file_table[fd]->vn_pointer)) {
        lock_release(curproc->file_table[fd]->f_lock);
        return ESPIPE; 
    }

    struct stat file_info;
    result = VOP_STAT(curproc->file_table[fd]->vn_pointer, &file_info);
    if (result) {
        lock_release(curproc->file_table[fd]->f_lock);
        return result;
    }

    if (whence == SEEK_SET) {
        *retval64 = pos;
    } else if (whence == SEEK_CUR) {
        *retval64 = curproc->file_table[fd]->pos + pos;
    } else if (whence == SEEK_END) {
        *retval64 = pos + file_info.st_size;
    } else {
        //somthing woring
        lock_release(curproc->file_table[fd]->f_lock);
        return EINVAL;
    }

    //The resulting seek position would be negative.
    if (*retval64 < 0) {
        lock_release(curproc->file_table[fd]->f_lock);
        return EINVAL;
    }
    //lseek() allows the file offset to be set beyond the end of the file
    //(but this does not change the size of the file).
    //no need to check
    curproc->file_table[fd]->pos = *retval64;
    lock_release(curproc->file_table[fd]->f_lock);
    return 0;
}

int sys_dup2(int oldfd, int newfd, int * retval) {
    //initial to -1  error
    * retval = -1;
    if (oldfd < 0 || oldfd >= OPEN_MAX || curproc->file_table[oldfd] == NULL) {
        return EBADF;
    }
    if (newfd < 0 || newfd >= OPEN_MAX) {
        return EBADF;
    } 

    // if equals
    if (oldfd == newfd) {
        * retval = oldfd;
        return 0;
    }
    if (curproc->file_table[newfd] != NULL) {
        //If newfd names an already-open file, that file is closed.
        sys_close(newfd, retval);
    }
    curproc->file_table[newfd] = curproc->file_table[oldfd];
    lock_acquire(curproc->file_table[oldfd]->f_lock);  
    curproc->file_table[oldfd]->reference_num += 1;
    lock_release(curproc->file_table[oldfd]->f_lock);
    * retval = newfd;
    return 0;
}

