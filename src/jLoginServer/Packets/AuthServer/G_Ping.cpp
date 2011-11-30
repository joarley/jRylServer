#include "G_Ping.h"

#include "shared/typedef.h"


namespace jRylServer {
namespace jLoginServer {
namespace PacketAuthServer {
using namespace common::shared;

Ping::Ping(Buffer_ptr buff): PacketBase(buff) {
    
}

Ping::~Ping() {
    
}

}
}
}
