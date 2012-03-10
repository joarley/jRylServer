#ifndef _AUTHACCOUNTACK_H_
#define	_AUTHACCOUNTACK_H_

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

class AuthAccountAck: public PacketBase {
public:
    enum AuthErro {
        AE_ChecksumError = 3,
        AE_VersionError,
        AE_WrongLogin,
        AE_ServerFull = 7,
        AE_AccountBanned,
        AE_AlreadyLogged = 41,
        AE_DBError = 1,
    };

    static const uint8 PacketID = 0x01;
    static const uint16 PacketLength = 18;

	AuthAccountAck(Buffer_ptr buff);
    AuthAccountAck();

    //uint32 UserId;
    //uint8 AgentServerType;
    //uint8 FirstLogin;

	inline uint32 GetUserId();
    inline uint8 GetAgentServerType();
    inline uint8 GetFirstLogin();
	inline AuthErro GetAuthErro();

	inline void SetUserId(uint32 userId);
    inline void SetAgentServerType(uint8 agentServerType);
    inline void SetFirstLogin(uint8 firstLogin);
	inline void SetAuthErro(AuthErro authErro);

private:
	virtual void ProcessPacket();
	
	uint32 m_userId;
    uint8 m_agentServerType;
    uint8 m_firstLogin;
};

inline uint32 AuthAccountAck::GetUserId() {
	return m_userId;
}

inline uint8 AuthAccountAck::GetAgentServerType() {
	return m_agentServerType;
}

inline uint8 AuthAccountAck::GetFirstLogin() {
	return m_firstLogin;
}

inline AuthAccountAck::AuthErro AuthAccountAck::GetAuthErro() {
	return (AuthErro)m_Status;
}

inline void AuthAccountAck::SetUserId(uint32 userId) {
	m_userId = userId;
}

inline void AuthAccountAck::SetAgentServerType(uint8 agentServerType) {
	m_agentServerType = agentServerType;
}

inline void AuthAccountAck::SetFirstLogin(uint8 firstLogin) {
	m_firstLogin = firstLogin;
}

inline void AuthAccountAck::SetAuthErro(AuthErro authErro) {
	m_Status = authErro;
}

}
}
}
}

#endif	/* AUTHACCOUNTRET_H_ */