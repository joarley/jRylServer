#ifndef _UTILS_H_
#define _UTILS_H_

#include <ctype.h>

#include "shared/typedef.h"

namespace jRylServer {
namespace common {
namespace shared {

template<class T> inline byte GetByte(T value, int pos) {
    return (byte) (value >> (pos * 8));
}

template<class T> inline T RotateL(T value, int shift) {
    return (value << shift) | (value >> (sizeof (T) * 8 - shift));
}

template<class T> inline T RotateR(T value, int shift) {
    return (value >> shift) | (value << (sizeof (T) * 8 - shift));
}

template<class T> inline T EndianChange(T value) {
    byte* src = (byte*) & value;
    byte dst[sizeof (T)];
    for (int i = 0, j = sizeof (T) - 1; i < sizeof (T); i++, j--) {
        dst[j] = src[i];
    }
    return *((T*)dst);
}

template<class T> T j_atoi(const char *nptr) {
    char *s = (char *) nptr;
    T acc = 0;
    int neg = 0;

    while (isspace((int) *s))
        s++;
    if (*s == '-') {
        neg = 1;
        s++;
    } else if (*s == '+')
        s++;

    while (isdigit((int) *s)) {
        acc = 10 * acc + ((int) *s - '0');
        s++;
    }

    if (neg)
        acc *= -1;
    return acc;
}

template<class T> T j_atoui(const char *nptr) {
    char *s = (char *) nptr;
    T acc = 0;

    while (isspace((int) *s))
        s++;
    while (isdigit((int) *s)) {
        acc = 10 * acc + ((int) *s - '0');
        s++;
    }
    return acc;
}

} //namespace shared
} //namespace common
} //namespace jRylServer

#endif