#ifndef JLOGINSERVER_H
#define	JLOGINSERVER_H
#include <boost/noncopyable.hpp>

#include "shared/iStartupClass.h"
#include "shared/typedef.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/network/cSocketMgr.h"
#include "shared/cLogger.h"

#include "cAccount.h"
#include "cCharacter.h"
#include "cGameServer.h"

#include <map>
#include <string>

using namespace std;

namespace jRylServer {
namespace jLoginServer {
using namespace common::shared;

class LoginServer :public StartupClass {
public:
    LoginServer(vector<string> params);
    bool LoadConfig();
    virtual ~LoginServer();
    int Start();
    void Stop();
    
    uint32 GetId();
    uint32 GetVersion();
    uint32 GetChecksum();
    char* GetPatchAddress();
    uint32 GetMaxUsers();
    uint32 GetCurrentUsersCount();
    Account* GetAccountById(uint32 id);
    void PacketProcess(network::SocketSession_ptr session, Buffer_ptr buff);
    bool ConnectClient(network::SocketSession_ptr session);
    
    bool LoginAccount(Account* acc);
    uint32 GenSessionId();
protected:
    uint32 m_id;
    char* m_globalUser;
    char* m_globalPass;
    char* m_listenAddress;
    char* m_listenClientPort;
    char* m_listenLauncherPort;
    char* m_listenGameServerPort;
    uint32 m_clientVersion;
    uint32 m_clientChecksum;
    char* m_patchAddress;
    uint32 m_maxUsers;
    uint32 m_userCount;

    Logger::LogFile_ptr m_logFile;
    
    network::SocketServer* m_listenClient;
    network::SocketServer* m_listenLauncher;
    network::SocketServer* m_listenGameServer;

    map<network::SocketSession_ptr, Account*> m_loginServerAccounts;
    map<uint32, Account*> m_gameServerAccounts;
    map<network::SocketSession_ptr, GameServer*> m_gameServers;
};

}
}

#endif	/* JLOGINSERVER_H */