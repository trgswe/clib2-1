#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#include <features.h>

__BEGIN_DECLS

#include <sys/types.h>

#define WNOHANG 1
#define WUNTRACED 2
   
#define WIFEXITED(w)	(((w) & 0xff) == 0)
#define WIFSIGNALED(w)	(((w) & 0x7f) > 0 && (((w) & 0x7f) < 0x7f))
#define WIFSTOPPED(w)	(((w) & 0xff) == 0x7f)
#define WEXITSTATUS(w)	(((w) >> 8) & 0xff)
#define WTERMSIG(w)	((w) & 0x7f)
#define WSTOPSIG	WEXITSTATUS

__END_DECLS

#endif