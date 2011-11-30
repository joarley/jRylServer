#ifndef PacketLauncher_PING_H
#define	PacketLauncher_PING_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketLauncher {

using namespace common::shared;

class Ping: public PacketBase{
public:
    Ping(Buffer_ptr buff);
    virtual ~Ping();

    static const uint8 PacketID = 0x59;
    static const uint16 PacketLength = 16;

    //Packet struct
    //uint32 PingValue;
private:

};

}
}
}
#endif	/* AUTHACCOUNT_H */

