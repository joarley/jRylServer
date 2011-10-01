#ifndef _UTILS_H_
#define _UTILS_H_

#include "shared/typedef.h"

#include <ctype.h>
#include <cstring>
#include <ctime>


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

inline uint64 EndianChange(uint64 value) {
    return (RotateR(value, 8) & 0xFF000000FF000000)
      | (RotateR(value, 24) & 0x00FF000000FF0000)
      | (RotateL(value, 24) & 0x0000FF000000FF00)
      | (RotateL(value, 8) & 0x000000FF000000FF);
}

inline uint32 EndianChange(uint32 value) {
    return (RotateR(value, 8) & 0xFF00FF00)
      | (RotateL(value, 8) & 0x00FF00FF);
}

inline uint16 EndianChange(uint16 value) {
    return RotateR(value, 8);
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

template<class T> char* j_itoa(T value, int base = 10) {
    if (base < 2 || base > 36) {
        return NULL;
    }

    char* result = new char[65];

    char* ptr = result;
    char* ptr1 = result;
    char tmpChar;
    T tmpValue;

    do {
        tmpValue = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmpValue - value * base)];
    } while (value);

    if (tmpValue < 0) {
        *ptr++ = '-';
    }

    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmpChar = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmpChar;
    }
    return result;
}

template<class T> char* j_ftoa(T in) {
    char* value = new char[100];
    sprintf(value, "%f", in);
    return value;
}

template<class T> T j_atof(const char *nptr) {
    return T(atof(nptr));
}

void j_GetLocalTime(tm* tmResult);
int j_hexStrToBytes(char* src, byte* dst);
int j_parseHexSeq(char* src, byte* dst);


} //namespace shared
} //namespace common
} //namespace jRylServer

#endif