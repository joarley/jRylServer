#include "AS_Ping.h"

#include "shared/typedef.h"


namespace jRylServer {
namespace jLoginServer {
namespace PacketAuthServer {
using namespace common::shared;

Ping::Ping(Buffer_ptr buff): PacketBase(buff) {
    m_PingValue = m_Buffer->Get<uint32>();
}

Ping::~Ping() {
    
}

}
}
}
