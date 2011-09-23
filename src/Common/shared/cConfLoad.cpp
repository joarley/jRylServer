#include "cConfLoad.h"
#include <string>
#include "iniparser/iniparser.h"

namespace jRylServer {
namespace common {
namespace shared {

ConfLoad::ConfLoad() {
    m_dic = NULL;
}

bool ConfLoad::LoadConfig(const char* file) {
    m_dic = iniparser_load((char*)file);
    if (m_dic == NULL) {
        return false;
    }
    return true;
}

bool ConfLoad::GetBool(const char* section, const char* key) {
	char seckey[1024];
	sprintf(seckey, "%s:%s", section, key);
	if (m_dic == NULL || !iniparser_getboolean(m_dic, seckey, 0)) {
        return false;
    } else {
        return true;
    }
}

char* ConfLoad::GetString(const char* section, const char* key) {
    if (m_dic == NULL) {
        return NULL;
    }
	char seckey[1024];
	sprintf(seckey, "%s:%s", section, key);
	return iniparser_getstring(m_dic, seckey, NULL);
}

ConfLoad::~ConfLoad() {
    if (m_dic) {
        iniparser_freedict(m_dic);
    }
}
} //namespace shared
} //namespace common
} //namespace jRylServer
