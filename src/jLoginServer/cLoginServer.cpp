#include "cLoginServer.h"

#include "shared/cConfLoad.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"
#include "shared/network/cSocketServer.h"
#include "shared/network/cSocketSession.h"
#include "shared/network/cSocketMgr.h"
#include "shared/time/cTimeMgr.h"
#include "shared/utils.h"

#include "cAccount.h"
#include "cCharacter.h"

#include "Packets/Client/C_AuthAccount.h"
#include "Packets/Client/C_Ping.h"
#include "Packets/Client/C_UserLogin.h"

#define STARTUP_CLASS jLoginServer::LoginServer
#define MODULE_NAME "LoginServer"
#include "shared/main.h"

namespace jRylServer {
namespace jLoginServer {
using namespace common::shared;

LoginServer::LoginServer(std::vector<std::string> params) {
    m_userCount = 0;
}

bool LoginServer::LoadConfig() {
    ConfLoad global;
    ConfLoad loginServer;
    Logger* log =  Logger::GetInstance_ptr();

    if(!global.LoadConfig("./config/global.ini")) {
        log->ShowError("[jLoginServer] Could not load configuration file [./config/global.ini]\n");
        return false;
    }

    if(!loginServer.LoadConfig("./config/jLoginServer.ini")) {
        log->ShowError("[jLoginServer] Could not load configuration file [./config/jLoginServer.ini]\n");
        return false;
    }
    
    {//Logger configure
        char* file = loginServer.GetString("Log", "File");
        bool trunc = loginServer.GetBool("Log", "Truncate");
        if(file == NULL) {
             log->ShowError("[jLoginServer] key[Log:File] set wrong in file [./config/jLoginServer.ini]\n");
             return false;
        }

        m_logFile = Logger::GetInstance().CreateLogFile(file, trunc);
        if(m_logFile->GetFileDescriptor() == INVALID_FILEDESCRIPTOR) {
            log->ShowError("[jLoginServer] could not open file log[%s]\n", file);
            return false;
        }
    }

    {//CryptEngine configure
        byte GGKey[30];
        byte GGClientSeedKeyIni[16];
        byte GGServerSeedKeyIni[16];
        int ret;

        ret = global.GetBytes(GGKey, "GameGuard", "Key");
        if(ret < 0) {
            log->ShowError("[jLoginServer] key[GameGuard:Key] set wrong in file [./config/global.ini]\n");
            return false;
        }
        ret = global.GetBytes(GGClientSeedKeyIni, "GameGuard", "ClientSeedKeyIni");
        if(ret != 16) {
            log->ShowError("[jLoginServer] key[GameGuard:ClientSeedKeyIni] set wrong in file [./config/global.ini]\n");
            return false;
        }
        ret = global.GetBytes(GGServerSeedKeyIni, "GameGuard", "ServerSeedKeyIni");
        if(ret != 16) {
            log->ShowError("[jLoginServer] key[GameGuard:ServerSeedKeyIni] set wrong in file [./config/global.ini]\n");
            return false;
        }
        crypt::CryptEngine::GetInstance().SetGGCryptParams(GGClientSeedKeyIni, GGServerSeedKeyIni, GGKey);
    }

    {//DataBase configure
        char* type = global.GetString("DataBase","Type");
        if(type == NULL) {
            log->ShowError("[jLoginServer] key[DataBase:Type] set wrong in file [./config/global.ini]\n");
            return false;
        }
        if(strcmp(type, "mysql") == 0 || strcmp(type, "postgre") == 0) {
            char* address = global.GetString("DataBase", "Address");
            char* port = global.GetString("DataBase", "Port");
            char* user = global.GetString("DataBase", "User");
            char* pass = global.GetString("DataBase", "Pass");
            char* base = global.GetString("DataBase", "Base");
            if(address == NULL || port == NULL || user == NULL || pass == NULL || base == NULL) {
                log->ShowError("[jLoginServer] section[DataBase] set wrong params in file [./config/global.ini]\n");
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
            log->ShowError("[jLoginServer] key[DataBase:Type] set wrong in file [./config/global.ini]\n");
            return false;
        }

        int poolsize;
        if(!global.GetInt("DataBase", "PoolSize", poolsize)) {
            log->ShowError("[jLoginServer] key[DataBase:PoolSize] set wrong in file [./config/global.ini]\n");
            return false;
        }
        database::DBMgr::GetInstance().SetSizePool(poolsize);
    }

    {//Network configure
        char* listenAddress = loginServer.GetString("Network", "ListenAddress");
        char* listenClientPort = loginServer.GetString("Network", "ListenClientPort");
        char* listenLauncherPort = loginServer.GetString("Network", "ListenLauncherPort");
        char* listenGameServerPort = loginServer.GetString("Network", "ListenGameServerPort");

        if(listenAddress == NULL|| listenClientPort == NULL|| listenLauncherPort == NULL|| listenGameServerPort == NULL) {
            log->ShowError("[jLoginServer] section[Network] set wrong params in file [./config/jLoginServer.ini]\n");
            return false;
        }

        m_listenClient = network::SocketMgr::GetInstance().CreateServer("ListenClient");
        m_listenLauncher = network::SocketMgr::GetInstance().CreateServer("ListenLauncher");
        m_listenGameServer = network::SocketMgr::GetInstance().CreateServer("ListenGameServer");

        if(!m_listenClient->Bind(listenAddress, listenClientPort)) {
            log->ShowError("[jLoginServer] Error bind address for listen cliets\n");
            return false;
        }
        if(!m_listenLauncher->Bind(listenAddress, listenLauncherPort)) {
            log->ShowError("[jLoginServer] Error bind address for listen launcher\n");
            return false;
        }
        if(!m_listenGameServer->Bind(listenAddress, listenGameServerPort)) {
            log->ShowError("[jLoginServer] Error bind address for listen GameServers\n");
            return false;
        }
    }

    {
         global.GetInt("Client", "Version", m_clientVersion);
         global.GetInt("Client", "Checksum", m_clientChecksum);
         m_patchAddress = global.GetString("Client", "PatchAddress");
         global.GetInt("Server", "MaxUsers", m_maxUsers);
    }

    return true;
}

int LoginServer::Start() {
    m_listenClient->Start();
    m_listenLauncher->Start();
    m_listenGameServer->Start();

    m_listenClient->SetAcceptCallBack(boost::bind(&LoginServer::ConnectClient, this, _1));

    getchar();
    return 0;
}

void LoginServer::Stop() {
}

LoginServer::~LoginServer() {
    m_logFile->Release();
}

uint32 LoginServer::GetId() {
    return m_id;
}

uint32 LoginServer::GetVersion() {
    return m_clientVersion;
}

uint32 LoginServer::GetChecksum() {
    return m_clientChecksum;
}

char* LoginServer::GetPatchAddress() {
    return m_patchAddress;
}

uint32 LoginServer::GetMaxUsers() {
    return m_maxUsers;
}

uint32 LoginServer::GetCurrentUsersCount() {
    return m_userCount;
}

Account* LoginServer::GetAccountById(uint32 id) {
    map<network::SocketSession_ptr, Account*>::iterator accLoginServer;
    map<uint32, Account*>::iterator accGameServer;
    accLoginServer = m_loginServerAccounts.begin(); 
    accGameServer = m_gameServerAccounts.begin();
    while(accLoginServer != m_loginServerAccounts.end()) {
        if(accLoginServer->second->GetId() == id) {
            return accLoginServer->second;
        }
        accLoginServer++;
    }
    
    map<uint32, Account*>::iterator it = m_gameServerAccounts.find(id);
    if(it != m_gameServerAccounts.end()) {
        return it->second;
    }
    return NULL;
}

bool LoginServer::LoginAccount(Account* acc) {
    m_loginServerAccounts[acc->GetSocketSession()] = acc;
    return true;
}

uint32 LoginServer::GenSessionId() {
    return 1808;
}

void LoginServer::PacketProcess(network::SocketSession_ptr session, Buffer_ptr buff) {
    buff->SetReaderOffset(0);
    if(buff->Get<uint8>() != PACKET_START_BIT) {
        
    }
    crypt::CryptEngine::GetInstance().XorDecrypt(buff);
        
    switch(buff->Get<uint8>()) {
    case 0x01:
        PacketClient::AuthAccount(buff, m_loginServerAccounts[session]);
        break;
    case 0x03:
        PacketClient::UserLogin(buff, m_loginServerAccounts[session]);
        break;
    case 0x59:
        PacketClient::Ping(buff, m_loginServerAccounts[session]);
        break;
    }
}

bool LoginServer::ConnectClient(network::SocketSession_ptr session) {
    session->SetPacketProcessCallBack(boost::bind(&LoginServer::PacketProcess, this, _1, _2));
    Account* acc = new Account;
    acc->SetSocketSession(session);
    acc->SetServer(this);
    m_loginServerAccounts[session] = acc;
    return true;
}

}
}