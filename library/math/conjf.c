/*
 * $Id: complex_conjf.c,v 1.2 2006-01-08 12:04:22 clib2devs Exp $
*/

#ifndef _COMPLEX_HEADERS_H
#include "complex_headers.h"
#endif /* _COMPLEX_HEADERS_H */

float complex
conjf(float complex z) {
    return CMPLXF(crealf(z), -cimagf(z));
}
