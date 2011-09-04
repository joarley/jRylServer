#include "cCrc32.h"
#include "Crc32.tab"

#include "shared/typedef.h"

namespace jRylServer {
namespace common {
namespace shared {

    uint32 Crc32::Calculate(LPBYTE bytes, size_t length) {
        uint32 crc = 0xffffffff;
        while (length--) {
            crc = (crc >> 8) ^ Table[(crc & 0xFF) ^ *bytes++];
        }    
        return (crc ^ 0xffffffff);
    }

} //namespace shared
} //namespace common
} //namespace jRylServer