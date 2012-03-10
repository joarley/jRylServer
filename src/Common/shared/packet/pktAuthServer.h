#ifndef _AUTHSERVER_H_
#define	_AUTHSERVER_H_

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"


namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

class AuthServer: public PacketBase{
public:
	static const uint8 PacketID = 0x01;
    static const uint16 PacketLength = 0;

    AuthServer(Buffer_ptr buff);
	AuthServer();
    virtual ~AuthServer();

	inline uint8 GetSlotNum();
    inline string GetUser();
    inline string GetPass();

	inline void SetSlotNum(uint8 slotNum);
    inline void SetUser(string user);
    inline void SetPass(string pass);

    //Packet struct
    //uint8 SlotNum;
    //char* User;
    //char* Pass;
private:
	virtual void ProcessPacket();

	uint8 m_slotNum;
	string m_user;
	string m_pass;
};

inline uint8 AuthServer::GetSlotNum() {
	return m_slotNum;
}

inline string AuthServer::GetUser() {
	return m_user;
}

inline string AuthServer::GetPass() {
	return m_pass;
}

inline void AuthServer::SetSlotNum(uint8 slotNum) {
	m_slotNum = slotNum;
}

inline void AuthServer::SetUser(string user) {
	m_user = user;
}

inline void AuthServer::SetPass(string pass) {
	m_pass = pass;
}

}
}
}
}
#endif	/* _AUTHSERVER_H_ */

