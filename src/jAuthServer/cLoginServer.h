#ifndef _LOGINSERVER_H_
#define _LOGINSERVER_H_

#include "shared/typedef.h"
#include "shared/stdDefs.h"
#include "shared/cBuffer.h"
#include "shared/network/cSocketSession.h"

#include <string>
using namespace std;

namespace jRylServer {
namespace jAuthServer {
using namespace common::shared;

class LoginServer {
public:
	LoginServer();
	void SetConf(uint8 serverSlot, string serverName, uint8 serverGroup,
		string loginServerAddress, string loginServerPort,
		string globalUser, string globalPass);
	bool Start();
private:
	uint8 m_serverSlot;
	string m_serverName;
	uint8 m_serverGroup;
	string m_loginServerAddress;
	string m_loginServerPort;
	string m_globalUser;
	string m_globalPass;
	network::SocketSession_ptr m_socketSessionLogin;

	void ParsePacket(network::SocketSession_ptr socketSession, Buffer_ptr buff);
};

} //namespace jAuthServer
} //namespace jRylServer

#endif