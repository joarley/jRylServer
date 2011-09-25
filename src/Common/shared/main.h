#ifndef MAIN_H
#define MAIN_H
#include <string>
#include <vector>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include "shared/iStartupClass.h"
#include "shared/Splash.h"
#include "shared/cLogger.h"
#include "shared/time/cTimeMgr.h"
#include "shared/network/cSocketMgr.h"

using namespace jRylServer;
using namespace jRylServer::common::shared;

int main(int argc, char *argv[]) {
    Logger::GetInstance().ClearDefaultLogFile();
    Logger::GetInstance().ShowMessage(Splash, MODULE_NAME, 0);
    
    time::TimeMgr::GetInstance().Start();
    network::SocketMgr::GetInstance().Start();

    

#ifdef STARTUP_CLASS
    //BOOST_STATIC_ASSERT_MSG((boost::is_base_of<iStartupClass, STARTUP_CLASS>::value), "STARTUP_CLASS is not a StartupClass");
    
    std::vector<std::string> params;
    for (int i = 0; i < argc; i++) {
        params.push_back(argv[i]);
    }
    STARTUP_CLASS Main(params);
    if (!Main.LoadConfig()) {
        return -1;
    }
    int startRet = Main.Start();

    time::TimeMgr::GetInstance().Stop();
    network::SocketMgr::GetInstance().Stop();
    
    Logger::GetInstance().ShowMessage(CL_RESET);
    Logger::DestroyInstance();
    crypt::CryptEngine::DestroyInstance();
    network::SocketMgr::DestroyInstance();
    
    return startRet;
#else
#error STARTUP_CLASS not defined
#endif
}


#endif