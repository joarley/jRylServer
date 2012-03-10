#include "cAuthServer.h"

#include "shared/network/cSocketSession.h"
#include "shared/cBuffer.h"
#include "shared/stdDefs.h"
#include "shared/cLogger.h"
#include "shared/utils.h"

#include "shared/packet/pktPing.h"
#include "shared/packet/pktServerDetail.h"
#include "shared/packet/pktServerDetailAsk.h"

#include <boost/bind.hpp>

#include <assert.h>

namespace jRylServer {
namespace jLoginServer {

using namespace common::shared;

AuthServer::AuthServer(network::SocketSession_ptr session) {
    m_socketSession = session;
    m_socketSession->SetPacketProcessCallBack(boost::bind(&AuthServer::PacketParser, this, _1, _2));
	m_socketSession->SendPacket(packet::ServerDetailAsk());
    m_status = ASS_WAIT_DETAILS;
}

AuthServer::~AuthServer() { }

void AuthServer::PacketParser(network::SocketSession_ptr session, Buffer_ptr buff) {
	buff->SetReaderOffset(0);
	switch(buff->Get<uint8>(1)) {
	case packet::Ping::PacketID:
        m_lastPing = packet::Ping(buff).GetPingValue();
		break;
	case packet::ServerDetail::PacketID:
		if(m_status == ASS_WAIT_DETAILS) {
            packet::ServerDetail serverDetail(buff);
            m_group = serverDetail.GetGroup();
            m_name = serverDetail.GetName();
			m_address = serverDetail.GetAddress();
			m_status = ASS_OK;
			__LOG->ShowInfo("Connected AuthServer(%s) with ip address %s\n", m_name.c_str(), IpToStr(m_address));
        }
		break;
    }
}

} //jLoginServer
} //jRylServer