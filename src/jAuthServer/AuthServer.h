#ifndef _JAUTHSERVER_H_
#define	_JAUTHSERVER_H_

#include "shared/iStartupClass.h"
#include "shared/cConfLoad.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/network/cSocketMgr.h"
#include "shared/time/cTimeMgr.h"
#include "shared/utils.h"
#include "cLoginServer.h"

#include <vector>
#include <string>

using namespace std;

namespace jRylServer {
namespace jAuthServer {
using namespace common::shared;

class AuthServer : public StartupClass {
public:
    AuthServer(vector<string> params);
    virtual ~AuthServer();
    int Start();
    void Stop();
    bool LoadConfig();
protected:
	uint8 m_serverSlot;
	string m_serverName;
	string m_globalUser;
	string m_globalPass;
	uint32 m_version;
	uint32 m_checkSum;
	uint8 m_serverGroup;
	common::shared::Logger::LogFile_ptr m_LogFile;
	LoginServer m_loginServer;
};

}
}

#endif	/* _JAUTHSERVER_H_ */