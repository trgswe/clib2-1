/*
 * $Id: math_isfinite.c,v 1.3 2022-03-10 12:04:23 clib2devs Exp $
*/


#ifndef _STDIO_HEADERS_H
#include "stdio_headers.h"
#endif /* _STDIO_HEADERS_H */

int
__isfinite_float(float f) {
    union IEEEf2bits u;

    u.f = f;
    return (u.bits.exp != 255);
}

int
__isfinite_double(double d) {
    union IEEEd2bits u;

    u.d = d;
    return (u.bits.exp != 2047);
}

int
__isfinite_long_double(long double d) {
    union IEEEd2bits u;

    u.d = d;
    return (u.bits.exp != 2047);
}