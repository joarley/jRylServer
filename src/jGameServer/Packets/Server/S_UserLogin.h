#ifndef PacketServer_USERLOGIN_H
#define	PacketServer_USERLOGIN_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cBuffer.h"

#include "../../cAccount.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketServer {

using namespace common::shared;

class UserLogin: public PacketBase{
public:
    UserLogin(Account* acc);
    virtual ~UserLogin();

    static const uint8 PacketID = 0x59;
    static const uint16 PacketLength = 488;
private:

};

}
}
}
#endif	/* AUTHACCOUNT_H */

