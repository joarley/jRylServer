#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_
#include <boost/cstdint.hpp>

namespace jRylServer {
namespace common {
namespace shared {
namespace math {
class Half;
}//namespace math
}//namespace shared
}//namespace common
}//namespace jRylServer

typedef boost::int64_t int64;
typedef boost::int32_t int32;
typedef boost::int16_t int16;
typedef boost::int8_t int8;
typedef boost::uint64_t uint64;
typedef boost::uint32_t uint32;
typedef boost::uint16_t uint16;
typedef boost::uint8_t uint8;
typedef jRylServer::common::shared::math::Half f16;
typedef float f32;
typedef double f64;
typedef uint8 byte;

#ifdef _WIN32
    #include <WinSock2.h>
    #include <Windows.h>
#else
    typedef void* LPVOID;
    typedef uint32 DWORD;
    typedef uint16 WORD;
    typedef byte BYTE;
    typedef BYTE* LPBYTE;
    typedef const char* LPCSTR;
    typedef DWORD* LPDWORD;
#endif

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

#endif