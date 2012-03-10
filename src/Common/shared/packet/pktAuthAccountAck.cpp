#include "pktAuthAccountAck.h"

#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

AuthAccountAck::AuthAccountAck() : PacketBase(PacketID, PacketLength) { }

AuthAccountAck::AuthAccountAck(Buffer_ptr buff) : PacketBase(PacketID, PacketLength) {
    m_userId = m_Buffer->Get<uint32>();
    m_agentServerType = m_Buffer->Get<uint8>();
	m_firstLogin = m_Buffer->Get<uint8>();
}

void AuthAccountAck::ProcessPacket() {
	m_Buffer->Add(m_userId);
    m_Buffer->Add(m_agentServerType);
	m_Buffer->Add(m_firstLogin);
}

}
}
}
}