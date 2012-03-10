#ifndef AUTHSERVER_H
#define AUTHSERVER_H

#include "shared/network/cSocketSession.h"
#include "shared/cBuffer.h"

#include <string>

using namespace std;

namespace jRylServer {
namespace jLoginServer {
using namespace common::shared;

class AuthServer {
public:
	enum AuthServerStatus {
		ASS_OK,
		ASS_WAIT_DETAILS
	};
	AuthServer(network::SocketSession_ptr session);
	~AuthServer();
	void PacketParser(common::shared::network::SocketSession_ptr session, common::shared::Buffer_ptr buff);

	inline network::SocketSession_ptr GetSocketSession();
	inline uint8 GetGroup();
    inline string GetName();
    inline uint32 GetAddress();
    inline uint32 GetLastPing();
	inline AuthServerStatus GetStatus();
private:
	uint8 m_group;
    string m_name;
    uint32 m_address;
    uint32 m_lastPing;
    common::shared::network::SocketSession_ptr m_socketSession;
	AuthServerStatus m_status;
};

inline network::SocketSession_ptr AuthServer::GetSocketSession() {
	return m_socketSession;
}

inline uint8 AuthServer::GetGroup() {
	return m_group;
}

inline string AuthServer::GetName() {
	return m_name;
}

inline uint32 AuthServer::GetAddress() {
	return m_address;
}

inline uint32 AuthServer::GetLastPing() {
	return m_lastPing;
}

inline AuthServer::AuthServerStatus AuthServer::GetStatus() {
	return m_status;
}


} //jLoginServer
} //jRylServer

#endif //AUTHSERVER_H