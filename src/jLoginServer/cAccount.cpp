#include "cAccount.h"

namespace jRylServer {
namespace jLoginServer {

Account::Account() {
	m_Characters.resize(CHAR_SLOT_LENGTH);
    m_gameServer = NULL;
    m_socketSession.reset();
}

Account::~Account() {
    
}

uint32 Account::GetId() {
    return m_id;
}

Account::Nation Account::GetNation() {
    return m_nation;
}

uint32 Account::GetLastPing() {
    return m_lastPing;
}

uint32 Account::GetFistPing() {
    return m_fistPing;
}

bool Account::IsLogged() {
    if(m_socketSession.get() != NULL) {
        return m_socketSession->Connected();
    }
    else if(m_gameServer != NULL) {
        return m_gameServer->IsLoggedAccount(m_id);
    }
    return false;
}

network::SocketSession_ptr Account::GetSocketSession() {
    return m_socketSession;
}

LoginServer* Account::GetServer() {
    return m_server;
}


void Account::SetId(uint32 id) {
    m_id = id;
}

void Account::SetNation(Nation nation) {
    m_nation = nation;
}

void Account::SetLastPing(uint32 ping) {
    m_lastPing = ping;
}

void Account::SetFistPing(uint32 ping) {
    m_fistPing = ping;
}

void Account::SetSocketSession(network::SocketSession_ptr socketSession) {
    m_socketSession = socketSession;
}

void Account::SetServer(LoginServer* server) {
    m_server = server;
}


void Account::Close() {
    if(m_socketSession.get() != NULL) {
        if(m_socketSession->Connected()) {
            m_socketSession->Stop();
        }
    }
    else if(m_gameServer != NULL) {
        m_gameServer->CloseAccount(m_id);
    }
}

void Account::SetAuthenticated(bool authenticated) {
    m_authenticated = authenticated;
}

bool Account::isAuthenticated() {
     return m_authenticated;
}

void Account::SetSessionId(uint32 id) {
    m_serverId = id;
}

uint32 Account::GetSessionId() {
    return m_serverId;
}

 void Account::SetCharacter(int slot, Character* character) {
    if(m_Characters[slot] != NULL) {
        delete m_Characters[slot];
    }
    m_Characters[slot] = character;
 }

 Character* Account::GetCharacter(int slot) {
    return m_Characters[slot];
 }

}
}