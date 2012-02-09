#include "SAS_AuthServer.h"

#include "shared/typedef.h"


namespace jRylServer {
namespace jLoginServer {
namespace PacketServer_AuthServer {
using namespace common::shared;

AuthServer::AuthServer(uint8 slotNum, ErrorAuth error): PacketBase(PacketID, PacketLength) {
    if(error == EA_SUCESS) {
        m_Buffer->Add(slotNum);
    } else {
        m_Status = static_cast<uint32>(error);
    }
}

AuthServer::~AuthServer() {
    
}

}
}
}
