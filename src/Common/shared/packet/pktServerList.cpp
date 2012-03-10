#include "pktServerList.h"

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

ServerList::ServerList(Buffer_ptr buff): PacketBase(buff) {
	m_clientVersion = m_Buffer->Get<uint32>();
	m_patchAddress = m_Buffer->GetStringSizeFixed<string>(100);
	m_numServers = m_Buffer->Get<uint16>();
	for(int i = 0; i < m_numServers; i++) {
        m_authServers[i].Group = m_Buffer->Get<uint8>();
		m_authServers[i].Name = m_Buffer->GetStringSizeFixed<string>(15);
		m_authServers[i].ClientNum[0] = m_Buffer->Get<uint32>();
		m_authServers[i].ClientNum[1] = m_Buffer->Get<uint32>();
		m_authServers[i].Address = m_Buffer->Get<uint32>();
    }
}

ServerList::ServerList(): PacketBase(PacketID, PacketLength) {
	memset(m_authServers, 0, sizeof(m_authServers));
	m_clientVersion = 0;
    m_patchAddress = "";
    m_numServers = 0;
}

void ServerList::ProcessPacket() {
	m_Buffer->Add(m_clientVersion)
        .AddStringSizeFixed(m_patchAddress, 100)
        .Add(m_numServers);
    
	for(int i = 0; i < m_numServers; i++) {
        m_Buffer->Add(m_authServers[i].Group)
			.AddStringSizeFixed(m_authServers[i].Name, 15)
			.Add(m_authServers[i].ClientNum[0])
			.Add(m_authServers[i].ClientNum[1])
			.Add(m_authServers[i].Address);
    }
}

ServerList::~ServerList() { }

}
}
}
}