#ifndef PacketServer_AuthServer_AUTHSERVER_H
#define	PacketServer_AuthServer_AUTHSERVER_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketServer_AuthServer {

using namespace common::shared;

class AuthServer: public PacketBase{
public:
    enum ErrorAuth {
        EA_SUCESS,
        EA_SERVER_FULL,
        EA_USER_PASS_INCORRECT,
        EA_MAX_LOGIN_ATTEMPTS
    };
    AuthServer(uint8 slotNum, ErrorAuth error = EA_SUCESS);
    virtual ~AuthServer();

    static const uint8 PacketID = 0x01;
    static const uint16 PacketLength = 13;
    
    //Packet struct (SUCCESS[error == EA_SUCESS])
    //uint8 SlotNum
    
    //Packet struct (ERROR[error != EA_SUCESS])
    //m_Status = errorCode
private:

};

}
}
}
#endif	//PacketServer_AuthServer_AUTHSERVER_H

