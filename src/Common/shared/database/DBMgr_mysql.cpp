#include "shared/database/cDBMgr.h"

#include "shared/cLogger.h"
#include "shared/typedef.h"

#include <vector>

using namespace std;

#if defined(DB_MYSQL_ONLY) || defined(DB_ALL_BACKEND)
#include <mysql.h>

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

DBMgr::MyConnection* DBMgr::MyGetConnection() {
    MyConnection* ret = NULL;
    vector<MyConnection*>::iterator it = m_MyConnections.begin();
    while(it != m_MyConnections.end()) {
        if(!(*it)->IsBusy) {
            ret = *it;
            ret->IsBusy = true;
            break;
        }
        it++;
    }
    if(ret == NULL) {
        ret = new MyConnection;
        if(mysql_real_connect(&ret->Conn, m_myHost.c_str(), m_myUser.c_str(), m_myPass.c_str(),
                                m_myDb.c_str(), m_myPort, NULL, NULL) == NULL) {
            Logger::GetInstance().ShowError("Error create temp database connection: [%d] %s", mysql_errno(&ret->Conn), mysql_error(&ret->Conn));
            delete ret;
            return NULL;
        }
        ret->IsTemp = true;
    }
    return ret;
}

void DBMgr::MyFreeConnection(MyConnection* conn) {
    if(conn->IsTemp) {
        mysql_close(&conn->Conn);
        delete conn;
    } else {
        conn->IsBusy = false;
    }
}

bool DBMgr::MySetParans(const char* host, uint port, const char* user, const char* pass, const char* db, int poolsize) {
    if(m_dbType != DB_UNSELECTED) {
        Logger::GetInstance().ShowError("Database Engine already configured.\n");
        return false;
    }

    m_myHost = host;
    m_myPort = port;
    m_myUser = user;
    m_myPass = pass;
    m_myDb = db;
    m_PoolSize = poolsize;

    m_dbType = DB_MYSQL;
    return true;
}

bool DBMgr::MyStart() {
    if(m_PoolSize < 0) {
        Logger::GetInstance().ShowError("DataBase pool not set.\n");
        return false;
    }
    m_MyConnections.resize(m_PoolSize);
    vector<MyConnection*>::iterator it = m_MyConnections.begin();
    while(it != m_MyConnections.end()) {
        MyConnection* conn = new MyConnection;
        conn->IsBusy = false;
        conn->IsTemp = false;
        mysql_init(&conn->Conn);
        if(mysql_real_connect(&conn->Conn, m_myHost.c_str(), m_myUser.c_str(), m_myPass.c_str(),
                                m_myDb.c_str(), m_myPort, NULL, NULL) == NULL) {
            Logger::GetInstance().ShowError("Error connect database: [%d] %s", mysql_errno(&conn->Conn), mysql_error(&conn->Conn));
            return false;
        }
        *it = conn;
        it++;
    }
    return true;
}

void DBMgr::MyStop() {
    for(uint i = 0; i < m_MyConnections.size(); i++) {
        mysql_close(&m_MyConnections[i]->Conn);
        delete m_MyConnections[i];
        m_MyConnections[i] = NULL;
    }
}

} //namespace database
} //namespace shared
} //namespace common
} //namespace jRylServer

#endif