#ifndef JLOGINSERVER_H
#define	JLOGINSERVER_H

#include "shared/iStartupClass.h"
#include "shared/typedef.h"
#include "shared/stdDefs.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/cLogger.h"
#include "shared/cBuffer.h"

#include "cAuthServerMgr.h"

#include <map>
#include <string>
#include <ctime>

using namespace std;

namespace jRylServer {
namespace jLoginServer {
using namespace common::shared;

class LoginServer : public StartupClass {
public:
    LoginServer(vector<string> params);
    virtual ~LoginServer();
    int Start();
    void Stop();
    bool LoadConfig();

	bool ConnectLauncher(network::SocketSession_ptr socketSession);
	void ParsePacketLauncher(network::SocketSession_ptr socketSession, Buffer_ptr buff);
protected:
	string m_address;
	uint32 m_version;
	string m_patchAddress;
    string m_launcherListenPort;
	string m_authServerListenPort;
	network::SocketServer* m_launcherListen;

	AuthServerMgr m_authServerMgr;
    string m_globalUser;
    string m_globalPass;
    Logger::LogFile_ptr m_logFile;
};

}
}

#endif	/* JLOGINSERVER_H */