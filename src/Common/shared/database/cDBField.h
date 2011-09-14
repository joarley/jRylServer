#ifndef DBFIELD_H
#define DBFIELD_H

#include "shared/typedef.h"

#include <string>

using namespace std;

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

class DBField {
public:
    enum FieldType {
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

    char* ToString();
    f64 ToFloat64();
    f32 ToFloat32();
    int64 ToInt64();
    int32 ToInt32();
    int16 ToInt16();
    int8 ToInt8();
    uint64 ToUInt64();
    uint32 ToUInt32();
    uint16 ToUInt16();
    uint8 ToUInt8();
    byte* ToByte();
    bool ToBool();

    size_t GetLenght();
    FieldType GetType();

    void SetValue(void* value);
    void SetValueLength(size_t valueLength);
    void SetType(FieldType type);

    DBField(){}
    ~DBField(){delete[] m_value; }
private:
    void* m_value;
    size_t m_valueLength;
    FieldType m_type;
};

}//namespace database
}//namespace shared
}//namespace common
}//namespace jRylServer

#endif