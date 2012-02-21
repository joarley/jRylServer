#ifndef TYPETRAITS_MYSQL_H
#define TYPETRAITS_MYSQL_H

#include "shared/cBuffer.h"
#include "shared/typedef.h"

#include <mysql.h>

#include <ctime>
namespace {
template<class T> struct MysqlTrait {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_NULL;
    }
};

template<> struct MysqlTrait<bool> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_TINY;
    }
};

template<> struct MysqlTrait<int8> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_TINY;
    }
};

template<> struct MysqlTrait<uint8> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_TINY;
    }
};

template<> struct MysqlTrait<int16> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_SHORT;
    }
};

template<> struct MysqlTrait<uint16> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_SHORT;
    }
};

template<> struct MysqlTrait<int32> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_LONG;
    }
};

template<> struct MysqlTrait<uint32> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_LONG;
    }
};

template<> struct MysqlTrait<int64> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_LONGLONG;
    }
};

template<> struct MysqlTrait<uint64> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_LONGLONG;
    }
};

template<> struct MysqlTrait<f32> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_FLOAT;
    }
};

template<> struct MysqlTrait<f64> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_DOUBLE;
    }
};

template<> struct MysqlTrait<const char*> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_STRING;
    }
};

template<> struct MysqlTrait<tm> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_DATETIME;
    }
};

template<> struct MysqlTrait<jRylServer::common::shared::Buffer_ptr> {
    inline static enum_field_types GetType() {
        return MYSQL_TYPE_BLOB;
    }
};
}

template <class T> struct DBTypeTrait {
    struct Mysql {
        inline static enum_field_types Type() {return MysqlTrait<T>::GetType();};
    };
};


#endif
