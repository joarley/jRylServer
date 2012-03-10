#ifndef _AUTHACCOUNT_H_
#define	_AUTHACCOUNT_H_

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

class AuthAccount: public PacketBase{
public:
    static const uint8 PacketID = 0x01;
    static const uint16 PacketLength = 75;

	AuthAccount(Buffer_ptr buff);
	AuthAccount();
    virtual ~AuthAccount();

	inline string GetUserAccount();
    inline string GetUserPassWord();
    inline uint32 GetClientVersion();
    inline uint32 GetSessionID();
    inline uint32 GetClientCheckSum();
    inline uint16 GetFlag();

	inline void SetUserAccount(string userAccount);
    inline void SetUserPassWord(string userPassWord);
    inline void SetClientVersion(uint32 clientVersion);
    inline void SetSessionID(uint32 sessionID);
    inline void SetClientCheckSum(uint32 clientCheckSum);
    inline void SetFlag(uint16 flag);
    
    //char UserAccount[16];
    //char UserPassWord[33];
    //uint32 ClientVersion;
    //uint32 SessionID;
    //uint32 ClientCheckSum;
    //uint16 Flag;
private:
	virtual void ProcessPacket();
	
	string m_userAccount;
	string m_userPassWord;
	uint32 m_clientVersion;
	uint32 m_sessionID;
	uint32 m_clientCheckSum;
	uint16 m_flag;
};

inline string AuthAccount::GetUserAccount() {
	return m_userAccount;
}

inline string AuthAccount::GetUserPassWord() {
	return m_userPassWord;
}

inline uint32 AuthAccount::GetClientVersion() {
	return m_clientVersion;
}

inline uint32 AuthAccount::GetSessionID() {
	return m_sessionID;
}

inline uint32 AuthAccount::GetClientCheckSum() {
	return m_clientCheckSum;
}

inline uint16 AuthAccount::GetFlag() {
	return m_flag;
}

inline void AuthAccount::SetUserAccount(string userAccount) {
	m_userAccount = userAccount;
}

inline void AuthAccount::SetUserPassWord(string userPassWord) {
	m_userPassWord = userPassWord;
}

inline void AuthAccount::SetClientVersion(uint32 clientVersion) {
	m_clientVersion = clientVersion;
}

inline void AuthAccount::SetSessionID(uint32 sessionID) {
	m_sessionID = sessionID;
}

inline void AuthAccount::SetClientCheckSum(uint32 clientCheckSum) {
	m_clientCheckSum = clientCheckSum;
}

inline void AuthAccount::SetFlag(uint16 flag) {
	m_flag = flag;
}


}
}
}
}

#endif	/* AUTHACCOUNT_H */

