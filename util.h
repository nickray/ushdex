#ifndef USH_UTIL_H
#define USH_UTIL_H

#include <ctime>
#include <cstdio>

namespace ush {

enum { 
    billion  = 1000000000,
    million  = 1000000,
    thousand = 1000,
};

static inline long nano()
{
    static struct timespec tp;
    if (clock_gettime(CLOCK_REALTIME, &tp) == -1) 
        return -1;
    return tp.tv_sec*billion + tp.tv_nsec;
}

static inline long micro()
{
    static struct timespec tp;
    if (clock_gettime(CLOCK_REALTIME, &tp) == -1) 
        return -1;
    return tp.tv_sec*million + tp.tv_nsec/thousand;
}

static inline const char * const hex_dump(const double d) {
    static char buffer[32];
    sprintf(buffer, "%a", d);
    return buffer;
}

static inline const char * const readable_micro(const long t) {
    static char now_buffer[32 + 1]; // + 1 for '\0'
    long seconds(t/million);
    strftime(now_buffer, 32, "%y%m%d.%H%M%S.", localtime(&seconds));
    snprintf(&now_buffer[14], 32 - 14, "%06ld", t % million);
    return now_buffer;
}

/*
 * Martin Thompson quote:
 *
 * Since we are on x86_64, we can go with the following (we have one writer only!)
 * http://mechanical-sympathy.blogspot.ch/2011/07/
 *     memory-barriersfences.html?showComment=1334410306279#c5059759467872072653
 *
 * You are correct, my mistake.
 *
 * It should be noted that on x86_64 there is no need for a hardware memory
 * barrier at all because the code respects the Single Writer Principle. Other
 * platforms are different!
 *
 * However this does not make any guarantees about the compiler when we go with
 * C/C++. This code needs the synchronisation variables to be declared volatile
 * and access to be wrapped using functions like the following:
 *
 */

template <typename T>
static inline T load(volatile T * ptr) {
    T value = *ptr;
    asm volatile("" ::: "memory");
    return value;
}

template <typename T>
static inline void store(volatile T * ptr, T value) {
    asm volatile ("" ::: "memory");
    *ptr = value;
}

} // namespace ush

#endif // USH_UTIL_H
