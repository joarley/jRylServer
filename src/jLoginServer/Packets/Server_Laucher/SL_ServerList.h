#ifndef PacketServer_Laucher_SERVERLIST_H
#define	PacketServer_Laucher_SERVERLIST_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"

namespace jRylServer {
namespace jLoginServer {
class LoginServer;
class AuthServer;
}
}

namespace jRylServer {
namespace jLoginServer {
namespace PacketServer_Laucher {

using namespace common::shared;

class ServerList: public PacketBase{
public:
	ServerList(uint32 clientVersion, char* patchAddress, AuthServer authServer[]);
    virtual ~ServerList();

    static const uint8 PacketID = 0x56;
    static const uint16 PacketLength = 398;

    //Packet struct
    //uint32 ClientVersion
    //char PatchAddress[100]
    //uint16 NumServers
    //begin loop 0 to 10
    //uint8 Group;
    //char Name[15];
    //int32 ClientNum[2]; //something chanel server set to 1
    //uint32 m_Address;
    //end loop
private:

};

}
}
}
#endif	/* PacketServer_Laucher_SERVERLIST_H */

