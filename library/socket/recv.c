/*
 * $Id: socket_recv.c,v 1.7 2006-11-16 10:41:15 clib2devs Exp $
*/

#ifndef _SOCKET_HEADERS_H
#include "socket_headers.h"
#endif /* _SOCKET_HEADERS_H */

int
recv(int sockfd, void *buff, size_t nbytes, int flags) {
    struct fd *fd;
    int result = ERROR;

    ENTER();

    SHOWVALUE(sockfd);
    SHOWPOINTER(buff);
    SHOWVALUE(nbytes);
    SHOWVALUE(flags);

    assert((int) nbytes >= 0);
    assert(buff != NULL);
    assert(__SocketBase != NULL);

    if (buff == NULL) {
        SHOWMSG("invalid buffer parameter");

        __set_errno(EFAULT);
        goto out;
    }

    fd = __get_file_descriptor_socket(sockfd);
    if (fd == NULL)
        goto out;

    result = __recv(fd->fd_Socket, buff, (LONG) nbytes, flags);

out:

    __check_abort();

    RETURN(result);
    return (result);
}
