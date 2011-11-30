#ifndef PacketServer_AuthServer_SERVERDETAIL_H
#define	PacketServer_AuthServer_SERVERDETAIL_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketServer_AuthServer {

using namespace common::shared;

class ServerDetail: public PacketBase{
public:
    ServerDetail();
    virtual ~ServerDetail();

    static const uint8 PacketID = 0x02;
    static const uint16 PacketLength = PACKET_HEADER_SIZE;
    
    //Packet struct
private:

};

}
}
}
#endif	//PacketServer_AuthServer_SERVERDETAIL_H

