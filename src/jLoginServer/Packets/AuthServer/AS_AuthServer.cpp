#include "AS_AuthServer.h"

#include "shared/typedef.h"

#include <cstring>

namespace jRylServer {
namespace jLoginServer {
namespace PacketAuthServer {
using namespace common::shared;

AuthServer::AuthServer(Buffer_ptr buff, const char* globalUser, const char* globalPass): PacketBase(buff) {
    m_Slot = m_Buffer->Get<uint8>();
    char* pass = m_Buffer->GetString<char*>();
    char* user = m_Buffer->GetString<char*>();

    m_IsAuthenticated = (strcmp(user, globalUser) == 0 && strcmp(pass, globalPass) == 0);

    delete[] pass, user;
}

AuthServer::~AuthServer() {
    
}

}
}
}
