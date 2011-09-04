#include "cConfLoad.h"
#include <string>
#include "iniparser/iniparser.h"

namespace jRylServer {
namespace common {
namespace shared {

ConfLoad::ConfLoad() {
    m_dic = NULL;
}

bool ConfLoad::LoadConfig(std::string file) {
    m_dic = iniparser_load((char *) file.c_str());
    if (m_dic == NULL) {
        return false;
    }
    return true;
}

bool ConfLoad::GetBool(std::string section, std::string key) {
    if (m_dic == NULL || !iniparser_getboolean(m_dic, (char*) (section + ":" + key).c_str(), 0)) {
        return false;
    } else {
        return true;
    }
}

string ConfLoad::GetString(std::string section, std::string key) {
    string stret;
    if (m_dic == NULL) {
        return stret;
    }
    char* st = iniparser_getstring(m_dic, (char*) (section + ":" + key).c_str(), NULL);
    if (st == NULL) {
        return 0;
    }
    stret = st;
    return stret;
}

int64 ConfLoad::GetInt64(std::string section, std::string key) {
    if (m_dic == NULL) {
        return 0;
    }
    char* st = iniparser_getstring(m_dic, (char*) (section + ":" + key).c_str(), NULL);
    if (st == NULL) {
        return 0;
    }
    int64 value = j_atoi<int64>(st);
    return value;
}

int32 ConfLoad::GetInt32(std::string section, std::string key) {
    if (m_dic == NULL) {
        return 0;
    }
    char* st = iniparser_getstring(m_dic, (char*) (section + ":" + key).c_str(), NULL);
    if (st == NULL) {
        return 0;
    }
    int32 value = j_atoi<int32>(st);
    return value;
}

int16 ConfLoad::GetInt16(std::string section, std::string key) {
    if (m_dic == NULL) {
        return 0;
    }
    char* st = iniparser_getstring(m_dic, (char*) (section + ":" + key).c_str(), NULL);
    if (st == NULL) {
        return 0;
    }
    int16 value = j_atoi<int16>(st);
    return value;
}

int8 ConfLoad::GetInt8(std::string section, std::string key) {
    if (m_dic == NULL) {
        return 0;
    }
    char* st = iniparser_getstring(m_dic, (char*) (section + ":" + key).c_str(), NULL);
    if (st == NULL) {
        return 0;
    }
    int8 value = j_atoi<int8>(st);
    return value;
}

ConfLoad::~ConfLoad() {
    if (m_dic) {
        iniparser_freedict(m_dic);
    }
}
} //namespace shared
} //namespace common
} //namespace jRylServer
