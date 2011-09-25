#ifndef CDBMGR_H
#define CDBMGR_H
#include "shared/iSingleton.h"

#include "soci/soci_soci.h"
#include "soci/mysql/mysql_soci-mysql.h"
#include "soci/postgresql/postgresql_soci-postgresql.h"
#include "soci/sqlite3/sqlite3_soci-sqlite3.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

class DBMgr: public Singleton<DBMgr> {
public:
    MAKE_SINGLETON(DBMgr);
    
    enum DataBaseType {
        DB_mysql,
        DB_postgresql,
        DB_sqlite3
    };

    bool Start();
    void Stop();
    void SetConnectionDetails(DataBaseType type);
    void SetSizePool(int size);
private:
    DBMgr(): poolSize(1){};
    DataBaseType m_type;
    char m_connectionString[60];
    int poolSize;
    soci::connection_pool* m_pool;
};

} //namespace database
} //namespace shared
} //namespace common
} //namespace jRylServer


#endif