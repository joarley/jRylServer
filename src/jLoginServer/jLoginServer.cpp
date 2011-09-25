#include "jLoginServer.h"

#include "shared/cConfLoad.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/network/cSocketMgr.h"
#include "shared/time/cTimeMgr.h"

#define STARTUP_CLASS jLoginServer
#define MODULE_NAME "LoginServer"
#include "shared/main.h"

void back(time::Time::TimeError err, const char* errMsg) {
    common::shared::Logger::GetInstance().ShowError("terminou\n");
}

namespace jRylServer {
    using namespace common::shared;

    jLoginServer::jLoginServer(std::vector<std::string> params) {
        
    }

    bool jLoginServer::LoadConfig() {
        return true;
    }
    
    int jLoginServer::Start() {
        return 0;
    }

    void jLoginServer::Stop() {
    }

    jLoginServer::~jLoginServer() {
    }
}