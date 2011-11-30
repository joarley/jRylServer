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

bool ConfLoad::GetBool(const char* section, const char* key, bool &ret) {
	if (m_dic == NULL) {
        return false;
    }
    char seckey[1024];
	sprintf(seckey, "%s:%s", section, key);
    int aux = iniparser_getboolean(m_dic, seckey, -1);
    if(aux == -1) {
        return false;    
    }
    ret = (bool)aux;
    return true;
    
}

bool ConfLoad::GetString(const char* section, const char* key, string &ret) {
    if (m_dic == NULL) {
        return false;
    }
	char seckey[1024];
	sprintf(seckey, "%s:%s", section, key);
	char* aux = iniparser_getstring(m_dic, seckey, NULL);
    if(aux == NULL) {
        return false;
    }
    ret = aux;
    return true;
}

 int ConfLoad::GetBytes(const char* section, const char* key, byte* ret) {
    if (m_dic == NULL) {
        return -1;
    }
    char seckey[1024];
	sprintf(seckey, "%s:%s", section, key);
	char* aux = iniparser_getstring(m_dic, seckey, NULL);
    if(aux == NULL) {
        return -1;
    }
    return j_parseHexSeq(aux, ret);
 }

ConfLoad::~ConfLoad() {
    if (m_dic) {
        iniparser_freedict(m_dic);
    }
}
} //namespace shared
} //namespace common
} //namespace jRylServer
