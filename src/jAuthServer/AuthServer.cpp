#include "AuthServer.h"

#include "shared/cConfLoad.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/network/cSocketMgr.h"
#include "shared/time/cTimeMgr.h"
#include "shared/utils.h"
#include "shared/crypt/cCryptEngine.h"

#include <boost/algorithm/string/replace.hpp>
#include <boost/bind.hpp>

#include <vector>
#include <string>

using namespace std;

#define STARTUP_CLASS jAuthServer::AuthServer
#define MODULE_NAME "AuthServer"
#include "shared/main.h"

namespace jRylServer {
namespace jAuthServer {
using namespace common::shared;

AuthServer::AuthServer(vector<string> params) {
	m_serverSlot = 0;
	vector<string>::iterator it = params.begin();
	while(it != params.end()) {
		if(*it == "-a") {
			it++;
			m_serverSlot = j_atoi<uint8>(it->c_str());
		}
		it++;
	}
}

int AuthServer::Start() {
	network::SocketMgr::GetInstance().Start();
	m_loginServer.Start();
	getchar();
	return 0;
}

bool AuthServer::LoadConfig() {
	ConfLoad confGlobal;
    ConfLoad confAuthServer;
	string fileConfAuth = FILE_CONF_AUTHSERVER_INI;
	boost::algorithm::replace_all(fileConfAuth, "{0}", j_itoa(m_serverSlot));

	if (!confGlobal.LoadConfig(FILE_CONF_GLOBAL_INI)) {
        __LOG->ShowError("AuthServer::LoadConfig: Error Load ini file(%s)\n", FILE_CONF_GLOBAL_INI);
        return false;
    }
	if (!confAuthServer.LoadConfig(fileConfAuth.c_str())) {
        __LOG->ShowError("AuthServer::LoadConfig: Error Load ini file(%s)\n", fileConfAuth);
        return false;
    }

	//AuthServer params load
    {
        //Create Log file
        string fileDir;
        bool fileTrucate;
        if (!confAuthServer.GetBool("Log", "Truncate", fileTrucate)) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key AuthServer::Log:Truncate\n");
            return false;
        }
        if (!confAuthServer.GetString("Log", "File", fileDir)) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key AuthServer::Log:File\n");
            return false;
        }
		if (!confAuthServer.GetString("AuthServer", "Name", m_serverName)) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key AuthServer::AuthServer:Name\n");
            return false;
        }
		boost::algorithm::replace_all(fileDir, "{0}", j_itoa(m_serverSlot));
        m_LogFile = __LOG->CreateLogFile(fileDir.c_str(), fileTrucate);
        __LOG->AddObserver(m_LogFile);
    }

	//Global params load
    {
		string loginServerAddress;
		string loginServerPort;
		
        if (!confGlobal.GetUInt("Client", "Version", m_version)) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key Global::Client:Version\n");
            return false;
        }
		if (!confGlobal.GetUInt("Server", "Group", m_serverGroup)) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key Global::Server:Group\n");
            return false;
        }
		if (!confGlobal.GetUInt("Client", "Checksum", m_checkSum)) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key Global::Client:Checksum\n");
            return false;
        }
		if (!confGlobal.GetString("LoginServer", "Address", loginServerAddress)) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key Global::LoginServer:Address\n");
            return false;
        }
		if (!confGlobal.GetString("LoginServer", "AuthServerPort", loginServerPort)) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key Global::LoginServer:AuthServerPort\n");
            return false;
        }
		if (!confGlobal.GetString("Server", "GlobalUser", m_globalUser)) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key Global::Server:GlobalUser\n");
            return false;
        }
		if (!confGlobal.GetString("Server", "GlobalPass", m_globalPass)) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key Global::Server:GlobalPass\n");
            return false;
        }

		m_loginServer.SetConf(m_serverSlot, m_serverName, m_serverGroup, loginServerAddress,
			loginServerPort, m_globalUser, m_globalPass);
        
        byte GGkey[100];
        byte GGClientSeedKeyIni[100];
        byte GGServerSeedKeyIni[100];
        if (confGlobal.GetBytes("GameGuard", "Key", GGkey) < 0) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key Global::GameGuard:Key\n");
            return false;
        }
        if (confGlobal.GetBytes("GameGuard", "ClientSeedKeyIni", GGClientSeedKeyIni) < 0) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key Global::GameGuard:ClientSeedKeyIni\n");
            return false;
        }
        if (confGlobal.GetBytes("GameGuard", "ServerSeedKeyIni", GGServerSeedKeyIni) < 0) {
            __LOG->ShowError("AuthServer::LoadConfig: Error key Global::GameGuard:ServerSeedKeyIni\n");
            return false;
        }
        crypt::CryptEngine::GetInstance().SetGGCryptParams(GGClientSeedKeyIni, GGServerSeedKeyIni, GGkey);
    }

	return true;
}

AuthServer::~AuthServer() {

}

void AuthServer::Stop() {

}

}
}