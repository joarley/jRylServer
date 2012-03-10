#ifndef _SERVERDETAIL_H_
#define	_SERVERDETAIL_H_

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

using namespace common::shared;

class ServerDetail: public PacketBase{
public:
    ServerDetail(Buffer_ptr buff);
	ServerDetail();

    virtual ~ServerDetail();
    
    static const uint8 PacketID = 0x02;
    static const uint16 PacketLength = 0;

    inline uint8 GetGroup();
    inline string GetName();
    inline uint32 GetAddress();

	inline void SetGroup(uint8 group);
    inline void SetName(string name);
    inline void SetAddress(uint32 address);

    //Packet struct
    //uint8 Group
    //string Name
    //uint32 Address
private:
	virtual void ProcessPacket();

    uint8 m_group;
    string m_name;
    uint32 m_address;
};

inline uint8 ServerDetail::GetGroup() {
    return m_group;
}

inline string ServerDetail::GetName() {
    return m_name;
}

inline uint32 ServerDetail::GetAddress() {
    return m_address;
}

inline void ServerDetail::SetGroup(uint8 group) {
	m_group = group;
}

inline void ServerDetail::SetName(string name) {
	m_name = name;
}

inline void ServerDetail::SetAddress(uint32 address) {
	m_address = address;
}

}
}
}
}

#endif	/* _SERVERDETAIL_H_ */