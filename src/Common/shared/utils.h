#ifndef _UTILS_H_
#define _UTILS_H_

#include <ctype.h>
#include <cstring>

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

template<class T> char* j_itoa(T in) {
    char buf[19 + 2] = {};   //19 enough for 64 bit integer
    char *p = buf + 19 + 1;	/* points to terminating '\0' */
    if (in >= 0) {
        do {
            *--p = '0' + (in % 10);
            in /= 10;
        } while (in != 0);
    }
    else {			/* i < 0 */
        do {
            *--p = '0' - (in % 10);
            in /= 10;
        } while (in != 0);
        *--p = '-';
    }

    char* value = new char[strlen(p) + 1];
    strcpy(value, p);
    return value;
}

template<class T> char* j_ftoa(T in) {
    char* value = new char[100];
    sprintf(value, "%f", in);
}

template<class T> T j_atof(const char *nptr) {
    return T(atof(nptr));
}
} //namespace shared
} //namespace common
} //namespace jRylServer

#endif