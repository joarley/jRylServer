#include "shared/database/cDBMgr.h"

#include "shared/cLogger.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

DBMgr::DBMgr() {
    m_dbType = DB_UNSELECTED;
}

DBMgr::~DBMgr() {
    
}

bool DBMgr::Start() {
#if defined(DB_MYSQL_ONLY) || defined(DB_ALL_BACKEND)
    if(m_dbType == DB_MYSQL) {
        return MyStart();
    }
#endif
#if defined(DB_SQLITE3_ONLY) || defined(DB_ALL_BACKEND)
    if(m_dbType == DB_SQLITE3) {
        return LiteStart();
    }
#endif
    Logger::GetInstance().ShowError("incorrect initialization DataBase engine.\n");
    return false;
}

void DBMgr::Stop() {
#if defined(DB_MYSQL_ONLY) || defined(DB_ALL_BACKEND)
    if(m_dbType == DB_MYSQL) {
        return MyStop();
    }
#endif
#if defined(DB_SQLITE3_ONLY) || defined(DB_ALL_BACKEND)
    if(m_dbType == DB_SQLITE3) {
        return LiteStop();
    }
#endif
}

} //namespace database
} //namespace shared
} //namespace common
} //namespace jRylServer