#ifndef PacketAuthServer_AUTHSERVER_H
#define	PacketAuthServer_AUTHSERVER_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketAuthServer {

using namespace common::shared;

class AuthServer: public PacketBase{
public:
    AuthServer(Buffer_ptr buff, const char* globalUser, const char* globalPass);
    virtual ~AuthServer();

    inline bool IsAuthenticated();
    inline uint8 GetSlot();

    static const uint8 PacketID = 0x01;
    static const uint16 PacketLength = 0;

    //Packet struct
    //uint8 SlotNum;
    //char* User;
    //char* Pass;
private:
    bool m_IsAuthenticated;
    uint8 m_Slot;
};

inline bool AuthServer::IsAuthenticated() {
    return m_IsAuthenticated;
}

inline uint8 AuthServer::GetSlot() {
    return m_Slot;
}

}
}
}
#endif	/* PacketAuthServer_AUTHSERVER_H */

