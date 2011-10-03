#include "C_Ping.h"

#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cBuffer.h"

#include "../../cAccount.h"
#include "../../cLoginServer.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketClient {

using namespace common::shared;

Ping::Ping(Buffer_ptr buff, Account* acc): PacketBase(buff) {
    if(!acc->IsLogged()) {
        if(acc->GetFistPing() > 0) {
            acc->Close();
            return;
        }

        uint32 ping = m_Buffer->Get<uint32>();

        acc->SetAuthenticated(false);
        acc->SetFistPing(ping);
        acc->SetLastPing(ping);
        LoginServer* server = acc->GetServer();
        acc->SetSessionId(server->GenSessionId());
        return;
    }
     acc->SetLastPing(m_Buffer->Get<uint32>());
}

Ping::~Ping() {
    
}

}
}
}
