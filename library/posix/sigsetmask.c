/*
 * $Id: posix_sigsetmask.c,v 1.4 2006-01-08 12:04:24 clib2devs Exp $
*/

#ifndef _SIGNAL_HEADERS_H
#include "signal_headers.h"
#endif /* _SIGNAL_HEADERS_H */

int
sigsetmask(int signal_mask)
{
	int result;

	ENTER();

	SHOWVALUE(signal_mask);

	result = __signals_blocked;

	__signals_blocked = signal_mask;

	RETURN(result);
	return(result);
}
