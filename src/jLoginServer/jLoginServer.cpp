#include "jLoginServer.h"
#if (defined _WIN32 || defined _WIN64) && defined _DEBUG
#include <vld.h>
#endif
#include "shared/cConfLoad.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/network/cSocketMgr.h"

#define STARTUP_CLASS jLoginServer
#define MODULE_NAME "LoginServer"
#include "shared/main.h"

namespace jRylServer {

    using namespace common::shared;

    jLoginServer::jLoginServer(std::vector<std::string> params) {
        m_socketMgr = network::SocketMgr::GetInstance_ptr();
        m_socketMgr->CreateServer("LoginServer");
    }

    bool jLoginServer::LoadConfig() {
        return true;
    }
    
    int jLoginServer::Start() {
        network::SocketServer* server = m_socketMgr->GetServer("LoginServer");
        m_socketMgr->Start();

        server->Bind("192.168.1.100", "1808");
        
        server->SetAcceptCallBack(boost::bind(&jLoginServer::newClient, this, _1));
        server->Start();
        
        getchar();
        m_socketMgr->DestroyServer("LoginServer");
        m_socketMgr->GetServer("LoginServer");
        m_socketMgr->Stop();
        return 0;
    }

    jLoginServer::~jLoginServer() {
    }

    bool jLoginServer::newClient(network::SocketSession_ptr client) {
        client->SetPacketProcessCallBack(boost::bind(&jLoginServer::processClient, this, _1, _2));
        Logger::GetInstance().ShowMessage("new client conectado\n");
        return true;
    }

    void jLoginServer::processClient(network::SocketSession_ptr client, Buffer_ptr buff) {
        Logger::GetInstance().ShowMessage("Recebido: %d\n", buff->Get<uint32>());
        client->Write(buff);
    }
}
