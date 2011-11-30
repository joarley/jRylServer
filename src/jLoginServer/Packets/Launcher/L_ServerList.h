#ifndef PacketLauncher_SERVERLIST_H
#define	PacketLauncher_SERVERLIST_H

#include "shared/typedef.h"
#include "shared/cPacketBase.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketLauncher {

using namespace common::shared;

class ServerList: public PacketBase{
public:
    ServerList(Buffer_ptr buff);
    virtual ~ServerList();

    static const uint8 PacketID = 0x56;
    static const uint16 PacketLength = 16;

    //Packet struct
    //uint32 ClientVersion;
private:

};

} //PacketLauncher
} //jLoginServer
} //jRylServer

#endif	/* PacketLauncher_SERVERLIST_H */

