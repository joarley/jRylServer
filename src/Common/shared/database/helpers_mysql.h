#ifndef HELPERS_MYSQL_H
#define HELPERS_MYSQL_H

#include "shared/cBuffer.h"
#include "shared/typedef.h"
#include "shared/cLogger.h"
#include "shared/database/TypeTrait_mysql.h"

#include "boost/utility/enable_if.hpp"
#include "boost/type_traits/is_unsigned.hpp"

#include <mysql.h>
#include <cstring>

using namespace jRylServer::common::shared;

static jRylServer::common::shared::Logger* LOG = jRylServer::common::shared::Logger::GetInstance_ptr();


inline MYSQL_STMT* my_prepare(const char* text, MYSQL* conn) {
    MYSQL_STMT* stmt = mysql_stmt_init(conn);
    if (!stmt) {
        LOG->ShowError("my_prepare()[mysql_stmt_init()]: out of memory\n");
        return NULL;
    }
    if(mysql_stmt_prepare(stmt, text, static_cast<unsigned long>(strlen(text)))) {
        LOG->ShowError("my_prepare()[mysql_stmt_prepare()]: [%d] %s.\n", mysql_stmt_errno(stmt), mysql_stmt_error(stmt));
        return NULL;
    }
    return stmt;
}

template<class T> inline void my_prepare_bind(MYSQL_BIND* res, T& value) {
    res->buffer = (void*)&value;
    res->buffer_type = DBTypeTrait<T>::Mysql::Type();
    res->buffer_length = sizeof(T);
    res->is_null = &res->is_null_value;
    if(boost::is_unsigned<T>::value) {
        res->is_unsigned = 1;
    }
}

inline void my_prepare_bind(MYSQL_BIND* res, Buffer_ptr value) {
    res->buffer = (void*)value->Data();
    res->buffer_type = DBTypeTrait<Buffer_ptr>::Mysql::Type();
    res->buffer_length = static_cast<unsigned long>(value->MaxLength());
    res->length = &res->buffer_length;
    res->is_null = &res->is_null_value;
}

inline void my_prepare_bind(MYSQL_BIND* res, const char* value, size_t size) {
    res->buffer = (void*)value;
    res->buffer_type = DBTypeTrait<const char*>::Mysql::Type();
    res->buffer_length = static_cast<unsigned long>(size);
    res->is_null = &res->is_null_value;
}

#endif