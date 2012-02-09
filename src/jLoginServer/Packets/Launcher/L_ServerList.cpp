#include "L_ServerList.h"

#include "shared/cPacketBase.h"
#include "shared/typedef.h"

#include "../../LoginServer.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketLauncher {
using namespace common::shared;

ServerList::ServerList(Buffer_ptr buff, LoginServer* loginServer): PacketBase(buff) {
	loginServer->
}

ServerList::~ServerList() {
    
}

} //PacketLauncher
} //jLoginServer
} //jRylServer
