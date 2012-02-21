#include "SL_ServerList.h"
#include "../../cAuthServer.h"

#include "shared/typedef.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketServer_Laucher {
using namespace common::shared;

ServerList::ServerList(uint32 clientVersion, char* patchAddress, AuthServer authServers[]): PacketBase(PacketID, PacketLength) {
	m_Buffer->Add(clientVersion)
        .AddStringSizeFixed(patchAddress, 100);
    size_t numServersPos = m_Buffer->GetWriteOffset();
    uint16 numServers = 0;
    m_Buffer->Add((uint16)0);
    
    for(int i = 0; i < AUTHSERVER_SLOT_LENGTH; i++) {
        if(authServers[i] != NULL) {
            m_Buffer->Add(authServers[i]->GetGroup())
                .AddStringSizeFixed(authServers[i]->GetName(), 15)
                .
        }
    }
}

ServerList::~ServerList() {
    
}

}
}
}
