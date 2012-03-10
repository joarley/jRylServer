#include "pktUserLogin.h"

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

UserLogin::UserLogin(Buffer_ptr buff): PacketBase(buff) {
	m_userId = m_Buffer->Get<uint32>();
	m_checkSum = m_Buffer->Get<uint32>();
	m_loginType =  m_Buffer->Get<uint8>();
}

UserLogin::UserLogin(): PacketBase(PacketID, PacketLength) {
    m_userId = 0;
	m_checkSum = 0;
}

void UserLogin::ProcessPacket() {
	m_Buffer->Add(m_userId);
	m_Buffer->Add(m_checkSum);
	m_Buffer->Add(m_loginType);
}

UserLogin::~UserLogin() { }

}
}
}
}