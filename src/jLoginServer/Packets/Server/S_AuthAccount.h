#ifndef PacketServer_AUTHACCOUNT_H
#define	PacketServer_AUTHACCOUNT_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"

#include "../../cAccount.h"

namespace jRylServer {
namespace jLoginServer {
namespace PacketServer {
using namespace common::shared;

class AuthAccount: public PacketBase {
public:
    enum AuthError {
        AE_ChecksumError = 3,
        AE_VersionError,
        AE_ServerFull = 7,
        AE_AccountBanned,
        AE_AlreadyLogged = 41,
        AE_DBError = 1,
        AE_WrongLogin = 5
    };

    static const uint8 PacketID = 0x01;
    static const uint16 PacketLength = 18;

    uint32 UserId;
    uint8 AgentServerType;
    uint8 FirstLogin;

    AuthAccount(Account* acc);
    AuthAccount(AuthError errorCode);
};

}
}
}

#endif	/* AUTHACCOUNT_H */

