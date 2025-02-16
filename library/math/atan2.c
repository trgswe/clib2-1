/*
 * $Id: math_atan2.c,v 1.10 2021-02-16 07:54:24 clib2devs Exp $
*/

#ifndef _MATH_HEADERS_H
#include "math_headers.h"
#endif /* _MATH_HEADERS_H */

static volatile double
        tiny = 1.0e-300;

static const double
        zero = 0.0,
        pi_o_4 = 7.8539816339744827900E-01, /* 0x3FE921FB, 0x54442D18 */
        pi_o_2 = 1.5707963267948965580E+00, /* 0x3FF921FB, 0x54442D18 */
        pi = 3.1415926535897931160E+00; /* 0x400921FB, 0x54442D18 */

static volatile double
        pi_lo = 1.2246467991473531772E-16; /* 0x3CA1A626, 0x33145C07 */

double
atan2(double y, double x) {
    double z;
    int32_t k, m, hx, hy, ix, iy;
    uint32_t lx, ly;

    EXTRACT_WORDS(hx, lx, x);
    ix = hx & 0x7fffffff;
    EXTRACT_WORDS(hy, ly, y);
    iy = hy & 0x7fffffff;
    if (((ix | ((lx | -lx) >> 31)) > 0x7ff00000) ||
        ((iy | ((ly | -ly) >> 31)) > 0x7ff00000))    /* x or y is NaN */
        return x + y;
    if (hx == 0x3ff00000 && lx == 0) return atan(y);   /* x=1.0 */
    m = ((hy >> 31) & 1) | ((hx >> 30) & 2);    /* 2*sign(x)+sign(y) */

    /* when y = 0 */
    if ((iy | ly) == 0) {
        switch (m) {
            case 0:
            case 1:
                return y;    /* atan(+-0,+anything)=+-0 */
            case 2:
                return pi + tiny;/* atan(+0,-anything) = pi */
            case 3:
                return -pi - tiny;/* atan(-0,-anything) =-pi */
        }
    }
    /* when x = 0 */
    if ((ix | lx) == 0) return (hy < 0) ? -pi_o_2 - tiny : pi_o_2 + tiny;

    /* when x is INF */
    if (ix == 0x7ff00000) {
        if (iy == 0x7ff00000) {
            switch (m) {
                case 0:
                    return pi_o_4 + tiny;/* atan(+INF,+INF) */
                case 1:
                    return -pi_o_4 - tiny;/* atan(-INF,+INF) */
                case 2:
                    return 3.0 * pi_o_4 + tiny;/*atan(+INF,-INF)*/
                case 3:
                    return -3.0 * pi_o_4 - tiny;/*atan(-INF,-INF)*/
            }
        } else {
            switch (m) {
                case 0:
                    return zero;    /* atan(+...,+INF) */
                case 1:
                    return -zero;    /* atan(-...,+INF) */
                case 2:
                    return pi + tiny;    /* atan(+...,-INF) */
                case 3:
                    return -pi - tiny;    /* atan(-...,-INF) */
            }
        }
    }
    /* when y is INF */
    if (iy == 0x7ff00000) return (hy < 0) ? -pi_o_2 - tiny : pi_o_2 + tiny;

    /* compute y/x */
    k = (iy - ix) >> 20;
    if (k > 60) {            /* |y/x| >  2**60 */
        z = pi_o_2 + 0.5 * pi_lo;
        m &= 1;
    } else if (hx < 0 && k < -60) z = 0.0;    /* 0 > |y|/x > -2**-60 */
    else z = atan(fabs(y / x));        /* safe to do y/x */
    switch (m) {
        case 0:
            return z;    /* atan(+,+) */
        case 1:
            return -z;    /* atan(-,+) */
        case 2:
            return pi - (z - pi_lo);/* atan(+,-) */
        default: /* case 3 */
            return (z - pi_lo) - pi;/* atan(-,-) */
    }
}
