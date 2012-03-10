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
	
	inline uint16 GetLength();
    inline CryptEngine::Cryptkey GetKey();
    inline uint32 GetStatus();
    inline bool GetCompressed();
private:
	virtual void ProcessPacket() = 0;

protected:
    uint8 m_Startbit;
    uint8 m_Command;
    uint16 m_Length;
    CryptEngine::Cryptkey m_Key;
    uint32 m_Status;
    bool m_Compressed;

    Buffer_ptr m_Buffer;
};

uint16 PacketBase::GetLength() {
	return m_Length;
}

CryptEngine::Cryptkey PacketBase::GetKey() {
	return m_Key;
}

uint32 PacketBase::GetStatus() {
	return m_Status;
}

bool PacketBase::GetCompressed() {
	return m_Compressed;
}


} //namespace shared
} //namespace common
} //namespace jRylServer
#endif