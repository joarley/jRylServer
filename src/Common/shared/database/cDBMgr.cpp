#include "cDBMgr.h"

#include "soci/soci_soci.h"
#include "soci/mysql/mysql_soci-mysql.h"
#include "soci/postgresql/postgresql_soci-postgresql.h"
#include "soci/sqlite3/sqlite3_soci-sqlite3.h"

#include <string>

namespace jRylServer {
namespace common {
namespace shared {
namespace database {


bool DBMgr::Start() {
}

void DBMgr::Stop() {
}

void DBMgr::SetConnectionDetails(DataBaseType type, const char* host, const char* port,	const char* user, const char* pass, const char* db) {
}

void DBMgr::SetConnectionDetails(DataBaseType type, const char* file) {
}
    
void DBMgr::SetSizePool(int size) {
}

soci::session* DBMgr::CreateSession() {
}

soci::session* DBMgr::CreateSession(DataBaseType type, const char* host, const char* port, const char* user, const char* pass, const char* db) {

}

soci::session* DBMgr::CreateSession(DBMgr::DataBaseType type, const char* file) {
}

void DBMgr::FreeSession(soci::session* s) {
}
	
DBMgr::DataBaseType DBMgr::GetDataBaseType() {
}

} //namespace database
} //namespace shared
} //namespace common
} //namespace jRylServer
