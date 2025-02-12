/*
  $Id: sched.h,v 1.01 2022-07-18 12:09:49 clib2devs Exp $

  Copyright (C) 2014 Szilard Biro

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef SCHED_H
#define SCHED_H

#include <time.h>
#include <features.h>

struct sched_param
{
    int sched_priority;
};

#define SCHED_NORMAL	0
#define SCHED_OTHER		0
#define SCHED_FIFO		1
#define SCHED_RR		2
#define SCHED_BATCH		3

__BEGIN_DECLS

extern int sched_get_priority_max(int);
extern int sched_get_priority_min(int);
//extern int sched_getparam(pid_t, struct sched_param *);
//extern int sched_getscheduler(pid_t);
//extern int sched_rr_get_interval(pid_t, struct timespec *);
//extern int sched_setparam(pid_t, const struct sched_param *);
//extern int sched_setscheduler(pid_t, int, const struct sched_param *);
extern int sched_yield(void);

__END_DECLS

#endif
