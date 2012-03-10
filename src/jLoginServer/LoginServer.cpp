#include "LoginServer.h"

#include "shared/cConfLoad.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/network/cSocketMgr.h"
#include "shared/time/cTimeMgr.h"
#include "shared/utils.h"
#include "shared/crypt/cCryptEngine.h"

#include "cAuthServerMgr.h"
#include "cAuthServer.h"

#include "shared/packet/pktPing.h"
#include "shared/packet/pktServerList.h"
#include "shared/packet/pktServerListAsk.h"

#include <ctime>
#include <string>

using namespace std;

#define STARTUP_CLASS jLoginServer::LoginServer
#define MODULE_NAME "LoginServer"
#include "shared/main.h"
#include "shared/database/cDBMgr.h"

namespace jRylServer {
namespace jLoginServer {
using namespace common::shared;

LoginServer::LoginServer(std::vector<std::string> params) { }

bool LoginServer::LoadConfig() {
    ConfLoad confGlobal;
    ConfLoad confLoginServer;
    if (!confGlobal.LoadConfig(FILE_CONF_GLOBAL_INI)) {
        __LOG->ShowError("LoginServer::LoadConfig: Error Load ini file(%s)\n", FILE_CONF_GLOBAL_INI);
        return false;
    }
    if (!confLoginServer.LoadConfig(FILE_CONF_LOGINSERVER_INI)) {
        __LOG->ShowError("LoginServer::LoadConfig: Error Load ini file(%s)\n", FILE_CONF_LOGINSERVER_INI);
        return false;
    }
    //LoginServer params load
    {
        //Create Log file
        string fileDir;
        bool fileTrucate;
        if (!confLoginServer.GetBool("Log", "Truncate", fileTrucate)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key LoginServer::Log:Truncate\n");
            return false;
        }
        if (!confLoginServer.GetString("Log", "File", fileDir)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key LoginServer::Log:File\n");
            return false;
        }
        m_logFile = __LOG->CreateLogFile(fileDir.c_str(), fileTrucate);
        __LOG->AddObserver(m_logFile);
    }
    //Global params load
    {
		if (!confGlobal.GetUInt("Client", "Version", m_version)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::Client:Version\n");
            return false;
        }
        if (!confGlobal.GetString("Client", "PatchAddress", m_patchAddress)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::Client:PatchAddress\n");
            return false;
        }
        if (!confGlobal.GetString("LoginServer", "Address", m_address)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::LoginServer:Address\n");
            return false;
        }
		if (!confGlobal.GetString("LoginServer", "LauncherPort", m_launcherListenPort)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::LoginServer:LauncherPort\n");
            return false;
        }
        if (!confGlobal.GetString("LoginServer", "AuthServerPort", m_authServerListenPort)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::LoginServer:AuthServerPort\n");
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

		m_authServerMgr.SetConf(m_address, m_authServerListenPort, m_globalUser, m_globalPass);
        
        byte GGkey[100];
        byte GGClientSeedKeyIni[100];
        byte GGServerSeedKeyIni[100];
        if (confGlobal.GetBytes("GameGuard", "Key", GGkey) < 0) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::GameGuard:Key\n");
            return false;
        }
        if (confGlobal.GetBytes("GameGuard", "ClientSeedKeyIni", GGClientSeedKeyIni) < 0) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::GameGuard:ClientSeedKeyIni\n");
            return false;
        }
        if (confGlobal.GetBytes("GameGuard", "ServerSeedKeyIni", GGServerSeedKeyIni) < 0) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::GameGuard:ServerSeedKeyIni\n");
            return false;
        }
        crypt::CryptEngine::GetInstance().SetGGCryptParams(GGClientSeedKeyIni, GGServerSeedKeyIni, GGkey);
    }

	{//Network configure
		if(!confGlobal.GetString("LoginServer", "Address", m_address)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::LoginServer:Address\n");
            return false;
        }
		if(!confGlobal.GetString("LoginServer", "LauncherPort", m_launcherListenPort)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::LoginServer:LauncherPort\n");
            return false;
        }
		if(!confGlobal.GetString("LoginServer", "AuthServerPort", m_authServerListenPort)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::LoginServer:AuthServerPort\n");
            return false;
        }

		m_launcherListen = network::SocketMgr::GetInstance().CreateServer("ListenLaucher");

		if(!m_launcherListen->Bind(m_address, m_launcherListenPort)) {
            __LOG->ShowError("[jLoginServer] Error bind address for listen GameServers\n");
            return false;
        }
	}
	
	return true;
}

int LoginServer::Start() {
	m_authServerMgr.Start();
	m_launcherListen->Start();
	m_launcherListen->SetAcceptCallBack(boost::bind(&LoginServer::ConnectLauncher, this, _1));

	getchar();
	return 0;
}

void LoginServer::Stop() { 

}

LoginServer::~LoginServer() { }

bool LoginServer::ConnectLauncher(network::SocketSession_ptr socketSession) {
	socketSession->SetPacketProcessCallBack(boost::bind(&LoginServer::ParsePacketLauncher, this, _1, _2));
    return true;
}

void LoginServer::ParsePacketLauncher(network::SocketSession_ptr socketSession, Buffer_ptr buff) {
	buff->SetReaderOffset(0);
	
	switch(buff->Get<uint8>(1)) {
	case packet::Ping::PacketID:
		break;
	case packet::ServerListAsk::PacketID:
		packet::ServerList sl;
		AuthServer** authServerList = m_authServerMgr.GetAuthServerList();
		packet::ServerList::AuthServer* authserver = sl.GetAuthServers();
		int numServer = 0;
		for(int i = 0; i < AUTHSERVER_SLOT_LENGTH; i++) {
			if(authServerList[i] != NULL && authServerList[i]->GetStatus() == AuthServer::ASS_OK) {
				authserver[numServer].Group = authServerList[i]->GetGroup();
				authserver[numServer].Name = authServerList[i]->GetName();
				authserver[numServer].Address = authServerList[i]->GetAddress();
				authserver[numServer].ClientNum[0] = 0;
				authserver[numServer].ClientNum[1] = 0;
				numServer++;
			}
		}
		if(numServer > 0) {		
			sl.SetClientVersion(m_version);
			sl.SetPatchAddress(m_patchAddress);
			sl.SetNumServers(numServer);
		} else {
			sl.SetErro(packet::ServerList::SLE_ServerUnavailable);
		}
		socketSession->SendPacket(sl);
		break;
	}
}

} //namespace jLoginServer
} //namespace jRylServer