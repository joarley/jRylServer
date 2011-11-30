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

#include "Packets/packets.h"

#include <ctime>
#include <string>

using namespace std;

#define STARTUP_CLASS jLoginServer::LoginServer
#define MODULE_NAME "LoginServer"
#include "shared/main.h"

namespace jRylServer {
namespace jLoginServer {
using namespace common::shared;

LoginServer::LoginServer(std::vector<std::string> params) {
    m_Version = 0;
    memset(m_AuthServers, NULL, sizeof(AuthServer*) * AUTHSERVER_SLOT_LENGTH);
}

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
        m_LogFile = __LOG->CreateLogFile(fileDir.c_str(), fileTrucate);
        __LOG->AddObserver(m_LogFile);
    }
    //Global params load
    {
        if (!confGlobal.GetUInt("Client", "Version", m_Version)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::Client:Version\n");
            return false;
        }
        if (!confGlobal.GetString("Client", "PatchAddress", m_PatchAddress)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::Client:PatchAddress\n");
            return false;
        }
        if (!confGlobal.GetString("LoginServer", "Address", m_Address)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::LoginServer:Address\n");
            return false;
        }
        if (!confGlobal.GetString("LoginServer", "LauncherPort", m_LauncherListenPort)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::LoginServer:LauncherPort\n");
            return false;
        }
        if (!confGlobal.GetString("LoginServer", "AuthServerPort", m_AuthServerListenPort)) {
            __LOG->ShowError("LoginServer::LoadConfig: Error key Global::LoginServer:AuthServerPort\n");
            return false;
        }
        
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

    return true;
}

int LoginServer::Start() {
    return 0;
}

void LoginServer::Stop() {
}

LoginServer::~LoginServer() {

}

bool LoginServer::ConnectAuthServer(network::SocketSession_ptr socketSession) {
    m_UnauthorizedSessions[socketSession].State = UnauthorizedSession::US_WAIT_PING;
    m_UnauthorizedSessions[socketSession].Time = ::time(NULL);
    m_UnauthorizedSessions[socketSession].AttemptLogin = 0;
    return true;
}

void LoginServer::ParsePacketUnauthorizedSession(network::SocketSession_ptr socketSession, Buffer_ptr buff) {
    buff->SetReaderOffset(0);
    if(buff->Get<uint8>() != PACKET_START_BIT || buff->Length() < PACKET_HEADER_SIZE) {
        m_UnauthorizedSessions.erase(socketSession);
        socketSession->Stop();
        return;
    }
    
    crypt::CryptEngine::GetInstance().XorDecrypt(buff);
    switch (m_UnauthorizedSessions[socketSession].State) {
        case UnauthorizedSession::US_WAIT_PING: {
            if(buff->Get<uint>(1) != PacketAuthServer::Ping::PacketID
                || buff->Length() != PacketAuthServer::Ping::PacketLength) {
                m_UnauthorizedSessions.erase(socketSession);
                socketSession->Stop();
                return;
            }
            m_UnauthorizedSessions[socketSession].State =  UnauthorizedSession::US_WAIT_USERPASS;
            m_UnauthorizedSessions[socketSession].Time = ::time(NULL);
            break;
        }
        case UnauthorizedSession::US_WAIT_USERPASS: {
            if(buff->Get<uint>(1) != PacketAuthServer::AuthServer::PacketID) {
                m_UnauthorizedSessions.erase(socketSession);
                socketSession->Stop();
                return;
            }
            PacketAuthServer::AuthServer authServer(buff, m_GlobalUser.c_str(), m_GlobalPass.c_str());
            if(authServer.IsAuthenticated()) {
                uint8 slotnum = authServer.GetSlot();
                if(m_AuthServers[slotnum] != NULL) {
                    bool slotfound = false;
                    for(slotnum++;slotnum < AUTHSERVER_SLOT_LENGTH;slotnum++) {
                        if(m_AuthServers[slotnum] == NULL) {
                            slotfound = true;
                            break;
                        }
                    }
                    if(!slotfound) {
                        for(slotnum = authServer.GetSlot();slotnum >= 0;slotnum--) {
                            if(m_AuthServers[slotnum] == NULL) {
                                slotfound = true;
                                break;
                            }
                        }
                    }
                    if(!slotfound) {
                        m_UnauthorizedSessions.erase(socketSession);
                        PacketServer_AuthServer::AuthServer authRes(0, PacketServer_AuthServer::AuthServer::EA_SERVER_FULL);
                        socketSession->SendPacket(authRes.GetProcessedBuffer());
                        socketSession->Stop();
                        return;
                    }
                    
                }

                 m_UnauthorizedSessions.erase(socketSession);
                 m_AuthServers[slotnum] = new AuthServer(socketSession, this);
            } else {
                if(m_UnauthorizedSessions[socketSession].AttemptLogin == MAX_AUTHSERVER_LOGIN_ATTEMPTS) {
                    m_UnauthorizedSessions.erase(socketSession);
                    PacketServer_AuthServer::AuthServer authRes(0, PacketServer_AuthServer::AuthServer::EA_MAX_LOGIN_ATTEMPTS);
                    socketSession->SendPacket(authRes.GetProcessedBuffer());
                    socketSession->Stop();
                } else {
                    PacketServer_AuthServer::AuthServer authRes(0, PacketServer_AuthServer::AuthServer::EA_USER_PASS_INCORRECT);
                    socketSession->SendPacket(authRes.GetProcessedBuffer());
                    m_UnauthorizedSessions[socketSession].AttemptLogin++;
                }
            }
            break;
        }
        default:
            m_UnauthorizedSessions.erase(socketSession);
            socketSession->Stop();
            break;
    }
}

void LoginServer::CloseAuthServer(AuthServer* authServer) {
    for(int i = 0; i < AUTHSERVER_SLOT_LENGTH; i++) {
        if(m_AuthServers[i] == authServer) {
            delete m_AuthServers[i];
            break;
        }
    }
}


} //namespace jLoginServer
} //namespace jRylServer