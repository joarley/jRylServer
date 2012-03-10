#include "pktUserLoginAck.h"

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

UserLoginAck::UserLoginAck(Buffer_ptr buff): PacketBase(buff) {

}

UserLoginAck::UserLoginAck(): PacketBase(PacketID, PacketLength) {

}

UserLoginAck::~UserLoginAck() {
    
}

void UserLoginAck::ProcessPacket() {

}

}
}
}
}