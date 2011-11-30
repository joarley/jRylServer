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
}

AuthServer::~AuthServer() {
}

void AuthServer::PacketParser(network::SocketSession_ptr session, Buffer_ptr buff) {
    buff->SetReaderOffset(0);
    if(buff->Get<uint8>() != PACKET_START_BIT) {
        session->Stop();
        return;
    }
}

} //jLoginServer
} //jRylServer