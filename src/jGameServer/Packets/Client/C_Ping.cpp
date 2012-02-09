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
    if(acc->GetFistPing() == 0) {
        uint32 ping = m_Buffer->Get<uint32>();
        acc->SetFistPing(ping);
        acc->SetLastPing(ping);
        LoginServer* server = acc->GetServer();
    } else {
        acc->SetLastPing(m_Buffer->Get<uint32>());
    }
}

Ping::~Ping() {
    
}

}
}
}