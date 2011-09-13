#ifndef IDBROW_H
#define IDBROW_H

#include <boost/function.hpp>

#include "shared/database/cDBResult.h"
#include "shared/database/cDBQuery.h"

#include <string>

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

class DBConnection {
public:
    virtual bool StartTransaction() = 0;
    virtual bool Commit() = 0;
    virtual void FreeResult(DBResult* result) = 0;
    virtual DBResult* QueryExecute(DBQuery* query) = 0;
    virtual int GetLastErrorCode() = 0;
    virtual char* GetErrorDescription(int error) = 0;
    virtual bool ScapeString(std::string& string) = 0;
    virtual void Ping() = 0;

    bool IsTemp() {
        return m_isTemp;
    }

    void SetIsTemp(bool isTemp) {
        m_isTemp = isTemp;
    }
    
    bool InUse() {
        return m_inUse;
    }

    void SetInUse(bool inUse) {
        m_inUse = inUse;
    }
private:
    bool m_isTemp;
    bool m_inUse;
};

}//namespace database
}//namespace shared
}//namespace common
}//namespace jRylServer

#endif