/*
 * $Id: msg_msgget.c,v 1.1 2021-01-26 19:32:53 clib2devs Exp $
*/

#ifdef HAVE_SYSV

#ifndef _SHM_HEADERS_H
#include "shm_headers.h"
#endif /* _SHM_HEADERS_H */

int 
_msgget(key_t key, int flags)
{
    DECLARE_SYSVYBASE();

    ENTER();

    SHOWVALUE(key);
    SHOWVALUE(flags);

    int ret = -1;
    if (__global_clib2->haveShm)
    {
        ret = msgget(key, flags);
        if (ret < 0)
        {
            __set_errno(GetIPCErr());
        }
    }
    else
    {
        __set_errno(ENOSYS);
    }

    RETURN(ret);
    return ret;
}

#endif