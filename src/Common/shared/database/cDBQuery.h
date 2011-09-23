#ifndef DBQUERY_H
#define DBQUERY_H

#include <string>
#include <vector>

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

class Param {
public:
	enum ParamType {
        FT_STRING,
        FT_FLOAT64,
        FT_FLOAT32,
        FT_INT64,
        FT_INT32,
        FT_INT16,
        FT_INT8,
        FT_UINT64,
        FT_UINT32,
        FT_UINT16,
        FT_UINT8,
        FT_BOOL,
        FT_BYTE
    };

    Param() {
        m_name = NULL;
        m_value = NULL;
    }

    ~Param() {
        if(m_name != NULL) {
            delete[] m_name;
        }

        if(m_value != NULL) {
            delete[] m_value;
        }
    }

    char* GetName() {
		return m_name;
	}
	
	void* GetValue() {
		return m_value;
	}
	
	void SetName(char* name) {
		
	}

    void SetValue(void* value) {
	
	}
    
private:
    char* m_name;
    void* m_value;
};

class DBQuery {
public:
    DBQuery(char* queryText);
    DBQuery();
    char* GetText();
    void SetText(const char* text);
    std::vector<Param*> Params();
    Param* GetParamByName(char* name);
private:
    std::vector<Param*> m_params;
    char *m_text;
};

}//namespace database
}//namespace shared
}//namespace common
}//namespace jRylServer

#endif