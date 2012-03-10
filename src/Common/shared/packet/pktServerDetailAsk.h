#ifndef _SERVERDETAILASK_H_
#define	_SERVERDETAILASK_H_

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

class ServerDetailAsk: public PacketBase{
public:
    ServerDetailAsk();
    virtual ~ServerDetailAsk();

    static const uint8 PacketID = 0x02;
    static const uint16 PacketLength = PACKET_HEADER_SIZE;
    
    //Packet struct
private:
	virtual void ProcessPacket();

};

}
}
}
}
#endif	//_SERVERDETAILASK_H_

