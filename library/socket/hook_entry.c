/*
 * $Id: socket_hook_entry.c,v 1.19 2021-01-231 13:12:58 clib2devs Exp $
*/

#ifndef _SOCKET_HEADERS_H
#include "socket_headers.h"
#endif /* _SOCKET_HEADERS_H */

#include <sys/ioctl.h>

int64_t
__socket_hook_entry(struct fd *fd, struct file_action_message *fam) {
    struct ExamineData *fib;
    BOOL is_aliased;
    int result;
    int param;

    assert(fam != NULL && fd != NULL);

    /* Careful: file_action_close has to monkey with the file descriptor
                table and therefore needs to obtain the stdio lock before
                it locks this particular descriptor entry. */
    if (fam->fam_Action == file_action_close)
        __stdio_lock();

    __fd_lock(fd);

    switch (fam->fam_Action) {
        case file_action_read:
            SHOWMSG("file_action_read");

            assert(fam->fam_Data != NULL);
            assert(fam->fam_Size > 0);

            SHOWPOINTER(fam->fam_Data);
            SHOWVALUE(fam->fam_Size);

            result = __recv(fd->fd_Socket, fam->fam_Data, fam->fam_Size, 0);
            if (result < 0)
                fam->fam_Error = __get_errno();

            break;

        case file_action_write:
            SHOWMSG("file_action_write");

            assert(fam->fam_Data != NULL);
            assert(fam->fam_Size > 0);

            SHOWPOINTER(fam->fam_Data);
            SHOWVALUE(fam->fam_Size);

            result = __send(fd->fd_Socket, fam->fam_Data, fam->fam_Size, 0);
            if (result < 0)
                fam->fam_Error = __get_errno();

            break;

        case file_action_close:
            SHOWMSG("file_action_close");
            result = OK;

            /* If this is an alias, just remove it. */
            is_aliased = __fd_is_aliased(fd);
            if (is_aliased) {
                __remove_fd_alias(fd);
            } else {
                /* Are we permitted to close this file? */
                if (FLAG_IS_CLEAR(fd->fd_Flags, FDF_NO_CLOSE)) {
                    result = __CloseSocket(fd->fd_Socket);
                }
            }
            __fd_unlock(fd);

            /* Free the lock semaphore now. */
            if (NOT is_aliased)
                __delete_semaphore(fd->fd_Lock);

            /* And that's the last for this file descriptor. */
            memset(fd, 0, sizeof(*fd));
            fd = NULL;
            break;
        case file_action_seek:
            SHOWMSG("file_action_seek");
            result = ERROR;
            fam->fam_Error = ESPIPE;
            break;
        case file_action_set_blocking:
            SHOWMSG("file_action_set_blocking");
            param = (int) (fam->fam_Arg == 0);
            result = __IoctlSocket(fd->fd_Socket, FIONBIO, &param);
            if (result < 0)
                fam->fam_Error = __get_errno();
            break;
        case file_action_set_async:
            SHOWMSG("file_action_set_async");
            param = (int) (fam->fam_Arg != 0);
            result = __IoctlSocket(fd->fd_Socket, FIOASYNC, &param);
            if (result < 0)
                fam->fam_Error = __get_errno();
            break;
        case file_action_examine:
            SHOWMSG("file_action_examine");
            fib = fam->fam_FileInfo;
            memset(fib, 0, sizeof(*fib));
            fib->Type = ST_SOCKET;
            DateStamp(&fib->Date);
            result = OK;
            break;

        default:
            SHOWVALUE(fam->fam_Action);
            result = ERROR;
            fam->fam_Error = EBADF;
            break;
    }

    __fd_unlock(fd);

    if (fam->fam_Action == file_action_close)
        __stdio_unlock();

    RETURN(result);
    return (result);
}
