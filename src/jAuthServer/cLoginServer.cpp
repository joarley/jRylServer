#include "cLoginServer.h"

#include "shared/typedef.h"
#include "shared/stdDefs.h"
#include "shared/cBuffer.h"
#include "shared/network/cSocketMgr.h"
#include "shared/network/cSocketSession.h"

#include "shared/packet/pktPing.h"
#include "shared/packet/pktAuthServer.h"
#include "shared/packet/pktAuthServerAck.h"
#include "shared/packet/pktServerDetailAsk.h"
#include "shared/packet/pktServerDetail.h"

#include <string>
using namespace std;

namespace jRylServer {
namespace jAuthServer {
using namespace common::shared;

LoginServer::LoginServer() {}

void LoginServer::SetConf(uint8 serverSlot, string serverName, uint8 serverGroup,
string loginServerAddress, string loginServerPort,
string globalUser, string globalPass) {
	m_serverSlot = serverSlot;
	m_serverName = serverName;
	m_serverGroup = serverGroup;
	m_loginServerAddress = loginServerAddress;
	m_loginServerPort = loginServerPort;
	m_globalUser = globalUser;
	m_globalPass = globalPass;
}

bool LoginServer::Start() {
	m_socketSessionLogin.reset(new network::SocketSession(*network::SocketMgr::GetInstance().GetService()));
	__LOG->ShowInfo("connecting to LoginServer\n");
	m_socketSessionLogin->ConnectServer(m_loginServerAddress, m_loginServerPort);
	m_socketSessionLogin->SetPacketProcessCallBack(boost::bind(&LoginServer::ParsePacket, this, _1, _2));
	m_socketSessionLogin->Start();

	packet::Ping ping;
	ping.SetPingValue(0);
	m_socketSessionLogin->SendPacket(ping);

	packet::AuthServer authServer;
	authServer.SetPass(m_globalPass);
	authServer.SetUser(m_globalUser);
	authServer.SetSlotNum(m_serverSlot);
	m_socketSessionLogin->SendPacket(authServer);
	return true;
}

void LoginServer::ParsePacket(network::SocketSession_ptr socketSession, Buffer_ptr buff) {
	buff->SetReaderOffset(0);
    
	switch(buff->Get<uint8>(1)) {
	case packet::AuthServerAck::PacketID: {
		packet::AuthServerAck authServerAck(buff);
		switch(authServerAck.GetErro()) {
		case packet::AuthServerAck::EA_SUCESS:
			if(authServerAck.GetSlotNum() != m_serverSlot) {
				m_serverSlot = authServerAck.GetSlotNum();
				__LOG->ShowWarning("Slot server changed to %d\n", m_serverSlot);
			}
			__LOG->ShowInfo("connected to LoginServer successfully\n");
			break;
		case packet::AuthServerAck::EA_SERVER_FULL:
			break;
		}
	}
	case packet::ServerDetailAsk::PacketID: {
		packet::ServerDetail serverDetail;
		serverDetail.SetAddress(m_socketSessionLogin->Socket().local_endpoint().address().to_v4().to_ulong());
		serverDetail.SetGroup(m_serverGroup);
		serverDetail.SetName(m_serverName);
		socketSession->SendPacket(serverDetail);
		break;
	}
	}
}


} //namespace jAuthServer
} //namespace jRylServer
