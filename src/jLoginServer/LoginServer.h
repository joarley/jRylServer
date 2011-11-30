#ifndef JLOGINSERVER_H
#define	JLOGINSERVER_H

#include "shared/iStartupClass.h"
#include "shared/typedef.h"
#include "shared/stdDefs.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/cLogger.h"
#include "shared/cBuffer.h"

#include "cAuthServer.h"

#include <map>
#include <string>
#include <ctime>

using namespace std;

namespace jRylServer {
namespace jLoginServer {
using namespace common::shared;

class LoginServer : public StartupClass {
public:
    LoginServer(vector<string> params);
    virtual ~LoginServer();
    int Start();
    void Stop();
    bool LoadConfig();

    bool ConnectAuthServer(network::SocketSession_ptr socketSession);
    void ParsePacketUnauthorizedSession(network::SocketSession_ptr socketSession, Buffer_ptr buff);
    void CloseAuthServer(AuthServer* authServer);

protected:
    struct UnauthorizedSession {
        enum UnauthorizedSessionState {
            US_WAIT_PING,
            US_WAIT_USERPASS
        };
        UnauthorizedSessionState State;
        time_t Time;
        int32 AttemptLogin;
    };

    uint32 m_Version;
    string m_PatchAddress;

    string m_Address;
    string m_LauncherListenPort;
    string m_AuthServerListenPort;
    AuthServer* m_AuthServers[AUTHSERVER_SLOT_LENGTH];
    map<network::SocketSession_ptr, UnauthorizedSession> m_UnauthorizedSessions;

    string m_GlobalUser;
    string m_GlobalPass;
    Logger::LogFile_ptr m_LogFile;
};

}
}

#endif	/* JLOGINSERVER_H */