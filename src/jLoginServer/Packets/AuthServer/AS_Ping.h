#ifndef PacketAuthServer_PING_H
#define	PacketAuthServer_PING_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketAuthServer {

using namespace common::shared;

class Ping: public PacketBase{
public:
    Ping(Buffer_ptr buff);
    virtual ~Ping();
    
    static const uint8 PacketID = 0x59;
    static const uint16 PacketLength = 16;

    inline uint32 GetPingValue();

    //Packet struct
    //uint32 PingValue;
private:
    uint32 m_PingValue;
};

inline uint32 Ping::GetPingValue() {
    return m_PingValue;
}

}
}
}
#endif	/* PacketAuthServer_PING_H */

