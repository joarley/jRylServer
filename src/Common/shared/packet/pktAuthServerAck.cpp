#include "pktAuthServerAck.h"

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

AuthServerAck::AuthServerAck(): PacketBase(PacketID, PacketLength) { }

AuthServerAck::AuthServerAck(Buffer_ptr buff): PacketBase(buff) {
	m_slotNum = m_Buffer->Get<uint8>();
}

AuthServerAck::~AuthServerAck() {
    
}

void AuthServerAck::ProcessPacket() {
	m_Buffer->Add(m_slotNum);
}

}
}
}
}