#ifndef IDBADAPTOR_H
#define IDBADAPTOR_H

#include "shared/database/iDBConnection.h"

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

class DBAdaptor {
public:
    virtual DBConnection* GetConnection() = 0;
    virtual void FreeConnection(DBConnection* connection) = 0;
};

}//namespace database
}//namespace shared
}//namespace common
}//namespace jRylServer

#endif