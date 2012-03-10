#ifndef _AUTHSERVERACK_H_
#define	_AUTHSERVERACK_H_

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

class AuthServerAck: public PacketBase{
public:
    enum ErroAuth {
        EA_SUCESS,
        EA_SERVER_FULL,
        EA_USER_PASS_INCORRECT,
        EA_MAX_LOGIN_ATTEMPTS
    };

	static const uint8 PacketID = 0x01;
    static const uint16 PacketLength = 13;

    AuthServerAck(Buffer_ptr buff);
	AuthServerAck();
    virtual ~AuthServerAck();
    
	inline uint8 GetSlotNum();
	inline ErroAuth GetErro();

	inline void SetSlotNum(uint8 slotNum);
	inline void SetErro(ErroAuth erroAuth);

    //Packet struct (SUCCESS[error == EA_SUCESS])
    //uint8 SlotNum
    
    //Packet struct (ERROR[error != EA_SUCESS])
    //m_Status = errorCode
private:
	virtual void ProcessPacket();

	uint8 m_slotNum;
};

inline uint8 AuthServerAck::GetSlotNum() {
	return m_slotNum;
}

inline AuthServerAck::ErroAuth AuthServerAck::GetErro() {
	return (ErroAuth)m_Status;
}

inline void AuthServerAck::SetSlotNum(uint8 slotNum) {
	m_slotNum = slotNum;
}

inline void AuthServerAck::SetErro(ErroAuth erroAuth) {
	m_Status = erroAuth;
}

}
}
}
}
#endif	//_AUTHSERVERACK_H_

