#include "pktAuthServer.h"

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"


namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

AuthServer::AuthServer(Buffer_ptr buff): PacketBase(buff) {
	m_slotNum = m_Buffer->Get<uint8>();
	m_user = m_Buffer->GetString<string>();
	m_pass = m_Buffer->GetString<string>();
}

AuthServer::AuthServer(): PacketBase(PacketID) { }

AuthServer::~AuthServer() { }

void AuthServer::ProcessPacket() {
	m_Buffer->Add(m_slotNum);
	m_Buffer->AddString(m_user);
	m_Buffer->AddString(m_pass);
}

}
}
}
}