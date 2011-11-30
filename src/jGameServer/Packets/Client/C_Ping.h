#ifndef PacketClient_PING_H
#define	PacketClient_PING_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cBuffer.h"

#include "../../cAccount.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketClient {

using namespace common::shared;

class Ping: public PacketBase{
public:
    Ping(Buffer_ptr buff, Account* acc);
    virtual ~Ping();

    //static const uint8 PacketID = 0x59;
    //static const uint16 PacketLength = 16;

    //uint32 PingValue;
private:

};

}
}
}
#endif	/* AUTHACCOUNT_H */

