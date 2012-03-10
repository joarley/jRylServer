#include "pktPing.h"

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

Ping::Ping(Buffer_ptr buff): PacketBase(buff) {
    m_pingValue = m_Buffer->Get<uint32>();
}

Ping::Ping(): PacketBase(PacketID, PacketLength) { }

void Ping::ProcessPacket() {
	m_Buffer->Add(m_pingValue);
}

Ping::~Ping() { }

}
}
}
}
