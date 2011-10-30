#ifndef _UTILS_H_
#define _UTILS_H_

#include "shared/typedef.h"

#include <boost/type_traits/is_float.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>

#include <ctype.h>
#include <cstring>
#include <ctime>


namespace jRylServer {
namespace common {
namespace shared {

void j_GetLocalTime(tm* tmResult);
int j_hexStrToBytes(char* src, byte* dst);
int j_parseHexSeq(char* src, byte* dst);


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

inline uint64 EndianChange(int64 value) {
    return (RotateR(value, 8) & 0xFF000000FF000000)
      | (RotateR(value, 24) & 0x00FF000000FF0000)
      | (RotateL(value, 24) & 0x0000FF000000FF00)
      | (RotateL(value, 8) & 0x000000FF000000FF);
}

inline uint32 EndianChange(int32 value) {
    return (RotateR(value, 8) & 0xFF00FF00)
      | (RotateL(value, 8) & 0x00FF00FF);
}

inline uint16 EndianChange(int16 value) {
    return RotateR(value, 8);
}

template<class T> typename boost::enable_if<boost::is_integral<T>, T>::type j_aton(const char *nptr) {
    return j_atoi<T>(nptr);
}

template<class T> typename boost::enable_if<boost::is_float<T>, T>::type j_aton(const char *nptr) {
    return j_atof<T>(nptr);
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

    if(*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')) {
        s += 2;
        size_t auxsize = sizeof(T) * 2 + 1; 
        char* aux = new char[auxsize];
        memcpy(aux, s, auxsize - 1);
        aux[auxsize - 1] = 0;
        j_hexStrToBytes(aux, (byte*)&acc);
        delete[] aux;
        return EndianChange(acc);
    }
    
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
    
    if(*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')) {
        s += 2;
        size_t auxsize = sizeof(T) * 2 + 1; 
        char* aux = new char[auxsize];
        memcpy(aux, s, auxsize - 1);
        aux[auxsize - 1] = 0;
        j_hexStrToBytes(aux, (byte*)&acc);
        delete[] aux;
        return EndianChange(acc);
    }

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

    static int buffPos = -1;
    static char buff[32][65];
    buffPos++;
    if(buffPos == 32) {
        buffPos = 0;
    }

    char* result = buff[buffPos]; 

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

char* j_ftoa(double in);

template<class T> T j_atof(const char *nptr) {
    return T(atof(nptr));
}
} //namespace shared
} //namespace common
} //namespace jRylServer

#endif