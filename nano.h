#ifndef NANO_H
#define NANO_H

#include <time.h>

#define NANOS_PER_SECONDS (__builtin_pow(10, 9))
#define MICROS_PER_SECONDS (__builtin_pow(10, 6))

static inline long nano()
{
    struct timespec tp;
    if (clock_gettime(CLOCK_REALTIME, &tp) == -1) 
        return -1;
    return tp.tv_sec*NANOS_PER_SECONDS + tp.tv_nsec;
}

static inline long micro()
{
    struct timespec tp;
    if (clock_gettime(CLOCK_REALTIME, &tp) == -1) 
        return -1;
    return tp.tv_sec*MICROS_PER_SECONDS + tp.tv_nsec/1000;
}

#endif
