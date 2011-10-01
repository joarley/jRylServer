#include "cDBMgr.h"

#include "shared/cLogger.h"

#include "soci/soci_soci.h"
#include "soci/mysql/mysql_soci-mysql.h"
#include "soci/postgresql/postgresql_soci-postgresql.h"
#include "soci/sqlite3/sqlite3_soci-sqlite3.h"

#include <string>

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

using namespace std;

bool DBMgr::Start() {
    if(m_poolSize == 0) {
        Logger::GetInstance().ShowError("[DBMgr] PoolSize incorrectly configured.\n");
        return false;
    }
    try{        
        soci::register_factory_mysql();
        soci::register_factory_postgresql();
        soci::register_factory_sqlite3();
        
        m_pool = new soci::connection_pool(m_poolSize);
        for (size_t i = 0; i != m_poolSize; ++i)
        {
            soci::session & sql = m_pool->at(i);

            sql.open(m_connectionString);
        }
    } catch(exception e) {
        Logger::GetInstance().ShowError("[DBMgr] %s\n", e.what());
        return false;
    }
    
    return true;
}

void DBMgr::Stop() {
    delete m_pool;
}

void DBMgr::SetConnectionDetails(DataBaseType type, const char* host, const char* port,	const char* user, const char* pass, const char* db) {
    if(type == DB_mysql) {
        m_connectionString = "mysql://host=";
        m_connectionString += host;
        m_connectionString += " port=";
        m_connectionString += port;
        m_connectionString += " user=";
        m_connectionString += user;
        m_connectionString += " pass=";
        m_connectionString += pass;
        m_connectionString += " db=";
        m_connectionString += db;
    } else if(type == DB_postgresql) {
        m_connectionString = "postgresql://host=";
        m_connectionString += host;
        m_connectionString += " port=";
        m_connectionString += port;
        m_connectionString += " user=";
        m_connectionString += user;
        m_connectionString += " password=";
        m_connectionString += pass;
        m_connectionString += " dbname=";
        m_connectionString += db;
    }
    m_type = type;
}

void DBMgr::SetConnectionDetails(DataBaseType type, const char* file) {
    if(type == DB_sqlite3) {
         m_connectionString = "sqlite3://";
        m_connectionString += file;
    }
}
    
void DBMgr::SetSizePool(int size) {
    m_poolSize = size;
}

soci::session* DBMgr::CreateSession() {
    soci::session* s;
    try {
        s = new soci::session(*m_pool);
    } catch(exception e) {
        delete s;
        Logger::GetInstance().ShowError("[DBMgr] %s", e.what());
        return NULL;
    }
    return s;
}

soci::session* DBMgr::CreateSession(DataBaseType type, const char* host, const char* port, const char* user, const char* pass, const char* db) {
    string connectionString;
    soci::session* s;
    if(type == DB_mysql) {
        connectionString = "mysql://host=";
        connectionString += host;
        connectionString += "/port=";
        connectionString += port;
        connectionString += "/user=";
        connectionString += user;
        connectionString += "/pass=";
        connectionString += pass;
        connectionString += "/db=";
        connectionString += db;
    } else if(type == DB_postgresql) {
        connectionString = "postgresql://hostaddr=";
        connectionString += host;
        connectionString += "/port=";
        connectionString += port;
        connectionString += "/user=";
        connectionString += user;
        connectionString += "/password=";
        connectionString += pass;
        connectionString += "/dbname=";
        connectionString += db;
    } else {
        return NULL;
    }

    try {
        s = new soci::session(connectionString);
    } catch(exception e) {
        Logger::GetInstance().ShowError("[DBMgr] %s", e.what());
        return NULL;
    }
    return s;
}

soci::session* DBMgr::CreateSession(DBMgr::DataBaseType type, const char* file) {
    string connectionString;
    soci::session* s;
    if(type == DB_sqlite3) {
        connectionString = "sqlite3://";
        connectionString += file;
    } else {
        return NULL;
    }

    try {
        s = new soci::session(connectionString);
    } catch(exception e) {
        Logger::GetInstance().ShowError("[DBMgr] %s", e.what());
        return NULL;
    }
    return s;
}

void DBMgr::FreeSession(soci::session* s) {
    delete s;
}
	
DBMgr::DataBaseType DBMgr::GetDataBaseType() {
    return m_type;
}

} //namespace database
} //namespace shared
} //namespace common
} //namespace jRylServer
