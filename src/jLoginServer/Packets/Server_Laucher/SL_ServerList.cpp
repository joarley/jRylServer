#include "SL_ServerList.h"
#include "../../cAuthServer.h"

#include "shared/typedef.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketServer_Laucher {
using namespace common::shared;

ServerList::ServerList(AuthServer servers[]): PacketBase(PacketID, PacketLength) {
	
}

ServerList::~ServerList() {
    
}

}
}
}
