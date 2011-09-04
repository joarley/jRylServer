#ifndef CRC32_H
#define CRC32_H
#include "shared/typedef.h"

namespace jRylServer {
namespace common {
namespace shared {

class Crc32 {
public:
    static uint32 Calculate(LPBYTE bytes, size_t length);
private:
    Crc32(void);
};

} //namespace shared
} //namespace common
} //namespace jRylServer
#endif