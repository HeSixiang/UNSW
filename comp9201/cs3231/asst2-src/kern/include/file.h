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
int open(const char * filename, int flags);
int open(const char * filename, int flags, mode_t mode);
int close(int fd);
ssize_t read(int fd, void *buf )



#endif /* _FILE_H_ */
