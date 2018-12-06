#ifndef SGL_UNIX_CRITICAL_SECTION_H
#define SGL_UNIX_CRITICAL_SECTION_H

#include "core_types.h"
#include "hal/pthread_critical_section.h"

/// Unix uses pthreads
typedef PThreadCriticalSection CriticalSection;
typedef PThreadRWLock RWLock;

#endif