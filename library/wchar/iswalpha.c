/*
 * $Id: wctype_iswalpha.c,v 1.4 2021-02-03 23:55:27 clib2devs Exp $
*/

#ifndef _WCHAR_HEADERS_H
#include "wchar_headers.h"
#endif /* _WCHAR_HEADERS_H */

#ifndef _WCTYPE_HEADERS_H
#include <wctype.h>
#endif /* _WCTYPE_HEADERS_H */

#include <ctype.h>

#include "utf8alpha.h"

int
iswalpha(wint_t c) {
    int unicode = 0;

    if (__global_clib2->_current_locale[0] == 'C' && __global_clib2->_current_locale[1] == '\0') {
        unicode = 0;
        /* fall-through */
    } else if (!strcmp(__global_clib2->_current_locale, "C-JIS")) {
        c = __jp2uc(c, JP_JIS);
        unicode = 1;
    } else if (!strcmp(__global_clib2->_current_locale, "C-SJIS")) {
        c = __jp2uc(c, JP_SJIS);
        unicode = 1;
    } else if (!strcmp(__global_clib2->_current_locale, "C-EUCJP")) {
        c = __jp2uc(c, JP_EUCJP);
        unicode = 1;
    } else if (!strcmp(__global_clib2->_current_locale, "C-UTF-8")) {
        unicode = 1;
    }

    if (unicode) {
        unsigned const char *table;
        unsigned char *ptr;
        unsigned char ctmp;
        int size;
        wint_t x = (c >> 8);

        /* for some large sections, all characters are alphabetic so handle them here */
        if ((x >= 0x34 && x <= 0x4c) ||
            (x >= 0x4e && x <= 0x9e) ||
            (x >= 0xac && x <= 0xd6) ||
            (x >= 0x200 && x <= 0x2a5))
            return 1;

        switch (x) {
            case 0x00:
                table = u0;
                size = sizeof(u0);
                break;
            case 0x01:
            case 0x15:
            case 0xa0:
            case 0xa1:
            case 0xa2:
            case 0xa3:
            case 0xf9:
            case 0xfc:
            case 0x2f8:
            case 0x2f9:
                return 1;
            case 0x02:
                table = u2;
                size = sizeof(u2);
                break;
            case 0x03:
                table = u3;
                size = sizeof(u3);
                break;
            case 0x04:
                table = u4;
                size = sizeof(u4);
                break;
            case 0x05:
                table = u5;
                size = sizeof(u5);
                break;
            case 0x06:
                table = u6;
                size = sizeof(u6);
                break;
            case 0x07:
                table = u7;
                size = sizeof(u7);
                break;
            case 0x09:
                table = u9;
                size = sizeof(u9);
                break;
            case 0x0a:
                table = ua;
                size = sizeof(ua);
                break;
            case 0x0b:
                table = ub;
                size = sizeof(ub);
                break;
            case 0x0c:
                table = uc;
                size = sizeof(uc);
                break;
            case 0x0d:
                table = ud;
                size = sizeof(ud);
                break;
            case 0x0e:
                table = ue;
                size = sizeof(ue);
                break;
            case 0x0f:
                table = uf;
                size = sizeof(uf);
                break;
            case 0x10:
                table = u10;
                size = sizeof(u10);
                break;
            case 0x11:
                table = u11;
                size = sizeof(u11);
                break;
            case 0x12:
                table = u12;
                size = sizeof(u12);
                break;
            case 0x13:
                table = u13;
                size = sizeof(u13);
                break;
            case 0x14:
                table = u14;
                size = sizeof(u14);
                break;
            case 0x16:
                table = u16;
                size = sizeof(u16);
                break;
            case 0x17:
                table = u17;
                size = sizeof(u17);
                break;
            case 0x18:
                table = u18;
                size = sizeof(u18);
                break;
            case 0x1e:
                table = u1e;
                size = sizeof(u1e);
                break;
            case 0x1f:
                table = u1f;
                size = sizeof(u1f);
                break;
            case 0x20:
                table = u20;
                size = sizeof(u20);
                break;
            case 0x21:
                table = u21;
                size = sizeof(u21);
                break;
            case 0x24:
                table = u24;
                size = sizeof(u24);
                break;
            case 0x30:
                table = u30;
                size = sizeof(u30);
                break;
            case 0x31:
                table = u31;
                size = sizeof(u31);
                break;
            case 0x4d:
                table = u4d;
                size = sizeof(u4d);
                break;
            case 0x9f:
                table = u9f;
                size = sizeof(u9f);
                break;
            case 0xa4:
                table = ua4;
                size = sizeof(ua4);
                break;
            case 0xd7:
                table = ud7;
                size = sizeof(ud7);
                break;
            case 0xfa:
                table = ufa;
                size = sizeof(ufa);
                break;
            case 0xfb:
                table = ufb;
                size = sizeof(ufb);
                break;
            case 0xfd:
                table = ufd;
                size = sizeof(ufd);
                break;
            case 0xfe:
                table = ufe;
                size = sizeof(ufe);
                break;
            case 0xff:
                table = uff;
                size = sizeof(uff);
                break;
            case 0x103:
                table = u103;
                size = sizeof(u103);
                break;
            case 0x104:
                table = u104;
                size = sizeof(u104);
                break;
            case 0x1d4:
                table = u1d4;
                size = sizeof(u1d4);
                break;
            case 0x1d5:
                table = u1d5;
                size = sizeof(u1d5);
                break;
            case 0x1d6:
                table = u1d6;
                size = sizeof(u1d6);
                break;
            case 0x1d7:
                table = u1d7;
                size = sizeof(u1d7);
                break;
            case 0x2a6:
                table = u2a6;
                size = sizeof(u2a6);
                break;
            case 0x2fa:
                table = u2fa;
                size = sizeof(u2fa);
                break;
            default:
                return 0;
        }
        /* we have narrowed down to a section of 256 characters to check */
        /* now check if c matches the alphabetic wide-chars within that section */
        ptr = (unsigned char *) table;
        ctmp = (unsigned char) c;
        while (ptr < table + size) {
            if (ctmp == *ptr)
                return 1;
            if (ctmp < *ptr)
                return 0;
            /* otherwise c > *ptr */
            /* look for 0x0 as next element which indicates a range */
            ++ptr;
            if (*ptr == 0x0) {
                /* we have a range..see if c falls within range */
                ++ptr;
                if (ctmp <= *ptr)
                    return 1;
                ++ptr;
            }
        }
        /* not in table */
        return 0;
    }

    return (c < (wint_t) 0x100 ? isalpha(c) : 0);
}
