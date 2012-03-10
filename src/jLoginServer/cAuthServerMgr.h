#ifndef AUTHSERVERMGR_H
#define AUTHSERVERMGR_H

#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/cBuffer.h"

#include "cAuthServer.h"

#include <string>

using namespace std;

namespace jRylServer {
namespace jLoginServer {
using namespace common::shared;

class AuthServerMgr {
public:
    AuthServerMgr();
    ~AuthServerMgr();
	void SetConf(string address, string listenPort, string authUser, string authPass);
	bool Start();

	bool ConnectAuthServer(network::SocketSession_ptr socketSession);
	void CloseAuthServer(network::SocketSession_ptr socketSession);
	
	void ParsePacketUnauthorizedAuthServer(network::SocketSession_ptr socketSession, Buffer_ptr buff);

	inline AuthServer** GetAuthServerList();
private:
	struct UnauthorizedAuthServer {
        enum UnauthorizedAuthServerState {
            US_WAIT_PING,
            US_WAIT_USERPASS
        };
        UnauthorizedAuthServerState State;
        time_t Time;
        int32 AttemptLogin;
    };
	string m_address;
	string m_listenPort;
	string m_authUser;
	string m_authPass;

	network::SocketServer* m_serverListen;
	map<network::SocketSession_ptr, UnauthorizedAuthServer> m_unauthorizedAuthServers;
    AuthServer* m_authServers[AUTHSERVER_SLOT_LENGTH];
};

inline AuthServer** AuthServerMgr::GetAuthServerList() {
	return m_authServers;
}

} //jLoginServer
} //jRylServer

#endif //AUTHSERVERMGR_H