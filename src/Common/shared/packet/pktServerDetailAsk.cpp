#include "pktServerDetailAsk.h"

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

ServerDetailAsk::ServerDetailAsk(): PacketBase(PacketID, PacketLength) { }

ServerDetailAsk::~ServerDetailAsk() { }

void ServerDetailAsk::ProcessPacket() { }

}
}
}
}
