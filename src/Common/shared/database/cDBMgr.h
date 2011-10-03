#ifndef CDBMGR_H
#define CDBMGR_H
#include "shared/iSingleton.h"

#include "soci/soci_soci.h"
#include "soci/mysql/mysql_soci-mysql.h"
#include "soci/postgresql/postgresql_soci-postgresql.h"
#include "soci/sqlite3/sqlite3_soci-sqlite3.h"

#include <string>

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
    void SetConnectionDetails(DataBaseType type,
		const char* host,
		const char* port,
		const char* user,
		const char* pass,
		const char* db);
	void SetConnectionDetails(DataBaseType type,
		const char* file);
    
	void SetSizePool(int size);

	soci::session* CreateSession();
	soci::session* CreateSession(DataBaseType type,
		const char* host,
		const char* port,
		const char* user,
		const char* pass,
		const char* db);
	soci::session* CreateSession(DataBaseType type,
		const char* file);

	void FreeSession(soci::session* s);
	
	DataBaseType GetDataBaseType();
private:
    DBMgr(): m_poolSize(0){};
    DataBaseType m_type;
    std::string m_connectionString;
    int m_poolSize;
    soci::connection_pool* m_pool;
};

} //namespace database
} //namespace shared
} //namespace common
} //namespace jRylServer


#endif