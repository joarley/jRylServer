#include "S_AuthAccount.h"
#include <string.h>

#include "../../cLoginServer.h"
#include  "../../cAccount.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketServer {

AuthAccount::AuthAccount(Account* acc) : PacketBase(PacketID, PacketLength) {
    m_Status = 0;
    m_Buffer->Add(acc->GetId())
        .Add((uint8)22)
        .Add((uint8)0);
}

AuthAccount::AuthAccount(AuthError errorCode) : PacketBase(PacketID, PacketLength) {
    m_Status = errorCode;
    m_Buffer->Add((uint32)0)
             .Add((uint16)0);
}

}
}
}