#ifndef CONFLOAD_H
#define	CONFLOAD_H
#include <string>

#include <boost/static_assert.hpp>

#include "iniparser/iniparser.h"
#include "shared/typedef.h"
#include "shared/utils.h"

using namespace std;

namespace jRylServer {
namespace common {
namespace shared {

class ConfLoad {
public:
    ConfLoad();
    bool LoadConfig(std::string file);
    virtual ~ConfLoad();
    bool GetBool(std::string section, std::string key);
    string GetString(std::string section, std::string key);
    int64 GetInt64(std::string section, std::string key);
    int32 GetInt32(std::string section, std::string key);
    int16 GetInt16(std::string section, std::string key);
    int8 GetInt8(std::string section, std::string key);
private:
    dictionary *m_dic;
};
} //namespace shared
} //namespace common
} //namespace jRylServer
#endif	/* CONFLOAD_H */

