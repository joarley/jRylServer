#ifndef _SERVERLIST_H_
#define	_SERVERLIST_H_

#include "shared/cBuffer.h"
#include "shared/cPacketBase.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace packet {

class ServerList: public PacketBase{
public:
	struct AuthServer{
		uint8 Group;
		string Name;
		int32 ClientNum[2]; //something chanel server set to 1
		int32 Address;
	};
	enum ServerListErro {
		SLE_Sucess,
		SLE_FailGetList	 = 4,
		SLE_ServerUnavailable = 10 
	};

	ServerList(Buffer_ptr buff);
	ServerList();
    virtual ~ServerList();

    static const uint8 PacketID = 0x56;
    static const uint16 PacketLength = 398;

	inline uint32 GetClientVersion();
    inline string GetPatchAddress();
    inline uint16 GetNumServers();
	inline AuthServer* GetAuthServers();
	inline ServerListErro GetErro();

	inline void SetClientVersion(uint32 clientVersion);
    inline void SetPatchAddress(string patchAddress);
    inline void SetNumServers(uint16 numServers);
	inline void SetErro(ServerListErro serverListErro);

    //Packet struct
    //uint32 ClientVersion
    //char PatchAddress[100]
    //uint16 NumServers
    //begin loop 0 to 10
    //uint8 Group;
    //char Name[15];
    //int32 ClientNum[2]; //something chanel server set to 1
    //uint32 m_Address;
    //end loop
private:
	virtual void ProcessPacket();

	uint32 m_clientVersion;
    string m_patchAddress;
    uint16 m_numServers;
    AuthServer m_authServers[AUTHSERVER_SLOT_LENGTH];
};

inline uint32 ServerList::GetClientVersion() {
	return m_clientVersion;
}

inline string ServerList::GetPatchAddress() {
	return m_patchAddress;
}

inline uint16 ServerList::GetNumServers() {
	return m_numServers;
}

inline ServerList::AuthServer* ServerList::GetAuthServers() {
	return m_authServers;
}

inline ServerList::ServerListErro ServerList::GetErro() {
	return (ServerListErro)m_Status;
}

inline void ServerList::SetClientVersion(uint32 clientVersion) {
	m_clientVersion = clientVersion;
}

inline void ServerList::SetPatchAddress(string patchAddress) {
	m_patchAddress = patchAddress;
}

inline void ServerList::SetNumServers(uint16 numServers) {
	m_numServers = numServers;
}

inline void ServerList::SetErro(ServerListErro serverListErro) {
	m_Status = serverListErro;
}

}
}
}
}
#endif	/* _SERVERLIST_H_ */

