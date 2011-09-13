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
    switch (m_type) {
        case FT_STRING:
            char* value = new char[m_valueLength];
            memcpy(value, m_value, m_valueLength);
            return value;
        case FT_FLOAT64:
            return j_ftoa(*(f64*) m_value);
        case FT_FLOAT32:
            return j_ftoa(*(f32*) m_value);
        case FT_INT64:
            return j_itoa(*(int64*) m_value);
        case FT_INT32:
            return j_itoa(*(int32*) m_value);
        case FT_INT16:
            return j_itoa(*(int16*) m_value);
        case FT_INT8:
            return j_itoa(*(int8*) m_value);
        case FT_UINT64:
            return j_itoa(*(uint64*) m_value);
        case FT_UINT32:
            return j_itoa(*(uint32*) m_value);
        case FT_UINT16:
            return j_itoa(*(uint16*) m_value);
        case FT_UINT8:
            return j_itoa(*(uint8*) m_value);
            break;
        case FT_BOOL:
            if (*(int*) m_value) {
                char* value = new char[strlen("true")];
                strcpy(value, "true");
                return value;
            } else {
                char* value = new char[strlen("false")];
                strcpy(value, "false");
                return value;
            }
        case FT_BYTE:
            byte* value = new byte[m_valueLength];
            memcpy(value, m_value, m_valueLength);
            return (char*)value;
        default:
            return "";
    }
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
            return *(int64*) m_value;
        case FT_INT32:
            return *(int32*) m_value;
        case FT_INT16:
            return *(int16*) m_value;
        case FT_INT8:
            return *(int8*) m_value;
        case FT_UINT64:
            return *(uint64*) m_value;
        case FT_UINT32:
            return *(uint32*) m_value;
        case FT_UINT16:
            return *(uint16*) m_value;
        case FT_UINT8:
            return *(uint8*) m_value;
            break;
        case FT_BOOL:
            if (*(int*) m_value) {
                return 1;
            } else {
                return 0;
            }
        case FT_BYTE:
            return 0;
        default:
            return 0;
    }
}

f32 DBField::ToFloat32() {
    switch (m_type) {
        case FT_STRING:
            return j_atof<f32 > ((char*) m_value);
        case FT_FLOAT64:
            return 0;
        case FT_FLOAT32:
            return *(f32*) m_value;
        case FT_INT64:
            return 0;
        case FT_INT32:
            return *(int32*) m_value;
        case FT_INT16:
            return *(int16*) m_value;
        case FT_INT8:
            return *(int8*) m_value;
        case FT_UINT64:
            return 0;
        case FT_UINT32:
            return *(uint32*) m_value;
        case FT_UINT16:
            return *(uint16*) m_value;
        case FT_UINT8:
            return *(uint8*) m_value;
            break;
        case FT_BOOL:
            if (*(int*) m_value) {
                return 1;
            } else {
                return 0;
            }
        case FT_BYTE:
            return 0;
        default:
            return 0;
    }
}

int64 DBField::ToInt64() {
    switch (m_type) {
        case FT_STRING:
            return j_atoi<int64 > ((char*) m_value);
        case FT_FLOAT64:
            return *(f64*) m_value;
        case FT_FLOAT32:
            return *(f32*) m_value;
        case FT_INT64:
            return *(int64*) m_value;
        case FT_INT32:
            return *(int32*) m_value;
        case FT_INT16:
            return *(int16*) m_value;
        case FT_INT8:
            return *(int8*) m_value;
        case FT_UINT64:
            return *(uint64*) m_value;
        case FT_UINT32:
            return *(uint32*) m_value;
        case FT_UINT16:
            return *(uint16*) m_value;
        case FT_UINT8:
            return *(uint8*) m_value;
            break;
        case FT_BOOL:
            if (*(int*) m_value) {
                return 1;
            } else {
                return 0;
            }
        case FT_BYTE:
            return 0;
        default:
            return 0;
    }
}

int32 DBField::ToInt32() {
    switch (m_type) {
        case FT_STRING:
            return j_atoi<int64 > ((char*) m_value);
        case FT_FLOAT64:
            return *(f64*) m_value;
        case FT_FLOAT32:
            return *(f32*) m_value;
        case FT_INT64:
            return *(int64*) m_value;
        case FT_INT32:
            return *(int32*) m_value;
        case FT_INT16:
            return *(int16*) m_value;
        case FT_INT8:
            return *(int8*) m_value;
        case FT_UINT64:
            return *(uint64*) m_value;
        case FT_UINT32:
            return *(uint32*) m_value;
        case FT_UINT16:
            return *(uint16*) m_value;
        case FT_UINT8:
            return *(uint8*) m_value;
            break;
        case FT_BOOL:
            if (*(int*) m_value) {
                return 1;
            } else {
                return 0;
            }
        case FT_BYTE:
            return 0;
        default:
            return 0;
    }
}

int16 DBField::ToInt16() {
    switch (m_type) {
        case FT_STRING:
            return j_atoi<int64 > ((char*) m_value);
        case FT_FLOAT64:
            return *(f64*) m_value;
        case FT_FLOAT32:
            return *(f32*) m_value;
        case FT_INT64:
            return *(int64*) m_value;
        case FT_INT32:
            return *(int32*) m_value;
        case FT_INT16:
            return *(int16*) m_value;
        case FT_INT8:
            return *(int8*) m_value;
        case FT_UINT64:
            return *(uint64*) m_value;
        case FT_UINT32:
            return *(uint32*) m_value;
        case FT_UINT16:
            return *(uint16*) m_value;
        case FT_UINT8:
            return *(uint8*) m_value;
            break;
        case FT_BOOL:
            if (*(int*) m_value) {
                return 1;
            } else {
                return 0;
            }
        case FT_BYTE:
            return 0;
        default:
            return 0;
    }
}

int8 DBField::ToInt8() {
    switch (m_type) {
        case FT_STRING:
            return j_atoi<int64 > ((char*) m_value);
        case FT_FLOAT64:
            return *(f64*) m_value;
        case FT_FLOAT32:
            return *(f32*) m_value;
        case FT_INT64:
            return *(int64*) m_value;
        case FT_INT32:
            return *(int32*) m_value;
        case FT_INT16:
            return *(int16*) m_value;
        case FT_INT8:
            return *(int8*) m_value;
        case FT_UINT64:
            return *(uint64*) m_value;
        case FT_UINT32:
            return *(uint32*) m_value;
        case FT_UINT16:
            return *(uint16*) m_value;
        case FT_UINT8:
            return *(uint8*) m_value;
            break;
        case FT_BOOL:
            if (*(int*) m_value) {
                return 1;
            } else {
                return 0;
            }
        case FT_BYTE:
            return 0;
        default:
            return 0;
    }
}

uint64 DBField::ToUInt64() {
    switch (m_type) {
        case FT_STRING:
            return j_atoi<int64 > ((char*) m_value);
        case FT_FLOAT64:
            return *(f64*) m_value;
        case FT_FLOAT32:
            return *(f32*) m_value;
        case FT_INT64:
            return *(int64*) m_value;
        case FT_INT32:
            return *(int32*) m_value;
        case FT_INT16:
            return *(int16*) m_value;
        case FT_INT8:
            return *(int8*) m_value;
        case FT_UINT64:
            return *(uint64*) m_value;
        case FT_UINT32:
            return *(uint32*) m_value;
        case FT_UINT16:
            return *(uint16*) m_value;
        case FT_UINT8:
            return *(uint8*) m_value;
            break;
        case FT_BOOL:
            if (*(int*) m_value) {
                return 1;
            } else {
                return 0;
            }
        case FT_BYTE:
            return 0;
        default:
            return 0;
    }
}

uint32 DBField::ToUInt32() {
    switch (m_type) {
        case FT_STRING:
            return j_atoi<int64 > ((char*) m_value);
        case FT_FLOAT64:
            return *(f64*) m_value;
        case FT_FLOAT32:
            return *(f32*) m_value;
        case FT_INT64:
            return *(int64*) m_value;
        case FT_INT32:
            return *(int32*) m_value;
        case FT_INT16:
            return *(int16*) m_value;
        case FT_INT8:
            return *(int8*) m_value;
        case FT_UINT64:
            return *(uint64*) m_value;
        case FT_UINT32:
            return *(uint32*) m_value;
        case FT_UINT16:
            return *(uint16*) m_value;
        case FT_UINT8:
            return *(uint8*) m_value;
            break;
        case FT_BOOL:
            if (*(int*) m_value) {
                return 1;
            } else {
                return 0;
            }
        case FT_BYTE:
            return 0;
        default:
            return 0;
    }
}

uint16 DBField::ToUInt16() {
    switch (m_type) {
        case FT_STRING:
            return j_atoi<int64 > ((char*) m_value);
        case FT_FLOAT64:
            return *(f64*) m_value;
        case FT_FLOAT32:
            return *(f32*) m_value;
        case FT_INT64:
            return *(int64*) m_value;
        case FT_INT32:
            return *(int32*) m_value;
        case FT_INT16:
            return *(int16*) m_value;
        case FT_INT8:
            return *(int8*) m_value;
        case FT_UINT64:
            return *(uint64*) m_value;
        case FT_UINT32:
            return *(uint32*) m_value;
        case FT_UINT16:
            return *(uint16*) m_value;
        case FT_UINT8:
            return *(uint8*) m_value;
            break;
        case FT_BOOL:
            if (*(int*) m_value) {
                return 1;
            } else {
                return 0;
            }
        case FT_BYTE:
            return 0;
        default:
            return 0;
    }
}

uint8 DBField::ToUInt8() {
    switch (m_type) {
        case FT_STRING:
            return j_atoi<int64 > ((char*) m_value);
        case FT_FLOAT64:
            return *(f64*) m_value;
        case FT_FLOAT32:
            return *(f32*) m_value;
        case FT_INT64:
            return *(int64*) m_value;
        case FT_INT32:
            return *(int32*) m_value;
        case FT_INT16:
            return *(int16*) m_value;
        case FT_INT8:
            return *(int8*) m_value;
        case FT_UINT64:
            return *(uint64*) m_value;
        case FT_UINT32:
            return *(uint32*) m_value;
        case FT_UINT16:
            return *(uint16*) m_value;
        case FT_UINT8:
            return *(uint8*) m_value;
            break;
        case FT_BOOL:
            if (*(int*) m_value) {
                return 1;
            } else {
                return 0;
            }
        case FT_BYTE:
            return 0;
        default:
            return 0;
    }
}

byte* DBField::ToByte() {
    return (byte*) m_value;
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
                return true;
            } else {
                return false;
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
                return true;
            } else {
                return false;
            }
        case FT_BOOL:
            if (*(int*) m_value == 0) {
                return false;
            } else {
                return true;
            }
        case FT_BYTE:
            return false;
        default:
            return false;
    }
}

}//namespace database
}//namespace shared
}//namespace common
}//namespace jRylServer