#include "SL_ServerList.h"

#include "shared/typedef.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketServer_Laucher {
using namespace common::shared;

ServerList::ServerList(LoginServer* server): PacketBase(PacketID, PacketLength) {
    
}

ServerList::~ServerList() {
    
}

}
}
}
