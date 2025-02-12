/*
 * $Id: debug_kprintf1.c,v 1.3 2006-01-08 12:04:22 clib2devs Exp $
*/

#include "debug_headers.h"

VOID
kprintf(const char *format, ...) {
    assert(format != NULL);

    if (format != NULL) {
        va_list arg;

        va_start(arg, format);
        KPrintF(format, arg);
        va_end(arg);
    }
}
