#ifndef DBQUERY_H
#define DBQUERY_H

#include <string>
#include <vector>

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

class QueryParam {
public:
    QueryParam() {
        m_name = NULL;
        m_value = NULL;
    }

    ~QueryParam() {
        if(m_name != NULL) {
            delete[] m_name;
        }

        if(m_value != NULL) {
            delete[] m_value;
        }
    }

    char* GetName() {};
    void SetName(char* name) {};
    void SetValue(void* value) {};
    void* GetValue() {};
private:
    char* m_name;
    void* m_value;
}

class DBQuery {
public:
    DBQuery(char* queryText);
    DBQuery();
    char* GetText();
    void SetText(const char* text);
    *std::vector<QueryParam*> Params();
    QueryParam* GetParamByName(char* name);
private:
    std::vector<QueryParam*> m_params;
    char *m_text;
};

}//namespace database
}//namespace shared
}//namespace common
}//namespace jRylServer

#endif