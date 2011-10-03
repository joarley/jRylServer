#ifndef PacketClient_USERLOGIN_H
#define	PacketClient_USERLOGIN_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cBuffer.h"

#include "../../cAccount.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketClient {

using namespace common::shared;

class UserLogin: public PacketBase{
public:
    UserLogin(Buffer_ptr buff, Account* acc);
    virtual ~UserLogin();

    static const uint8 PacketID = 0x01;
    static const uint16 PacketLength = 21;
private:

};

}
}
}
#endif	/* AUTHACCOUNT_H */
