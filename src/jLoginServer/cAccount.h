#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "shared/typedef.h"
#include "shared/stdDefs.h"
#include "shared/network/cSocketSession.h"

#include "cCharacter.h"
#include "cGameServer.h"

#include <vector>

using namespace std;

namespace jRylServer {
namespace jLoginServer {
using namespace common::shared;

class LoginServer;

class Account {
public:
    enum Nation {
        NT_NotChosen,
		NT_Kartefant,
		NT_Merkhadian
    };

    Account();
    ~Account();

    uint32 GetId();
    Nation GetNation();
    uint32 GetLastPing();
    uint32 GetFistPing();
    bool IsLogged();
    network::SocketSession_ptr GetSocketSession();
    LoginServer* GetServer();
    bool isAuthenticated();
    uint32 GetSessionId();
    Character* GetCharacter(int slot);

    void SetId(uint32 id);
    void SetNation(Nation nation);
    void SetLastPing(uint32);
    void SetFistPing(uint32);
    void SetSocketSession(network::SocketSession_ptr socketSession);
    void SetServer(LoginServer* server);
    void SetAuthenticated(bool authenticated);
    void SetSessionId(uint32 id);
    void SetCharacter(int slot, Character* character);

    void Close();
protected:
    uint32 m_id;
    Nation m_nation;
    uint32 m_lastPing;
    uint32 m_fistPing;
    uint32 m_serverId;
    uint32 m_sessionId;
    bool m_authenticated;
    network::SocketSession_ptr m_socketSession;
    GameServer* m_gameServer;
    LoginServer* m_server;
    vector<Character*> m_Characters;
};

}
}
#endif
