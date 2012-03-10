#include "cAuthServerMgr.h"

#include "shared/stdDefs.h"
#include "shared/utils.h"
#include "shared/cLogger.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketMgr.h"

#include "shared/packet/pktPing.h"
#include "shared/packet/pktAuthServer.h"
#include "shared/packet/pktAuthServerAck.h"


namespace jRylServer {
namespace jLoginServer {

using namespace common::shared;

AuthServerMgr::AuthServerMgr() {
	memset(m_authServers, 0, sizeof(AuthServer));
}

void AuthServerMgr::SetConf(string address, string listenPort, string authUser, string authPass) {
	m_address = address;
	m_listenPort = listenPort;
	m_authUser = authUser;
	m_authPass = authPass;
}

AuthServerMgr::~AuthServerMgr() { }

bool AuthServerMgr::Start() {
	m_serverListen = network::SocketMgr::GetInstance().CreateServer("ListenAuthServer");
	m_serverListen->SetAcceptCallBack(boost::bind(&AuthServerMgr::ConnectAuthServer, this, _1));
	m_serverListen->SetCloseCallBack(boost::bind(&AuthServerMgr::CloseAuthServer, this, _1));
	if(!m_serverListen->Bind(m_address, m_listenPort)) {
		__LOG->ShowError("[cAuthServerMgr] Error bind address for listen AuthServers\n");
		return false;
    }
	m_serverListen->Start();
	return true;
}

bool AuthServerMgr::ConnectAuthServer(network::SocketSession_ptr socketSession) {
	socketSession->SetPacketProcessCallBack(boost::bind(&AuthServerMgr::ParsePacketUnauthorizedAuthServer, this, _1, _2));
	m_unauthorizedAuthServers[socketSession].State = UnauthorizedAuthServer::US_WAIT_PING;
    m_unauthorizedAuthServers[socketSession].Time = ::time(NULL);
    m_unauthorizedAuthServers[socketSession].AttemptLogin = 0;
    return true;
}

void AuthServerMgr::CloseAuthServer(network::SocketSession_ptr socketSession) {
	if(m_unauthorizedAuthServers.erase(socketSession) < 1) {
		for(int i = 0; i < AUTHSERVER_SLOT_LENGTH; i++) {
			if(m_authServers[i] != NULL && m_authServers[i]->GetSocketSession() == socketSession) {
				__LOG->ShowInfo("Desconnected AuthServer(%s) with ip address %s\n", m_authServers[i]->GetName().c_str(), IpToStr(m_authServers[i]->GetAddress()));
				delete m_authServers[i];
		        m_authServers[i] = NULL;
		        break;
		    }
		}
	}
}
	
void AuthServerMgr::ParsePacketUnauthorizedAuthServer(network::SocketSession_ptr socketSession, Buffer_ptr buff) {
	buff->SetReaderOffset(0);
    
	switch (m_unauthorizedAuthServers[socketSession].State) {
	case UnauthorizedAuthServer::US_WAIT_PING: {
		if(buff->Get<uint8>(1) != packet::Ping::PacketID) {
			m_unauthorizedAuthServers.erase(socketSession);
		    socketSession->Stop();
		    return;
		}
		m_unauthorizedAuthServers[socketSession].State =  UnauthorizedAuthServer::US_WAIT_USERPASS;
		m_unauthorizedAuthServers[socketSession].Time = ::time(NULL);
		break;
    }
	case UnauthorizedAuthServer::US_WAIT_USERPASS: {
		if(buff->Get<uint8>(1) != packet::AuthServer::PacketID) {
			m_unauthorizedAuthServers.erase(socketSession);
		    socketSession->Stop();
		    return;
		}
		packet::AuthServer authServer(buff);
		packet::AuthServerAck authServerAck;
		
		if(authServer.GetUser() == m_authUser && authServer.GetPass() == m_authPass) {
			uint8 slotnum = authServer.GetSlotNum();
		    if(m_authServers[slotnum] != NULL) {
		        bool slotfound = false;
		        for(slotnum++;slotnum < AUTHSERVER_SLOT_LENGTH;slotnum++) {
		            if(m_authServers[slotnum] == NULL) {
		                slotfound = true;
		                break;
		            }
		        }
		        if(!slotfound) {
		            for(slotnum = authServer.GetSlotNum();slotnum != 0xFF;slotnum--) {
		                if(m_authServers[slotnum] == NULL) {
		                    slotfound = true;
		                    break;
		                }
		            }
		        }
		        if(!slotfound) {
					m_unauthorizedAuthServers.erase(socketSession);
					authServerAck.SetErro(packet::AuthServerAck::EA_SERVER_FULL);
		            socketSession->SendPacket(authServerAck);
		            socketSession->Stop();
		            return;
		        }
		        
		    }

			m_unauthorizedAuthServers.erase(socketSession);
			authServerAck.SetErro(packet::AuthServerAck::EA_SUCESS);
			authServerAck.SetSlotNum(slotnum);
			socketSession->SendPacket(authServerAck);
			m_authServers[slotnum] = new AuthServer(socketSession);
		} else {
			packet::AuthServerAck authServerAck;
			m_unauthorizedAuthServers.erase(socketSession);
			if(m_unauthorizedAuthServers[socketSession].AttemptLogin == MAX_AUTHSERVER_LOGIN_ATTEMPTS) {                    
				authServerAck.SetErro(packet::AuthServerAck::EA_MAX_LOGIN_ATTEMPTS);					
		        socketSession->SendPacket(authServerAck);
		        socketSession->Stop();
		    } else {
				authServerAck.SetErro(packet::AuthServerAck::EA_USER_PASS_INCORRECT);
		        socketSession->SendPacket(authServerAck);
				m_unauthorizedAuthServers[socketSession].AttemptLogin++;
		    }
		}
		break;
    }
    default:
		m_unauthorizedAuthServers.erase(socketSession);
        socketSession->Stop();
        break;
    }
}

} //jLoginServer
} //jRylServer