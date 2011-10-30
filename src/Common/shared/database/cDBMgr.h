#ifndef DBMGR_H
#define DBMGR_H

#define DB_MYSQL_ONLY

#include "shared/iSingleton.h"
#include "shared/typedef.h"

#include <vector>
#include <string>

#if defined(DB_MYSQL_ONLY) || defined(DB_ALL_BACKEND)
#include <mysql.h>
#include "shared/database/helpers_mysql.h"
#endif

#if defined(DB_SQLITE3_ONLY) || defined(DB_ALL_BACKEND)
#include "sqlite3/sqlite3.h"
#endif

namespace jRylServer {
namespace common {
namespace shared {
namespace database {



class DBMgr: public Singleton<DBMgr> {
    MAKE_SINGLETON(DBMgr);
public:
    enum DB_Type {
        DB_UNSELECTED,
        DB_MYSQL,
        DB_SQLITE3
    };
    DBMgr();
    ~DBMgr();
    bool Start();
    void Stop();

    inline DB_Type GetDbType();
private:
    int m_PoolSize;
    DB_Type m_dbType;
//--------------------------Mysql-------------------------------
#if defined(DB_MYSQL_ONLY) || defined(DB_ALL_BACKEND)
public:
    struct MyConnection {
        friend class DBMgr;
        MYSQL Conn;
    private:
        bool IsTemp;
         bool IsBusy;
    };

    MyConnection* MyGetConnection();
    void MyFreeConnection(MyConnection* conn);
    bool MySetParans(const char* host, uint port, const char* user, const char* pass, const char* db, int poolsize);
private:
    bool MyStart();
    void MyStop();

    std::string m_myHost;
    std::string m_myUser;
    std::string m_myPass;
    std::string m_myDb;
    uint m_myPort;
    std::vector<MyConnection*> m_MyConnections;
#endif


//--------------------------Sqlite-------------------------------
#if defined(DB_SQLITE3_ONLY) || defined(DB_ALL_BACKEND)
public:
    struct LiteConnection {
        friend class DBMgr;
        sqlite3* Conn;
    private:
        bool IsTemp; 
        bool IsBusy;
    };

    LiteConnection* LiteGetSqConnection();
    void MyFreeConnection(LiteConnection* conn);
    bool LiteSetParans(const char* dbFile, int poolsize);
private:
    bool LiteStart();
    void LiteStop();

    std::string m_liteDbFile;
    std::vector<LiteConnection*> m_liteConnections;
#endif
};


inline DBMgr::DB_Type DBMgr::GetDbType() {
    return m_dbType;
}

} //namespace database
} //namespace shared
} //namespace common
} //namespace jRylServer

#endif