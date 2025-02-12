/*
 * $Id: math_fpclassify.c,v 1.4 2006-01-08 12:04:23 clib2devs Exp $
*/

#ifndef _MATH_HEADERS_H
#include "math_headers.h"
#endif /* _MATH_HEADERS_H */

int
__fpclassify_float(float f) {
    union IEEEf2bits u;

    u.f = f;
    if (u.bits.exp == 255) {
        if (u.bits.man == 0) {
            return FP_INFINITE;
        } else {
            return FP_NAN;
        }
    } else if (u.bits.exp != 0) {
        return FP_NORMAL;
    } else if (u.bits.man == 0) {
        return FP_ZERO;
    } else {
        return FP_SUBNORMAL;
    }
}

int
__fpclassify_double(double d) {
    union IEEEd2bits u;

    u.d = d;

    if (u.bits.exp == 2047) {
        if (u.bits.manl == 0 && u.bits.manh == 0) {
            return FP_INFINITE;
        } else {
            return FP_NAN;
        }
    } else if (u.bits.exp != 0) {
        return FP_NORMAL;
    } else if (u.bits.manl == 0 && u.bits.manh == 0) {
        return FP_ZERO;
    } else {
        return FP_SUBNORMAL;
    }
}


int
__fpclassify_long_double(long double e) {
    union IEEEl2bits u;

    u.e = e;
    mask_nbit_l(u);
    if (u.bits.exp == 32767) {
        if (u.bits.manl == 0 && u.bits.manh == 0) {
            return FP_INFINITE;
        } else {
            return FP_NAN;
        }
    } else if (u.bits.exp != 0) {
        return FP_NORMAL;
    } else if (u.bits.manl == 0 && u.bits.manh == 0) {
        return FP_ZERO;
    } else {
        return FP_SUBNORMAL;
    }
}