#ifndef CONFLOAD_H
#define	CONFLOAD_H

#include <boost/static_assert.hpp>

#include "iniparser/iniparser.h"
#include "shared/typedef.h"
#include "shared/utils.h"

#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

namespace jRylServer {
namespace common {
namespace shared {

class ConfLoad {
public:
    ConfLoad();
    bool LoadConfig(const char* file);
    virtual ~ConfLoad();
    bool GetBool(const char* section, const char* key);
    char* GetString(const char* section, const char* key);
    template<class T> bool GetInt(const char* section, const char* key, T& et);
	template<class T> bool GetUInt(const char* section, const char* key, T& ret);
	template<class T> bool GetFloat(const char* section, const char* key, T& ret);
private:
    dictionary *m_dic;
};

template<class T> bool ConfLoad::GetInt(const char* section, const char* key, T& ret) {
	if (m_dic == NULL) {
        return false;
    }
	char* str;
	char seckey[1024];
	sprintf(seckey, "%s:%s", section, key);
	if(str = iniparser_getstring(m_dic, seckey, NULL)) {
		ret = j_atoi<T>(str);
		return true;
	}
	return false;
}

template<class T> bool ConfLoad::GetUInt(const char* section, const char* key, T& ret) {
	if (m_dic == NULL) {
        return false;
    }
	char* str;
	char seckey[1024];
	sprintf(seckey, "%s:%s", section, key);
	if(str = iniparser_getstring(m_dic, seckey, NULL)) {
		ret = j_atoui<T>(str);
		return true;
	}
	return false;
}

template<class T> bool ConfLoad::GetFloat(const char* section, const char* key, T& ret) {
	if (m_dic == NULL) {
        return false;
    }
	char* str;
	char seckey[1024];
	sprintf(seckey, "%s:%s", section, key);
	if(str = iniparser_getstring(m_dic, seckey, NULL)) {
		ret = j_atof<T>(str);
		return true;
	}
	return false;
}

} //namespace shared
} //namespace common
} //namespace jRylServer
#endif	/* CONFLOAD_H */
