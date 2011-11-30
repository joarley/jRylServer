#ifndef PacketClient_AUTHACCOUNT_H
#define	PacketClient_AUTHACCOUNT_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "../../cAccount.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketClient {

using namespace common::shared;

class AuthAccount: public PacketBase{
public:
    AuthAccount(Buffer_ptr buff, Account* acc);
    virtual ~AuthAccount();

    //static const uint8 PacketID = 0x01;
    //static const uint16 PacketLength = 75;
    
    //char UserAccount[16];
    //char UserPassWord[33];
    //uint32 ClientVersion;
    //uint32 SessionID;
    //uint32 ClientCheckSum;
    //uint16 Flag;
private:

};

}
}
}
#endif	/* AUTHACCOUNT_H */

