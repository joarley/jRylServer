#include "pktServerDetail.h"

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

ServerDetail::ServerDetail(Buffer_ptr buff): PacketBase(buff) {
    m_group = m_Buffer->Get<uint8>();
    m_name = m_Buffer->GetString<string>();
    m_address = m_Buffer->Get<uint32>();
}

void ServerDetail::ProcessPacket() {
	m_Buffer->Add(m_group);
	m_Buffer->AddString(m_name);
	m_Buffer->Add(m_address);
}

ServerDetail::ServerDetail(): PacketBase(PacketID) { }
ServerDetail::~ServerDetail() { }

}
}
}
}