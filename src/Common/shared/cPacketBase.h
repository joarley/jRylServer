#ifndef PACKET_BASE_H
#define PACKET_BASE_H

#include <string>

#include "shared/typedef.h"
#include "shared/cBuffer.h"
#include "shared/stdDefs.h"
#include "shared/crypt/cCryptEngine.h"

using namespace std;

namespace jRylServer {
namespace common {
namespace shared {

using namespace crypt;

class PacketBase {
public:
    PacketBase(uint8 command, uint16 paketLength);
    PacketBase(uint8 command);
    PacketBase(Buffer_ptr buffer);
    Buffer_ptr GetProcessedBuffer();
protected:
    uint8 m_Startbit;
    uint8 m_Command;
    uint16 m_Length;
    CryptEngine::Cryptkey m_Key;
    uint32 m_Status;
    bool m_Compressed;

    Buffer_ptr m_Buffer;
};
} //namespace shared
} //namespace common
} //namespace jRylServer
#endif