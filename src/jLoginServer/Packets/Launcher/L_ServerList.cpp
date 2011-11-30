#include "L_ServerList.h"

#include "shared/cPacketBase.h"
#include "shared/typedef.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketLauncher {
using namespace common::shared;

ServerList::ServerList(Buffer_ptr buff): PacketBase(buff) {
    uint32 clientVersion = m_Buffer->Get<uint32>();
    //PacketServer::ServerList serverList(server);
    //session->SendPacket(ServerList.GetProcessedBuffer());
}

ServerList::~ServerList() {
    
}

} //PacketLauncher
} //jLoginServer
} //jRylServer
