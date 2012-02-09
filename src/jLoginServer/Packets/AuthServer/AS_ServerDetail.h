#ifndef PacketAuthServer_SERVERDETAIL_H
#define	PacketAuthServer_SERVERDETAIL_H

#include "shared/cPacketBase.h"
#include "shared/typedef.h"

#include <string>

using namespace std;

namespace jRylServer {
namespace jLoginServer {
namespace PacketAuthServer {

using namespace common::shared;

class ServerDetail: public PacketBase{
public:
    ServerDetail(Buffer_ptr buff);
    virtual ~ServerDetail();
    
    static const uint8 PacketID = 0x02;
    //static const uint16 PacketLength = ??;

    inline uint8 GetGroup();
    inline string GetName();
    inline uint32 GetAdress();

    //Packet struct
    //uint8 Group
    //string Name
    //uint32 Adress
private:
    uint8 m_Group;
    string m_Name;
    uint32 m_Adress;
};

inline uint8 ServerDetail::GetGroup() {
    return m_Group;
}

inline string ServerDetail::GetName() {
    return m_Name;
}

inline uint32 ServerDetail::GetAdress() {
    return m_Adress;
}

}
}
}
#endif	/* PacketAuthServer_SERVERDETAIL_H */

