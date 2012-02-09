#include "cAuthServer.h"

#include "LoginServer.h"
#include "Packets/packets.h"

#include "shared/network/cSocketSession.h"
#include "shared/cBuffer.h"
#include "shared/stdDefs.h"
#include "shared/cLogger.h"

#include <boost/bind.hpp>

#include <assert.h>

namespace jRylServer {
namespace jLoginServer {

using namespace common::shared;

AuthServer::AuthServer(network::SocketSession_ptr session, LoginServer* loginServer) {
    assert(loginServer != NULL);
    m_LoginServer = loginServer;
    m_SocketSession = session;
    m_SocketSession->SetPacketProcessCallBack(boost::bind(&AuthServer::PacketParser, this, _1, _2));
    m_SocketSession->SendPacket(PacketServer_AuthServer::ServerDetail().GetProcessedBuffer());
    m_Status = ASS_WAIT_DETAILS;
}

AuthServer::~AuthServer() {
    m_SocketSession->Stop();
}

void AuthServer::PacketParser(network::SocketSession_ptr session, Buffer_ptr buff) {
    if(buff->Get<uint8>(0) != PACKET_START_BIT) {
        session->Stop();
        m_LoginServer->CloseAuthServer(this);
        return;
    }

    
	switch(buff->Get<uint8>(1)) {

	case PacketAuthServer::Ping::PacketID:
        m_LastPing = PacketAuthServer::Ping(buff).GetPingValue();
		break;
	case PacketAuthServer::ServerDetail::PacketID:
		if(m_Status == ASS_WAIT_DETAILS) {
            PacketAuthServer::ServerDetail serverDetail(buff);
            m_Group = serverDetail.GetGroup();
            m_Name = serverDetail.GetName();
            m_Adress = serverDetail.GetAdress();
        }
		break;
    }
}

} //jLoginServer
} //jRylServer