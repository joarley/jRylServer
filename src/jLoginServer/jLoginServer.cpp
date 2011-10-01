#include "jLoginServer.h"

#include "shared/cConfLoad.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/network/cSocketMgr.h"
#include "shared/time/cTimeMgr.h"
#include "shared/utils.h"

#define STARTUP_CLASS jLoginServer
#define MODULE_NAME "LoginServer"
#include "shared/main.h"

namespace jRylServer {
    using namespace common::shared;

    jLoginServer::jLoginServer(std::vector<std::string> params) {
        
    }

    bool jLoginServer::LoadConfig() {
        ConfLoad global;
        if(!global.LoadConfig("./config/global.ini")) {
            Logger::GetInstance().ShowError("[jLoginServer] Could not load configuration file [./config/global.ini]");
            return false;
        }

        
        {//CryptEngine configure
            byte GGKey[30];
            byte GGClientSeedKeyIni[16];
            byte GGServerSeedKeyIni[16];
            int ret;

            ret = global.GetBytes(GGKey, "GameGuard", "Key");
            if(ret < 0) {
                Logger::GetInstance().ShowError("[jLoginServer] key[GameGuard:Key] set wrong in file [./config/global.ini]");
                return false;
            }
            ret = global.GetBytes(GGClientSeedKeyIni, "GameGuard", "ClientSeedKeyIni");
            if(ret != 16) {
                Logger::GetInstance().ShowError("[jLoginServer] key[GameGuard:ClientSeedKeyIni] set wrong in file [./config/global.ini]");
                return false;
            }
            ret = global.GetBytes(GGServerSeedKeyIni, "GameGuard", "ServerSeedKeyIni");
            if(ret != 16) {
                Logger::GetInstance().ShowError("[jLoginServer] key[GameGuard:ServerSeedKeyIni] set wrong in file [./config/global.ini]");
                return false;
            }
            crypt::CryptEngine::GetInstance().SetGGCryptParams(GGClientSeedKeyIni, GGServerSeedKeyIni, GGKey);
        }
        {//DataBase configure
            char* type = global.GetString("DataBase","Type");
            if(type == NULL) {
                Logger::GetInstance().ShowError("[jLoginServer] key[DataBase:Type] set wrong in file [./config/global.ini]");
                return false;
            }
            if(strcmp(type, "mysql") == 0 || strcmp(type, "postgre") == 0) {
                char* address = global.GetString("DataBase", "Address");
                char* port = global.GetString("DataBase", "Port");
                char* user = global.GetString("DataBase", "User");
                char* pass = global.GetString("DataBase", "Pass");
                char* base = global.GetString("DataBase", "Base");
                if(address == NULL || port == NULL || user == NULL || pass == NULL || base == NULL) {
                    Logger::GetInstance().ShowError("[jLoginServer] section[DataBase] set wrong params in file [./config/global.ini]");
                    return false;
                }

                if(strcmp(type, "mysql") == 0) {
                    database::DBMgr::GetInstance().SetConnectionDetails(database::DBMgr::DB_mysql, address, port, user, pass, base);
                } else {
                    database::DBMgr::GetInstance().SetConnectionDetails(database::DBMgr::DB_postgresql, address, port, user, pass, base);
                }
            } else if(strcmp(type, "sqlite") == 0) {
                char* file = global.GetString("DataBase", "file");
                database::DBMgr::GetInstance().SetConnectionDetails(database::DBMgr::DB_sqlite3, file);
            } else {
                Logger::GetInstance().ShowError("[jLoginServer] key[DataBase:Type] set wrong in file [./config/global.ini]");
                return false;
            }

            int poolsize;
            if(!global.GetInt("DataBase", "PoolSize", poolsize)) {
                Logger::GetInstance().ShowError("[jLoginServer] key[DataBase:PoolSize] set wrong in file [./config/global.ini]");
            }
            database::DBMgr::GetInstance().SetSizePool(poolsize);
        }

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