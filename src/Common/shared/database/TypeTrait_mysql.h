#ifndef TYPETRAITS_MYSQL_H
#define TYPETRAITS_MYSQL_H

#include "shared/cBuffer.h"
#include "shared/typedef.h"

#include <mysql.h>

#include <ctime>

template<class T> struct TypeTrait_Mysql {
    static enum_field_types Type;
};

template<class T> enum_field_types TypeTrait_Mysql<T>::Type = MYSQL_TYPE_NULL;
template<> enum_field_types TypeTrait_Mysql<bool>::Type = MYSQL_TYPE_TINY;
template<> enum_field_types TypeTrait_Mysql<int8>::Type = MYSQL_TYPE_TINY;
template<> enum_field_types TypeTrait_Mysql<uint8>::Type = MYSQL_TYPE_TINY;
template<> enum_field_types TypeTrait_Mysql<int16>::Type = MYSQL_TYPE_SHORT;
template<> enum_field_types TypeTrait_Mysql<uint16>::Type = MYSQL_TYPE_SHORT;
template<> enum_field_types TypeTrait_Mysql<int32>::Type = MYSQL_TYPE_LONG;
template<> enum_field_types TypeTrait_Mysql<uint32>::Type = MYSQL_TYPE_LONG;
template<> enum_field_types TypeTrait_Mysql<int64>::Type = MYSQL_TYPE_LONGLONG;
template<> enum_field_types TypeTrait_Mysql<uint64>::Type = MYSQL_TYPE_LONGLONG;
template<> enum_field_types TypeTrait_Mysql<f32>::Type = MYSQL_TYPE_FLOAT;
template<> enum_field_types TypeTrait_Mysql<f64>::Type = MYSQL_TYPE_DOUBLE;
template<> enum_field_types TypeTrait_Mysql<const char*>::Type = MYSQL_TYPE_STRING;
template<> enum_field_types TypeTrait_Mysql<tm>::Type = MYSQL_TYPE_DATETIME;
template<> enum_field_types TypeTrait_Mysql<jRylServer::common::shared::Buffer_ptr>::Type = MYSQL_TYPE_BLOB;

#endif