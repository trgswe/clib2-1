/*
 * $Id: time_utimes.c,v 1.0 2022-02-24 18:08:57 clib2devs Exp $
*/

#ifndef _TIME_HEADERS_H
#include "time_headers.h"
#endif /* _TIME_HEADERS_H */

#ifndef _STDIO_HEADERS_H
#include "stdio_headers.h"
#endif /* _STDIO_HEADERS_H */

#include <sys/time.h>

int
utimes(const char *name, const struct timeval *tvp) {
    struct name_translation_info path_name_nti;
    struct DateStamp ds;

    ENTER();

    if (__check_abort_enabled)
        __check_abort();

    DECLARE_TIMEZONEBASE();

    if (!tvp) {
        DateStamp(&ds);
    } else {
        time_t mtime = tvp[1].tv_sec;
        int32 __gmtoffset = 0;

        if (UNIX_TIME_OFFSET > mtime) {
            __set_errno(EINVAL);
            return -1;
        }

        GetTimezoneAttrs(NULL, TZA_UTCOffset, &__gmtoffset, TAG_DONE);

        mtime -= UNIX_TIME_OFFSET;
        mtime -= 60 * __gmtoffset;

        ds.ds_Days = mtime / (24 * 60 * 60);
        ds.ds_Minute = (mtime % (24 * 60 * 60)) / 60;
        ds.ds_Tick = (mtime % 60) * TICKS_PER_SECOND
                     + tvp[1].tv_usec / (1000000 / TICKS_PER_SECOND);

        while (ds.ds_Tick >= 60 * TICKS_PER_SECOND) {
            ds.ds_Tick -= 60 * TICKS_PER_SECOND;
            ds.ds_Minute++;
        }
        if (ds.ds_Minute >= 24 * 60) {
            ds.ds_Minute -= 24 * 60;
            ds.ds_Days++;
        }
    }

    if (__unix_path_semantics) {
        if (__translate_unix_to_amiga_path_name(&name, &path_name_nti) != 0) {
            return -1;
        }

        if (path_name_nti.is_root) {
            __set_errno(EPERM);
            return -1;
        }
    }

    if (!SetDate((STRPTR) name, &ds)) {
        __set_errno(__translate_io_error_to_errno(IoErr()));
        return -1;
    }

    return 0;
}