#ifndef STORELOAD_H
#define STORELOAD_H

/*
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

#endif
