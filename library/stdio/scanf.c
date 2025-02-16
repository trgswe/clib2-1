/*
 * $Id: stdio_scanf.c,v 1.8 2006-01-08 12:04:25 clib2devs Exp $
*/

#ifndef _STDIO_HEADERS_H
#include "stdio_headers.h"
#endif /* _STDIO_HEADERS_H */

int
scanf(const char *format, ...) {
    int result = EOF;
    va_list arg;

    ENTER();

    SHOWSTRING(format);

    assert(format != NULL);

    __check_abort();

    if (format == NULL) {
        SHOWMSG("invalid format parameter");

        __set_errno(EFAULT);
        goto out;
    }

    va_start(arg, format);
    result = vscanf(format, arg);
    va_end(arg);

out:

    RETURN(result);
    return (result);
}
