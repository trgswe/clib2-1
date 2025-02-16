/*
 * $Id: uio_writev.c,v 1.5 2006-01-08 12:04:27 clib2devs Exp $
*/

#ifndef _UIO_HEADERS_H
#include "uio_headers.h"
#endif /* _UIO_HEADERS_H */

ssize_t
writev(int file_descriptor, const struct iovec *iov, int vec_count) {
    ssize_t result = EOF;
    struct file_action_message msg;
    ssize_t total_num_bytes_written;
    char *buffer = NULL;
    struct fd *fd = NULL;
    int i;

    ENTER();

    SHOWVALUE(file_descriptor);
    SHOWPOINTER(iov);
    SHOWVALUE(vec_count);

    __stdio_lock();

    if (iov == NULL) {
        __set_errno(EFAULT);
        goto out;
    }

    if (vec_count < 1 || vec_count > MAX_IOVEC) {
        __set_errno(EINVAL);
        goto out;
    }

    /* Check for overflow. An expensive test, but better to do it
       here than in the write loop. */
    for (i = 0, total_num_bytes_written = 0; i < vec_count; i++) {
        total_num_bytes_written += iov[i].iov_len;
        if (total_num_bytes_written < 0) /* Rollover. */
        {
            __set_errno(EINVAL);
            goto out;
        }
    }

    fd = __get_file_descriptor(file_descriptor);
    if (fd == NULL) {
        __set_errno(EBADF);
        goto out;
    }

    __fd_lock(fd);

    buffer = malloc(total_num_bytes_written);
    if (buffer != NULL) {
        char *b = buffer;

        for (i = 0; i < vec_count; i++) {
            memcpy(b, iov[i].iov_base, iov[i].iov_len);
            b += iov[i].iov_len;
        }

        msg.fam_Action = file_action_write;
        msg.fam_Data = buffer;
        msg.fam_Size = total_num_bytes_written;

        if ((*fd->fd_Action)(fd, &msg) == EOF) {
            __set_errno(msg.fam_Error);
            goto out;
        }
    } else {
        for (i = 0; i < vec_count; i++) {
            __check_abort();

            if (iov[i].iov_len > 0) {
                msg.fam_Action = file_action_write;
                msg.fam_Data = (char *) iov[i].iov_base;
                msg.fam_Size = iov[i].iov_len;

                if ((*fd->fd_Action)(fd, &msg) == EOF) {
                    __set_errno(msg.fam_Error);
                    goto out;
                }
            }
        }
    }

    result = total_num_bytes_written;

out:

    if (buffer != NULL)
        free(buffer);

    __fd_unlock(fd);
    __stdio_unlock();

    RETURN(result);
    return (result);
}
