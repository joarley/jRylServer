#include "pktServerListAsk.h"

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

ServerListAsk::ServerListAsk(Buffer_ptr buff): PacketBase(buff) {
	m_clientVersion = m_Buffer->Get<uint32>();
}

ServerListAsk::ServerListAsk(): PacketBase(PacketID, PacketLength) { }

void ServerListAsk::ProcessPacket() {
	m_Buffer->Add(m_clientVersion);
}

ServerListAsk::~ServerListAsk() { }

}
}
}
}
