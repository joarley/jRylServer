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
#if defined(USE_DB_ENGINE)
#include "shared/database/cDBMgr.h"
#endif
#include "shared/crypt/cCryptEngine.h"

using namespace jRylServer;
using namespace jRylServer::common::shared;

#if defined(_WIN32) || defined(_WIN64) 
#include <Windows.h>
#ifdef _DEBUG
#include <vld.h>
#endif
BOOL sighandler(DWORD fdwCtrlType);
#else
#include <signal.h>
void sighandler(int signum, siginfo_t *info, void *ptr);
#endif

bool StartSignalCtrl();

StartupClass* Main;

#ifdef STARTUP_CLASS

int main(int argc, char *argv[]) {

    if (!StartSignalCtrl()) {
        return -1;
    }

    Logger::GetInstance().ClearDefaultLogFile();
    Logger::GetInstance().ShowMessage(Splash, MODULE_NAME, 0);

    Logger::GetInstance().ShowInfo("[main] Creating module\n");
    std::vector<std::string> params;
    for (int i = 0; i < argc; i++) {
        params.push_back(argv[i]);
    }
    Main = new STARTUP_CLASS(params);

    Logger::GetInstance().ShowInfo("[main] Starting module configuration\n");
    if (!Main->LoadConfig()) {
        return -1;
    }

    if (!time::TimeMgr::GetInstance().Start()) {
        Logger::GetInstance().ShowError("[main] Error starting TimeMgr\n");
        return -1;
    }
    if (!network::SocketMgr::GetInstance().Start()) {
        Logger::GetInstance().ShowError("[main] Error starting SocketMgr\n");
        return -1;
    }
#if defined(USE_DB_ENGINE)
    if (!database::DBMgr::GetInstance().Start()) {
        Logger::GetInstance().ShowError("[main] Error starting DBMgr\n");
        return -1;
    }
#endif
    if (!crypt::CryptEngine::GetInstance().Start()) {
        Logger::GetInstance().ShowError("[main] Error starting CryptEngine\n");
        return -1;
    }

    int startRet = Main->Start();
    delete Main;

    crypt::CryptEngine::GetInstance().Stop();
#if defined(USE_DB_ENGINE)
    database::DBMgr::GetInstance().Stop();
#endif
    time::TimeMgr::GetInstance().Stop();
    network::SocketMgr::GetInstance().Stop();

    Logger::GetInstance().ShowMessage(CL_RESET);

#if defined(USE_DB_ENGINE)
    database::DBMgr::DestroyInstance();
#endif
    Logger::DestroyInstance();
    time::TimeMgr::DestroyInstance();
    crypt::CryptEngine::DestroyInstance();
    network::SocketMgr::DestroyInstance();

    return startRet;
}
#else
#error STARTUP_CLASS not defined
#endif

#if defined(_WIN32) || defined(_WIN64)

bool StartSignalCtrl() {
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE) sighandler, TRUE)) {
        Logger::GetInstance().ShowError("Unable to add action to process signal control.\n");
        return false;
    }
    return true;
}

BOOL sighandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
        case CTRL_BREAK_EVENT:
        case CTRL_CLOSE_EVENT://Not cancelable
            Main->Stop();
            return TRUE;
        default:
            return FALSE;
    }
}

#else

#endif

#endif