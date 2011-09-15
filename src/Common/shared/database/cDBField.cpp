#include "cDBField.h"

#include "shared/typedef.h"
#include "shared/utils.h"

#include <string>
#include <ctime>
#include <cstdio>

using namespace std;

namespace jRylServer {
namespace common {
namespace shared {
namespace database {

char* DBField::ToString() {
    char* result;
    switch (m_type) {
        case FT_STRING:
            result = new char[m_valueLength];
            memcpy(result, m_value, m_valueLength);
            break;
        case FT_FLOAT64:
            result = j_ftoa(*(f64*) m_value);
            break;
        case FT_FLOAT32:
            result = j_ftoa(*(f32*) m_value);
            break;
        case FT_INT64:
            result = j_itoa(*(int64*) m_value);
            break;
        case FT_INT32:
            result = j_itoa(*(int32*) m_value);
            break;
        case FT_INT16:
            result = j_itoa(*(int16*) m_value);
            break;
        case FT_INT8:
            result = j_itoa(*(int8*) m_value);
            break;
        case FT_UINT64:
            result = j_itoa(*(uint64*) m_value);
            break;
        case FT_UINT32:
            result = j_itoa(*(uint32*) m_value);
            break;
        case FT_UINT16:
            result = j_itoa(*(uint16*) m_value);
            break;
        case FT_UINT8:
            result = j_itoa(*(uint8*) m_value);
            break;
        case FT_BOOL:
            if (*(int8*) m_value) {
                result = new char[strlen("true")];
                strcpy(result, "true");
            } else {
                result = new char[strlen("false")];
                strcpy(result, "false");
            }
            break;
        case FT_BYTE:
            result = (char*) new byte[m_valueLength];
            memcpy(result, m_value, m_valueLength);
            break;
        default:
            return NULL;
    }

    return result;
}

f64 DBField::ToFloat64() {
    switch (m_type) {
        case FT_STRING:
            return j_atof<f64 > ((char*) m_value);
        case FT_FLOAT64:
            return *(f64*) m_value;
        case FT_FLOAT32:
            return *(f32*) m_value;
        case FT_INT64:
            return (f64)*(int64*) m_value;
        case FT_INT32:
            return (f64)*(int32*) m_value;
        case FT_INT16:
            return (f64)*(int16*) m_value;
        case FT_INT8:
            return (f64)*(int8*) m_value;
        case FT_UINT64:
            return (f64)*(uint64*) m_value;
        case FT_UINT32:
            return (f64)*(uint32*) m_value;
        case FT_UINT16:
            return (f64)*(uint16*) m_value;
        case FT_UINT8:
            return (f64)*(uint8*) m_value;
        case FT_BOOL:
            if (*(int8*) m_value) {
                return 1.0;
            } else {
                return 0.0;
            }
        default:
            return 0.0;
    }
}

f32 DBField::ToFloat32() {
    switch (m_type) {
        case FT_STRING:
            return j_atof<f32 > ((char*) m_value);
        case FT_FLOAT64:
            return (f32)*(f64*) m_value;
        case FT_FLOAT32:
            return *(f32*) m_value;
        case FT_INT64:
            return (f32)*(int64*) m_value;
        case FT_INT32:
            return (f32)*(int32*) m_value;
        case FT_INT16:
            return (f32)*(int16*) m_value;
        case FT_INT8:
            return (f32)*(int8*) m_value;
        case FT_UINT64:
            return (f32)*(uint64*) m_value;
        case FT_UINT32:
            return (f32)*(uint32*) m_value;
        case FT_UINT16:
            return (f32)*(uint16*) m_value;
        case FT_UINT8:
            return (f32)*(uint8*) m_value;
        case FT_BOOL:
            if (*(int8*) m_value) {
                return 1.0;
            } else {
                return 0.0;
            }
        default:
            return 0.0;
    }
}

int64 DBField::ToInt64() {
    switch (m_type) {
        case FT_STRING:
            return j_atof<int64 > ((char*) m_value);
        case FT_FLOAT64:
            return (int64)*(f64*) m_value;
        case FT_FLOAT32:
            return (int64)*(f32*) m_value;
        case FT_INT64:
            return *(int64*) m_value;
        case FT_INT32:
            return (int64)*(int32*) m_value;
        case FT_INT16:
            return (int64)*(int16*) m_value;
        case FT_INT8:
            return (int64)*(int8*) m_value;
        case FT_UINT64:
            return (int64)*(uint64*) m_value;
        case FT_UINT32:
            return (int64)*(uint32*) m_value;
        case FT_UINT16:
            return (int64)*(uint16*) m_value;
        case FT_UINT8:
            return (int64)*(uint8*) m_value;
        case FT_BOOL:
            if (*(int8*) m_value) {
                return 1;
            } else {
                return 0;
            }
        default:
            return 0;
    }
}

int32 DBField::ToInt32() {
    switch (m_type) {
        case FT_STRING:
            return j_atof<int32 > ((char*) m_value);
        case FT_FLOAT64:
            return (int32)*(f64*) m_value;
        case FT_FLOAT32:
            return (int32)*(f32*) m_value;
        case FT_INT64:
            return (int32)*(int64*) m_value;
        case FT_INT32:
            return *(int32*) m_value;
        case FT_INT16:
            return (int32)*(int16*) m_value;
        case FT_INT8:
            return (int32)*(int8*) m_value;
        case FT_UINT64:
            return (int32)*(uint64*) m_value;
        case FT_UINT32:
            return (int32)*(uint32*) m_value;
        case FT_UINT16:
            return (int32)*(uint16*) m_value;
        case FT_UINT8:
            return (int32)*(uint8*) m_value;
        case FT_BOOL:
            if (*(int8*) m_value) {
                return 1;
            } else {
                return 0;
            }
        default:
            return 0;
    }
}

int16 DBField::ToInt16() {
    switch (m_type) {
        case FT_STRING:
            return j_atof<int16 > ((char*) m_value);
        case FT_FLOAT64:
            return (int16)*(f64*) m_value;
        case FT_FLOAT32:
            return (int16)*(f32*) m_value;
        case FT_INT64:
            return (int16)*(int64*) m_value;
        case FT_INT32:
            return (int16)*(int32*) m_value;
        case FT_INT16:
            return *(int16*) m_value;
        case FT_INT8:
            return (int16)*(int8*) m_value;
        case FT_UINT64:
            return (int16)*(uint64*) m_value;
        case FT_UINT32:
            return (int16)*(uint32*) m_value;
        case FT_UINT16:
            return (int16)*(uint16*) m_value;
        case FT_UINT8:
            return (int16)*(uint8*) m_value;
        case FT_BOOL:
            if (*(int8*) m_value) {
                return 1;
            } else {
                return 0;
            }
        default:
            return 0;
    }
}

int8 DBField::ToInt8() {
    switch (m_type) {
        case FT_STRING:
            return j_atof<int8 > ((char*) m_value);
        case FT_FLOAT64:
            return (int8)*(f64*) m_value;
        case FT_FLOAT32:
            return (int8)*(f32*) m_value;
        case FT_INT64:
            return (int8)*(int64*) m_value;
        case FT_INT32:
            return (int8)*(int32*) m_value;
        case FT_INT16:
            return (int8)*(int16*) m_value;
        case FT_INT8:
            return *(int8*) m_value;
        case FT_UINT64:
            return (int8)*(uint64*) m_value;
        case FT_UINT32:
            return (int8)*(uint32*) m_value;
        case FT_UINT16:
            return (int8)*(uint16*) m_value;
        case FT_UINT8:
            return (int8)*(uint8*) m_value;
        case FT_BOOL:
            if (*(int8*) m_value) {
                return 1;
            } else {
                return 0;
            }
        default:
            return 0;
    }
}

uint64 DBField::ToUInt64() {
    switch (m_type) {
        case FT_STRING:
            return j_atof<uint64 > ((char*) m_value);
        case FT_FLOAT64:
            return (uint64)*(f64*) m_value;
        case FT_FLOAT32:
            return (uint64)*(f32*) m_value;
        case FT_INT64:
            return (uint64)*(int64*) m_value;
        case FT_INT32:
            return (uint64)*(int32*) m_value;
        case FT_INT16:
            return (uint64)*(int16*) m_value;
        case FT_INT8:
            return (uint64)*(int8*) m_value;
        case FT_UINT64:
            return *(uint64*) m_value;
        case FT_UINT32:
            return (uint64)*(uint32*) m_value;
        case FT_UINT16:
            return (uint64)*(uint16*) m_value;
        case FT_UINT8:
            return (uint64)*(uint8*) m_value;
        case FT_BOOL:
            if (*(int8*) m_value) {
                return 1;
            } else {
                return 0;
            }
        default:
            return 0;
    }
}

uint32 DBField::ToUInt32() {
    switch (m_type) {
        case FT_STRING:
            return j_atof<uint32 > ((char*) m_value);
        case FT_FLOAT64:
            return (uint32)*(f64*) m_value;
        case FT_FLOAT32:
            return (uint32)*(f32*) m_value;
        case FT_INT64:
            return (uint32)*(int64*) m_value;
        case FT_INT32:
            return (uint32)*(int32*) m_value;
        case FT_INT16:
            return (uint32)*(int16*) m_value;
        case FT_INT8:
            return (uint32)*(int8*) m_value;
        case FT_UINT64:
            return (uint32)*(uint64*) m_value;
        case FT_UINT32:
            return *(uint32*) m_value;
        case FT_UINT16:
            return (uint32)*(uint16*) m_value;
        case FT_UINT8:
            return (uint32)*(uint8*) m_value;
        case FT_BOOL:
            if (*(int8*) m_value) {
                return 1;
            } else {
                return 0;
            }
        default:
            return 0;
    }
}

uint16 DBField::ToUInt16() {
    switch (m_type) {
        case FT_STRING:
            return j_atof<uint16 > ((char*) m_value);
        case FT_FLOAT64:
            return (uint16)*(f64*) m_value;
        case FT_FLOAT32:
            return (uint16)*(f32*) m_value;
        case FT_INT64:
            return (uint16)*(int64*) m_value;
        case FT_INT32:
            return (uint16)*(int32*) m_value;
        case FT_INT16:
            return (uint16)*(int16*) m_value;
        case FT_INT8:
            return (uint16)*(int8*) m_value;
        case FT_UINT64:
            return (uint16)*(uint64*) m_value;
        case FT_UINT32:
            return (uint16)*(uint32*) m_value;
        case FT_UINT16:
            return *(uint16*) m_value;
        case FT_UINT8:
            return (uint16)*(uint8*) m_value;
        case FT_BOOL:
            if (*(int8*) m_value) {
                return 1;
            } else {
                return 0;
            }
        default:
            return 0;
    }
}

uint8 DBField::ToUInt8() {
    switch (m_type) {
        case FT_STRING:
            return j_atof<uint8 > ((char*) m_value);
        case FT_FLOAT64:
            return (uint8)*(f64*) m_value;
        case FT_FLOAT32:
            return (uint8)*(f32*) m_value;
        case FT_INT64:
            return (uint8)*(int64*) m_value;
        case FT_INT32:
            return (uint8)*(int32*) m_value;
        case FT_INT16:
            return (uint8)*(int16*) m_value;
        case FT_INT8:
            return (uint8)*(int8*) m_value;
        case FT_UINT64:
            return (uint8)*(uint64*) m_value;
        case FT_UINT32:
            return (uint8)*(uint32*) m_value;
        case FT_UINT16:
            return (uint8)*(uint16*) m_value;
        case FT_UINT8:
            return *(uint8*) m_value;
        case FT_BOOL:
            if (*(int8*) m_value) {
                return 1;
            } else {
                return 0;
            }
        default:
            return 0;
    }
}

byte* DBField::ToByte() {
    return m_value;
}

bool DBField::ToBool() {
    switch (m_type) {
        case FT_STRING:
            if (((char*) m_value)[0] == 'y' || ((char*) m_value)[0] == 'Y' || ((char*) m_value)[0] == '1' || ((char*) m_value)[0] == 't' || ((char*) m_value)[0] == 'T') {
                return true;
            } else {
                return false;
            }
        case FT_FLOAT64:
        case FT_FLOAT32:
            if (*(f64*) m_value == 0.0) {
                return false;
            } else {
                return true;
            }
        case FT_INT64:
        case FT_INT32:
        case FT_INT16:
        case FT_INT8:
        case FT_UINT64:
        case FT_UINT32:
        case FT_UINT16:
        case FT_UINT8:
            if (*(uint64*) m_value == 0) {
                return false;
            } else {
                return true;
            }
        case FT_BOOL:
            if (*(int8*) m_value == 0) {
                return false;
            } else {
                return true;
            }
        default:
            return false;
    }
}

size_t DBField::GetLenght() {
    return m_valueLength;
}

DBField::FieldType DBField::GetType() {
    return m_type;
}

void DBField::SetValue(byte* value) {
    m_value = value;
}

void DBField::SetValueLength(size_t valueLength) {
    m_valueLength = valueLength;
}

void DBField::SetType(FieldType type) {
    m_type = type;
}


}//namespace database
}//namespace shared
}//namespace common
}//namespace jRylServer
