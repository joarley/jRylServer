#ifndef _USERLOGIN_H_
#define	_USERLOGIN_H_

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

class UserLogin: public PacketBase{
public:
	enum LoginType {
		LT_FirstLogin,
		LT_CharSelect
	};

	static const uint8 PacketID = 0x03;
    static const uint16 PacketLength = 21;

    UserLogin(Buffer_ptr buff);
	UserLogin();
    virtual ~UserLogin();

	inline uint32 GetUserId();
    inline uint32 GetCheckSum();
    inline LoginType GetLoginType();

	inline void SetUserId(uint32 userId);
	inline void SetCheckSum(uint32 checkSum);
	inline void SetLoginType(LoginType loginType);
    
	//uint32 UserId;
    //uint32 CheckSum
    //int8 LoginType;
private:
	virtual void ProcessPacket();

	uint32 m_userId;
	uint32 m_checkSum;
	int8 m_loginType;
};

inline uint32 UserLogin::GetUserId() {
	return m_userId;
}

inline uint32 UserLogin::GetCheckSum() {
	return m_checkSum;
}

inline UserLogin::LoginType UserLogin::GetLoginType() {
	return (LoginType)m_loginType;
}

inline void UserLogin::SetUserId(uint32 userId) {
	m_userId = userId;
}

inline void UserLogin::SetCheckSum(uint32 checkSum) {
	m_checkSum = checkSum;
}

inline void UserLogin::SetLoginType(LoginType loginType) {
	m_loginType = loginType;
}

}
}
}
}
#endif	/* _USERLOGIN_H_ */

