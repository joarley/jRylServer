#include "S_AuthAccount.h"
#include <string.h>

#include "../../cLoginServer.h"
#include  "../../cAccount.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketServer {

AuthAccount::AuthAccount(Account* acc) : PacketBase(PacketID, PacketLength) {
    m_Status = acc->GetId();
    m_Buffer->Add((uint32)acc->GetServer()->GetId())
             .Add((uint8)acc->GetNation());
}

AuthAccount::AuthAccount(AuthError errorCode) : PacketBase(PacketID, PacketLength) {
    m_Status = 0;
    m_Buffer->Add((uint32)errorCode)
             .Add((uint8)0);
}

}
}
}