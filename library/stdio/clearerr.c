/*
 * $Id: stdio_clearerr.c,v 1.5 2006-01-08 12:04:24 clib2devs Exp $
*/

#ifndef _STDIO_HEADERS_H
#include "stdio_headers.h"
#endif /* _STDIO_HEADERS_H */

#undef clearerr

void
clearerr(FILE *stream) {
    struct iob *file = (struct iob *) stream;

    ENTER();

    SHOWPOINTER(stream);

    assert(stream != NULL);

    if (stream == NULL) {
        __set_errno(EFAULT);
        goto out;
    }

    assert(__is_valid_iob(file));

    flockfile(stream);

    CLEAR_FLAG(file->iob_Flags, IOBF_ERROR);
    CLEAR_FLAG(file->iob_Flags, IOBF_EOF_REACHED);

    funlockfile(stream);

out:

    LEAVE();
}
