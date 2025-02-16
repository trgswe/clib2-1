/*
 * $Id: strings_strncasecmp.c,v 1.5 2006-01-08 12:04:27 clib2devs Exp $
*/

#ifndef _STDLIB_HEADERS_H
#include "stdlib_headers.h"
#endif /* _STDLIB_HEADERS_H */

#ifndef _STRINGS_HEADERS_H
#include "strings_headers.h"
#endif /* _STRINGS_HEADERS_H */

int
strncasecmp(const char *_s1, const char *_s2, size_t n) {
    const unsigned char *s1 = (const unsigned char *) _s1;
    const unsigned char *s2 = (const unsigned char *) _s2;
    unsigned char c1, c2;
    int result = 0;

    assert(s1 != NULL && s2 != NULL);
    assert((int) n >= 0);

    if (s1 == NULL || s2 == NULL) {
        __set_errno(EFAULT);
        goto out;
    }

    if (s1 != s2) {
        while (n-- > 0) {
            c1 = toupper(*s1++);
            c2 = toupper(*s2++);

            if (c1 == c2) {
                if (c1 == '\0')
                    break;
            } else {
                result = (int) c1 - (int) c2;
                break;
            }
        }
    }

out:

    return (result);
}
