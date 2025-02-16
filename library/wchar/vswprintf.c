/*
 * $Id: wchar_vswprintf.c,v 1.3 2006-01-08 12:04:27 clib2devs Exp $
*/

#ifndef _STDIO_HEADERS_H
#include "stdio_headers.h"
#endif /* _STDIO_HEADERS_H */

#ifndef _WCHAR_HEADERS_H
#include "wchar_headers.h"
#endif /* _WCHAR_HEADERS_H */

#include "wchar_wprintf_core.h"

int
vswprintf(wchar_t *s, size_t l, const wchar_t *fmt, va_list ap)
{
    va_list ap2;
    int ret, nl_type[NL_ARGMAX] = {0};
    union arg nl_arg[NL_ARGMAX];
    FOut _out[1];

    va_copy(ap2, ap);

    if ((!s) || (!l))
    {
        ret = wprintf_core(0, fmt, &ap2, nl_arg, nl_type);
        va_end(ap2);
        return ret;
    }
    out_init_buffer(_out, s, l);
    ret = wprintf_core(_out, fmt, &ap2, nl_arg, nl_type);

    va_end(ap2);

    if (out_overflow(_out))
        return EOF;
    return ret;
}
