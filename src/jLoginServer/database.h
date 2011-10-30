#ifndef DATABASE_H
#define DATABASE_H
#include "shared/database/cDBMgr.h"
#include "shared/cLogger.h"

#include <boost/any.hpp>
#include <map>
#include <cstring>
#include <string>
#include <vector>

using namespace std;
using namespace boost;

namespace jRylServer {
namespace jLoginServer {

typedef map<string,any> RESULT;
typedef vector<RESULT> RESULT_SET;

#define DB_CAST any_cast

enum DB_STATUS { 
    DS_SUCESS,
    DS_ERROR,
    DS_NO_DATA
};

DB_STATUS authenticate_account(const char* user, const char* pass, RESULT& out);

DB_STATUS load_characters_view(uint32 accountId, RESULT_SET& out);
DB_STATUS load_equips_view(uint32 charinfoid, RESULT_SET& out);

}
}
#endif