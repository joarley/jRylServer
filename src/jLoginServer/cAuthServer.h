#ifndef AUTHSERVER_H
#define AUTHSERVER_H

#include "shared/network/cSocketSession.h"
#include "shared/cBuffer.h"

#include <string>

using namespace std;

namespace jRylServer {
namespace jLoginServer {
class LoginServer;

class AuthServer {
public:
    AuthServer(common::shared::network::SocketSession_ptr session, LoginServer* loginServer);
    ~AuthServer();

    inline uint32 GetAdress();
    inline string GetName();
    inline uint8 GetGroup();
    inline uint32 GetLastPing();
private:
    uint8 m_Group;
    string m_Name;
    uint32 m_Adress;
    uint32 m_LastPing;
    common::shared::network::SocketSession_ptr m_SocketSession;
    LoginServer *m_LoginServer;
    
    void PacketParser(common::shared::network::SocketSession_ptr session, common::shared::Buffer_ptr buff);
};

inline uint32 AuthServer::GetAdress() {
    return m_Adress;
}

inline string AuthServer::GetName() {
    return m_Name;
}

inline uint8 AuthServer::GetGroup() {
    return m_Group;
}

inline uint32 AuthServer::GetLastPing() {
    return m_LastPing;
}

} //jLoginServer
} //jRylServer

#endif //AUTHSERVER_H